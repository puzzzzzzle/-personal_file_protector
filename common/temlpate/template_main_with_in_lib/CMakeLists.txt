#子项目名 替换template_name为子项目名
set(build_temp template_name)


message("\n###############################\nstart cmake ${build_temp}")
aux_source_directory(./ ${build_temp}cpps)
include_directories(./)
message("${build_temp}cpps find cpps : ${${build_temp}cpps}")

# 测试用的可执行文件
add_executable(${build_temp} ${${build_temp}cpps}

        #以下为common_funcs 的cpp文件
        ${PROJECT_SOURCE_DIR}/common/common_include/common_funcs.cpp
        ${PROJECT_SOURCE_DIR}/common/common_include/log_init.cpp

        #IDE不识别aux到的cpp文件，不进行符号表处理，只能手动变一遍
        #如果发布，删除以下内容

        )

# 输出库
add_library(lib_${build_temp} SHARED ${${build_temp}cpps})
# 复制配置文件
add_custom_command(
        TARGET ${build_temp}
        COMMAND echo copying files:
        COMMAND rm -rf `pwd`/configure/
        COMMAND mkdir `pwd`/configure/
        COMMAND cp -v ${PROJECT_SOURCE_DIR}/common/properties/boost_log_settings.ini `pwd`/configure/
        COMMAND echo copying files is finished.
)
#boost log 的宏
add_definitions(-DBOOST_LOG_DYN_LINK)
#通用动态链接库
target_link_libraries(${build_temp} gtest pthread boost_log boost_log_setup boost_thread boost_system)
# 链接本项目的库
include_directories(${PROJECT_SOURCE_DIR}/src/crypto_adaptor)
target_link_libraries(${build_temp} _template_lib_name)

message("${build_temp} cmake finish\n###############################\n")