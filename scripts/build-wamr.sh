num=$(ls wasm-micro-runtime | wc -l)
if [ $num == 0 ]
then
    git submodule update --init --recursive
    echo "Cloned wasm-micro-runtime!"
fi


CURR_DIR=$(pwd)
echo "#####################build basic project"
cd ${CURR_DIR}
mkdir -p cmake_build
cd cmake_build
cmake .. -DWAMR_BUILD_BULK_MEMORY=1
make
if [ $? != 0 ];then
    echo "BUILD_FAIL basic exit as $?\n"
    exit 2
fi

# cp -a basic ${OUT_DIR}

echo -e "\n"

echo "#####################build wasm apps"
