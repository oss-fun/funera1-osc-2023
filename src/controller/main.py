import signal
import time
import asyncio
import os
import zipfile
import base64
import requests
from fastapi import FastAPI, File, UploadFile, Form
from fastapi.responses import FileResponse, JSONResponse

app = FastAPI()

# Noneか現在走ってるprocessを持つ
global running_proc
running_proc = None


@app.get("/")
def read_root():
    return {"Hello": "World"}

@app.post("/app/init")
def init():
    global running_proc
    running_proc = None

@app.post("/app/start")
async def run():
    global running_proc
    if running_proc != None:
        return "Proccess is already running."

    wasm_executor = "../../cmake_build/basic"
    wasm_app = "../../src/wasi-lifegame/target/wasm32-wasi/debug/wasi-lifegame.wasm"

    args = ["-f", wasm_app]
    proc = await asyncio.create_subprocess_exec(
            wasm_executor,
            *args,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE,
        )

    running_proc = proc
    return proc.pid

@app.post("/app/restore")
async def restore():
    global running_proc
    if running_proc != None:
        return "Proccess is already running."

    wasm_executor = "../../cmake_build/basic"

    #NOTE: imgファイルについての操作をする必要があるかも
    args = ["-r", "0"]
    proc = await asyncio.create_subprocess_exec(
            wasm_executor,
            *args,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE,
        )

    running_proc = proc
    return proc.pid


@app.post("/app/stop")
def stop():
    global running_proc
    if running_proc == None:
        return "Proccess is not running."

    running_proc.send_signal(signal.SIGINT)
    running_proc = None
    time.sleep(1)
    
    if not (os.path.exists("interp.img") and os.path.exists("dump.img") and os.path.exists("frame.img") and os.path.exists("pool_info.img")):
        return "Not Found img file"

    with zipfile.ZipFile('imgs.zip', 'w', zipfile.ZIP_DEFLATED) as zf:
        zf.write("interp.img")
        zf.write("dump.img")
        zf.write("frame.img")
        zf.write("pool_info.img")

    os.remove("interp.img")
    os.remove("dump.img")
    os.remove("frame.img")
    os.remove("pool_info.img")

    return "Success"

@app.post("/app/migrate")
async def migrate(host: str, port: int):
    if not os.path.exists("imgs.zip"):
        return JSONResponse(status_code=400, content={"msg": "imgs.zip is not found"})
    # response = FileResponse(path = "./imgs.zip")
    with open("imgs.zip", "rb") as f:
        response = f.read()
    b = str(base64.b64encode(response))[2:-1]

    response = requests.post('http://' + host + ":" + str(port) + '/img', data={'enc': b})
    if response.status_code == 200:
        return JSONResponse(status_code=200, content={"msg": "Success"})
    else:
        return JSONResponse(status_code=400, content={"msg": "Failed to migration"})

@app.post("/img")
async def get_imgs(enc: str = Form()):
    if os.path.exists("imgs.zip"):
        os.replace("imgs.zip", "old-imgs.zip")

    try:
        b = base64.b64decode(enc, validate=True)
    except:
        return JSONResponse(status_code=400, content={"msg": "input string can't decode."})

    f = open("imgs.zip", 'wb')
    f.write(b)
    f.close()

    with zipfile.ZipFile('imgs.zip') as zf:
        if zf.namelist() != ["interp.img", "dump.img", "frame.img", "pool_info.img"]:
            return JSONResponse(status_code=400, content={"msg": "imgs.zip is not expected"})
        
        zf.extractall()

    return JSONResponse(status_code=200, content={"msg": "Success"})
