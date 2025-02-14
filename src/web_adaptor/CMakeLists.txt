#子项目名 替换template_name为子项目名
set(build_name web_adaptor)

message("\n###############################\nstart cmake ${build_name}")
aux_source_directory(./ ${build_name}cpps)
include_directories(./)
message("${build_name}cpps find cpps : ${${build_name}cpps}")

# 测试用的可执行文件
add_executable(${build_name} ${${build_name}cpps} main/main.cpp

        #以下为common_funcs 的cpp文件
        ${PROJECT_SOURCE_DIR}/common/common_include/common_funcs.cpp
        ${PROJECT_SOURCE_DIR}/common/common_include/log_init.cpp

        #IDE不识别aux到的cpp文件，不进行符号表处理，只能手动变一遍
        #如果发布，删除以下内容

        )

# 输出库
add_library(${build_name}_dev SHARED ${${build_name}cpps})
# 复制配置文件
add_custom_command(
        TARGET ${build_name}
        COMMAND echo copying files:
        COMMAND rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configure/
        COMMAND mkdir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configure/
        COMMAND cp -v ${PROJECT_SOURCE_DIR}/common/properties/boost_log_settings.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configure/
        COMMAND rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res/
        COMMAND cp -vr ${PROJECT_SOURCE_DIR}/common/res/default ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
        COMMAND cp -vr ${PROJECT_SOURCE_DIR}/common/res/qscmd.conf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/

        COMMAND echo copying files is finished.
)
#boost log 的宏
add_definitions(-DBOOST_LOG_DYN_LINK -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast)

#通用动态链接库
target_link_libraries(${build_name} gtest pthread boost_log boost_log_setup boost_thread boost_system boost_filesystem )

message("${build_name} cmake finish\n###############################\n")