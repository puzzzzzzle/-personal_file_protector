#子项目名 替换template_name为子项目名
set(build_name main_passwd)

message("\n###############################\nstart cmake ${build_name}")

aux_source_directory(./ ${build_name}cpps)
include_directories(./)

message("${build_name}cpps find cpps : ${${build_name}cpps}")


# 输出库
add_library(${build_name}_dev SHARED ${${build_name}cpps})


message("${build_name} cmake finish\n###############################\n")