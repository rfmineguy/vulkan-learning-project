#
# Check if /build/ exists and create it if not
#
build() {
    if [ ! -d "${PWD}/build/" ]; then
        echo "Build folder doesn't exist, creating it."
        mkdir build
    fi
    cd build
    cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=on
    cmake --build .   
}

if [[ $1 == "-debug"* ]]; then
    build
    lldb ./vk_demo
elif [[ $1 == "-normal"* ]]; then
    build
    ./vk_demo
else
    echo "Defaulting to -normal"
    build
    ./vk_demo
fi
