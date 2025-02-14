#子项目名 替换template_name为子项目名
set(build_name passwd_adaptor)

message("\n###############################\nstart cmake ${build_name}")
aux_source_directory(./ ${build_name}cpps)
include_directories(./)
message("${build_name}cpps find cpps : ${${build_name}cpps}")

set(common_cpp ${PROJECT_SOURCE_DIR}/common/common_include/common_funcs.cpp ${PROJECT_SOURCE_DIR}/common/common_include/log_init.cpp)
# 测试用的可执行文件
add_executable(${build_name} ${${build_name}cpps} main/main.cpp ${common_cpp}
        #IDE不识别aux到的cpp文件，不进行符号表处理，只能手动变一遍
        #如果发布，删除以下内容

        )

# 输出库 和可执行文件保持一致
add_library(${build_name}_dev SHARED ${${build_name}cpps} ${common_cpp})

# 复制配置文件
add_custom_command(
        TARGET ${build_name}
        COMMAND echo copying files:
        COMMAND rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configure/
        COMMAND mkdir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configure/
        COMMAND cp -v ${PROJECT_SOURCE_DIR}/common/properties/boost_log_settings.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/configure/
        COMMAND echo copying files is finished.
)
#boost log 的宏
add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(${PROJECT_SOURCE_DIR}/src/main_passwd)
include_directories(${PROJECT_SOURCE_DIR}/src/crypto_adaptor)

set(local_libs main_passwd_dev crypto_adaptor_dev)
#动态链接库依赖
set(libs gtest pthread boost_log boost_log_setup boost_thread boost_system boost_filesystem yaml-cpp ${local_libs})
#可执行文件添加通用动态链接库依赖
target_link_libraries(${build_name} ${libs})
#动态链接库也要依赖
target_link_libraries(${build_name}_dev ${libs})

message("${build_name} cmake finish\n###############################\n")