cmake_minimum_required(VERSION 3.20)
project(cpp_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找所有 cpp 文件
file(GLOB_RECURSE CPP_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

foreach(cpp_file ${CPP_FILES})
    # 计算相对路径（相对于 src）
    file(RELATIVE_PATH rel_path ${PROJECT_SOURCE_DIR}/src ${cpp_file})

    # 去掉 .cpp 后缀
    string(REPLACE ".cpp" "" target_path ${rel_path})

    # target 名字（防止重名）
    string(REPLACE "/" "_" target_name ${target_path})

    add_executable(${target_name} ${cpp_file})

    # 获取目录路径（保持 src 目录结构）
    get_filename_component(dir_path ${target_path} DIRECTORY)

    # 设置二进制输出目录 = bin + src 子目录结构
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${dir_path}
    )
endforeach()

