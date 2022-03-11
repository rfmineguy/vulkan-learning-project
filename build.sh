#
# Check if /build/ exists and create it if not
#
build_dir_check() {
    if [ ! -d "${PWD}/build" ]; then
        mkdir build
    fi
    cd build
}
build() {
    build_dir_check
    cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=on
    cmake --build . #-v
}
if [[ $1 == "-validation"* ]]; then
    build
    ./vk_demo
elif [[ $1 == "-novalidation"* ]]; then
    build
    ./vk_demo
else
    build
    ./vk_demo
fi
