# funera1-osc-2023


## Directory
```bash
├── src 
│   ├── wasi-lifegame # Wasm application
│   ├── executor      # Wasm application executor
│   └──controller     # Http client and server
├── wasm-micro-runtime
└── scripts
```

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
