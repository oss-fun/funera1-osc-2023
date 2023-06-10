# funera1-osc-2023


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