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

    return proc


def signal_handler(signum, frame):
    print("STOP")

def stop(pid):
    signal.signal(signal.SIGINT, signal_handler)

proc = asyncio.run(run())
print("return async run")

time.sleep(1)
print("sleep(1)")

# stop(proc)
proc.send_signal(signal.SIGINT)
print("stop process")
