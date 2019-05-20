//
// Created by tao on 19-3-25.
//

#include "file_controller_adaptor.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include "log_init.h"
#include "AES_func_adaptor.h"
#include "get_usr_passwd.h"
#include "file_key_controller.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INDEX_FILE_FREFIX "./sysdata/%d.index.data"

std::mutex MainFileController::lock{};
MainFileController *MainFileController::Instance = new MainFileController();

int MainFileController::loadTable() {
    LockGround lockGround(&lock);
    char indexBuff[256]{};
    snprintf(indexBuff, sizeof(indexBuff),INDEX_FILE_FREFIX,userID);
    std::string indexFile(indexBuff);
    namespace bf=boost::filesystem;
    bf::ifstream infile(indexFile);
    if (!infile) {
        INFO("file don't exists :" << indexFile)
        return 0;
    }
    size_t buffLen = bf::file_size(indexFile) * 10;
    char *buff = (char *) malloc(buffLen);
    char *decBuff = (char *) malloc(buffLen);

    size_t dataLen = 0;
    while (!infile.eof()) {
        infile.read(buff + dataLen, buffLen);
        dataLen += infile.gcount();
    }
    if (aesDataToData(get_usr_key(userID), buff, dataLen, decBuff, buffLen, 0)) {
        ERROR_LOG("dec index fail :" << indexFile)
        free(buff);
        free(decBuff);
        return -1;
    }
    decBuff[buffLen] = '\0';
    DEBUG("yaml dec is" << decBuff)

    YAML::Node root = YAML::Load(decBuff);
    table.tab.clear();
    table << root;
    free(buff);
    free(decBuff);
    return 0;
}

int MainFileController::saveTable() {
    LockGround lockGround(&lock);
    //输出字符串
    YAML::Node node;
    table >> node;
    INFO("before enc : " << node)
    std::ostringstream outString;
    outString << node;
    std::string yamlNode = outString.str();
    DEBUG("yaml origin is" << yamlNode)
    size_t outLen = yamlNode.length() * 10;
    char *decBuff = (char *) malloc(outLen);

    //加密
    if (aesDataToData(get_usr_key(userID), yamlNode.c_str(), yamlNode.length(), decBuff, outLen, 1)) {
        ERROR_LOG("enc index fail ")
        free(decBuff);
        return -1;
    }
    decBuff[outLen] = '\0';
    //保存
    namespace bf=boost::filesystem;
    char indexBuff[256]{};
    snprintf(indexBuff, sizeof(indexBuff),INDEX_FILE_FREFIX,userID);
    std::string indexFile(indexBuff);

    bf::ofstream outfile(indexFile, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!outfile) {
        ERROR_LOG("file don't exists :" << indexFile)
        free(decBuff);
        return -1;
    }
    outfile.write(decBuff, outLen);
    free(decBuff);
    return 0;
}

int
MainFileController::encAndInsertFile(size_t &currSize, int encKey, std::string originPath, std::string encName) {
    std::string destFile = getRandFileName();
    if (aesFileToFile(get_file_key(userID, encKey), originPath, destFile, 1, currSize)) {
        ERROR_LOG("err when aes the file :" << originPath << " to :" << destFile);
        return -1;
    }
    EncINode iNode{};
    iNode.encFileType = FILE_TYPE_FILE;
    iNode.encName = encName;
    iNode.encMode = FILE_ENC_DES_CBC;

    iNode.encCryptFileName = destFile;
    iNode.encFileKey = encKey;
    table.insert(iNode);
    return 0;
}

int MainFileController::decToFile(size_t &currSize, int encKey, std::string originFileName, std::string destFileName) {
    if (destFileName == "") {
        return -1;
    }
    return aesFileToFile(get_file_key(userID, encKey), originFileName, destFileName, 0, currSize);
}

int MainFileController::decToFd(size_t &currSize, int encKey, std::string originFileName, int fd) {
    LockGround lockGround(&lock);
    namespace bf=boost::filesystem;
    if ((!bf::exists(originFileName))
        || (!bf::is_regular(originFileName))) {
        ERROR_LOG("enc file not exists!")
        return -1;
    }
    return aesFdToFd(get_file_key(userID, encKey), open(originFileName.c_str(), std::ios::in | std::ios::binary), fd, 0,
                     currSize);
}

