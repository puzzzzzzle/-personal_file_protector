//
// Created by Administrator on 2019/4/14.
//

#include "change_rodata.h"
#include "common_includes.h"
#include "common_funcs.h"
#include <sys/mman.h>
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int rand_rodata() {
    namespace bf=boost::filesystem;
    bf::path fPath = SO_FILE;
    if ((!bf::exists(fPath))
        || (!bf::is_regular(fPath))) {
        ERROR_LOG("err file :" << fPath.string())
        return -1;
    }
    int fd = open(fPath.string().c_str(), O_RDWR);
    size_t len = bf::file_size(fPath);
    char *ptr = (char *) mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char *dest = (ptr + POS);
    printf("get:\n%s\n", dest);
    for (int i = 0; i < 30; ++i) {
        *(dest + i) = randChar();
        DEBUG("inline for:" << dest);
    }
    DEBUG("set:" << dest);
    if (munmap(ptr, len)) {
        ERROR_LOG("err when mumap")
        return -1;
    }
    return 0;
}
