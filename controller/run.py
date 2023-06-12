import subprocess, sys
import signal
import time
import asyncio


async def run():
    wasm_executor = "../cmake_build/basic"
    wasm_app = "../wasi-lifegame/target/wasm32-wasi/debug/wasi-lifegame.wasm"

    args = ["-f", wasm_app]
    proc = await asyncio.create_subprocess_exec(
            wasm_executor,
            *args,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE,

        )

    # while True:
    #     stdout = (await proc.stdout.readline()).decode()
    #     if stdout:
    #         print(f'[stdout] {stdout}', end='', flush=True)
    #     stderr = (await proc.stderr.readline()).decode()
    #     if stderr:
    #         print(f'[sdterr] {stderr}', end='', flush=True, file=sys.stderr)
    # 
    #     await asyncio.sleep(1)
    return proc

    # proc = subprocess.run([wasm_executor, "-f", wasm_app])
    # return proc


def signal_handler(signum, frame):
    print("STOP")

def stop(pid):
    signal.signal(signal.SIGINT, signal_handler)

proc = asyncio.run(run())

time.sleep(10)

stop(proc)

