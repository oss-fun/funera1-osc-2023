# funera1-osc-2023


## Direcotry
src: Wasm application exectuor
wasi-lifegame: Wasm application

## How to use
### Build runtime
```
bash scripts/build-wamr.sh
```

### Build application
```
cd wasi-lifegame
cargo wasi build
```

### Run application
```
cmake_build/basic -f wasi-lifegame/target/wasm32-wasi/debug/wasi-lifegame.wasm
```
