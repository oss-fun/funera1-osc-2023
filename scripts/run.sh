#!/bin/bash

wasm_executor=$1
$wasm_executor -f wasi-lifegame/target/wasm32-wasi/debug/wasi-lifegame.wasm
