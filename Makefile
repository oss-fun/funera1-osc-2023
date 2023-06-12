wasm_executor := cmake_build/basic
build-wamr:
	bash scripts/build-wamr.sh 

build-app:
	bash scripts/build-app.sh

run-app:
	bash scripts/run.sh ${wasm_executor}

restore-app:
	bash scripts/restore.sh ${wasm_executor}
