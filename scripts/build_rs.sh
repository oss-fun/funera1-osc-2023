rustc --target wasm32-wasi wasm-apps/lifegame.rs
mv wasm-apps/lifegame.wasm out/
out/basic -f out/lifegame.wasm
