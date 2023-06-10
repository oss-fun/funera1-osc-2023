echo "#####################build basic project"
# cd ${CURR_DIR}
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
