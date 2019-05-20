/**************************************************
 * @FileName: work_mode.h
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:31  1.0         create
**************************************************/
#ifndef PERSONAL_FILE_PROTECTOR_WORK_MODE_H
#define PERSONAL_FILE_PROTECTOR_WORK_MODE_H
enum Platform {
    PLAT_ALL = 0,
    PLAT_CYGWIN = 1,
    PLAT_LINIX = 2,

    PLAT_NONE = 10,
};
enum WorkMode {
    ADAPTOR_MODE_NONE = 0,
    ADAPTOR_MODE_STRING = 1,
    ADAPTOR_MODE_FILE = 2,
    ADAPTOR_MODE_DATE = 3,      //通过ADAPTOR_MODE_DATE_BASE64在测试配置中调用
    ADAPTOR_MODE_DATE_BASE64 = 4,

    ADAPTOR_MODE_MAX = 10,
};
enum CryptType {
    ADAPTOR_TYPE_NONE = 0,
    //hash 类

    ADAPTOR_CRYPT_MD5 = 1,
    ADAPTOR_CRYPT_SHA256 = 2,

    ADAPTOR_CRYPT_HASH_MAX = 100,

    //base64类

    ADAPTOR_CRYPT_BASE64_ENCODE = 101,
    ADAPTOR_CRYPT_BASE64_DECODE = 102,

    ADAPTOR_CRYPT_BASE64_MAX = 200,

    //对称加密AES类

    ADAPTOR_CRYPT_AES_ENCODE = 201,
    ADAPTOR_CRYPT_AES_DECODE = 202,

    ADAPTOR_CRYPT_AES_MAX = 300,

    ADAPTOR_MAX = 99999,
};

#endif //PERSONAL_FILE_PROTECTOR_WORK_MODE_H
