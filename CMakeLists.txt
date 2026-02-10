cmake_minimum_required(VERSION 3.20)
project(cpp_learning LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找所有 cpp 文件（CONFIGURE_DEPENDS 自动检测文件变化）
file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)

foreach(cpp_file ${CPP_FILES})
    # 计算相对路径（相对于 src）
    file(RELATIVE_PATH rel_path ${PROJECT_SOURCE_DIR}/src ${cpp_file})

    # 去掉 .cpp 后缀
    string(REPLACE ".cpp" "" target_path ${rel_path})

    # target 名字（防止重名）
    string(REPLACE "/" "_" target_name ${target_path})

    add_executable(${target_name} ${cpp_file})
    
    target_include_directories(${target_name} PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
    )

    # 获取目录路径（保持 src 目录结构）
    get_filename_component(dir_path ${target_path} DIRECTORY)

    # 设置二进制输出目录 = bin + src 子目录结构
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${dir_path}
    )
endforeach()

# 辅助目标：列出所有可执行文件（方便 VS Code 和调试）
add_custom_target(list_executables
    COMMAND ${CMAKE_COMMAND} -E echo "Available executables:"
    COMMAND ${CMAKE_COMMAND} -E echo ""
)

foreach(cpp_file ${CPP_FILES})
    file(RELATIVE_PATH rel_path ${PROJECT_SOURCE_DIR}/src ${cpp_file})
    string(REPLACE ".cpp" "" target_path ${rel_path})
    string(REPLACE "/" "_" target_name ${target_path})
    add_custom_command(TARGET list_executables POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "  - ${target_name} -> bin/${target_path}"
    )
endforeach()

# 设置导出编译命令（有助于 IDE 识别）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ========== 自动清理 bin 目录中无效的可执行文件 ==========

# 收集所有期望的可执行文件完整路径（bin 目录下的相对路径）
# 实际路径格式: bin/<dir>/<target_name>，如 bin/algo/algo_sort
set(EXPECTED_BIN_PATHS "")
foreach(cpp_file ${CPP_FILES})
    file(RELATIVE_PATH rel_path ${PROJECT_SOURCE_DIR}/src ${cpp_file})
    string(REPLACE ".cpp" "" target_path ${rel_path})
    string(REPLACE "/" "_" target_name ${target_path})
    get_filename_component(dir_path ${target_path} DIRECTORY)
    if(dir_path)
        list(APPEND EXPECTED_BIN_PATHS "${dir_path}/${target_name}")
    else()
        list(APPEND EXPECTED_BIN_PATHS "${target_name}")
    endif()
endforeach()

# 收集 bin 目录下所有实际的可执行文件
file(GLOB_RECURSE ACTUAL_BIN_FILES 
    ${PROJECT_SOURCE_DIR}/bin/*
)

# 清理无效的可执行文件
foreach(bin_file ${ACTUAL_BIN_FILES})
    if(IS_EXECUTABLE ${bin_file})
        # 计算相对路径
        file(RELATIVE_PATH rel_bin_path ${PROJECT_SOURCE_DIR}/bin ${bin_file})
        # 检查是否在期望列表中
        list(FIND EXPECTED_BIN_PATHS ${rel_bin_path} INDEX)
        if(INDEX EQUAL -1)
            message(STATUS "清理无效的可执行文件: bin/${rel_bin_path}")
            file(REMOVE ${bin_file})
        endif()
    endif()
endforeach()

# 递归清理空目录（最多10轮，防止无限循环）
set(CLEANUP_ITERATIONS 0)
while(CLEANUP_ITERATIONS LESS 10)
    set(FOUND_EMPTY_DIR FALSE)
    # 使用 LIST_DIRECTORIES true 来匹配目录
    file(GLOB_RECURSE ALL_BIN_DIRS LIST_DIRECTORIES true ${PROJECT_SOURCE_DIR}/bin/*)
    # 按深度排序（深层目录先处理）
    list(SORT ALL_BIN_DIRS)
    list(REVERSE ALL_BIN_DIRS)
    
    foreach(dir ${ALL_BIN_DIRS})
        if(IS_DIRECTORY ${dir})
            file(GLOB DIR_CONTENTS ${dir}/*)
            if(NOT DIR_CONTENTS)
                file(REMOVE_RECURSE ${dir})
                message(STATUS "清理空目录: ${dir}")
                set(FOUND_EMPTY_DIR TRUE)
            endif()
        endif()
    endforeach()
    
    if(NOT FOUND_EMPTY_DIR)
        break()
    endif()
    math(EXPR CLEANUP_ITERATIONS "${CLEANUP_ITERATIONS} + 1")
endwhile()

