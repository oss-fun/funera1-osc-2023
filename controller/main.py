import signal
import asyncio
from fastapi import FastAPI

app = FastAPI()

# Noneか現在走ってるprocessを持つ
global running_proc
running_proc = None


@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.post("/app/start")
async def run():
    global running_proc
    if running_proc != None:
        return "Proccess is already running."

    wasm_executor = "../cmake_build/basic"
    wasm_app = "../wasi-lifegame/target/wasm32-wasi/debug/wasi-lifegame.wasm"

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

    wasm_executor = "../cmake_build/basic"

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
    
    return "Success"

