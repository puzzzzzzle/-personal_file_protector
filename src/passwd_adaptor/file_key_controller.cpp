//
// Created by tao on 19-3-25.
//
#include "file_key_controller.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include "log_init.h"
#include "AES_func_adaptor.h"
#include "get_usr_passwd.h"
#include "file_key_controller.h"
#include "../crypto_adaptor/AES_func_adaptor.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_KEY_PREFIX "./sysdata"
FileKeyTable *FileKeyTable::Instance = new FileKeyTable();
std::mutex FileKeyTable::lock{};

std::string get_file_key(int user, int para) {
    FileKeyTable *ptr = FileKeyTable::Instance;
    if (ptr->userID == -1
        || ptr->userID != user) {
        ptr->userID = user;
        ptr->loadKeyTab();
    }
    return ptr->getKey(para);
}

void FileKeyNode::operator<<(const YAML::Node &node) {
    this->id = node["id"].as<int>();
    this->key = node["key"].as<std::string>();
}

void FileKeyNode::operator>>(YAML::Node &node) const {
    node["id"] = this->id;
    node["key"] = this->key;
}

std::string FileKeyTable::getKey(int id) {
    return tab[id];
}

int FileKeyTable::insertKey(const FileKeyNode &node) {
    return tab.insert(std::make_pair(node.id, node.key)).second;
}

int FileKeyTable::insertKey(const std::string &key) {
    int _id{};
    do {
        _id = rand();
        auto dest = tab.find(_id);
        if (dest == tab.end()) {
            break;
        }
    } while (true);
    FileKeyNode node;
    node.id = _id;
    node.key = key;

    return insertKey(node);
}

void FileKeyTable::operator<<(const YAML::Node &node) {
    FileKeyNode iNode;
    for (auto n:node) {
        iNode.clear();
        iNode << n;
        tab.insert(std::make_pair(iNode.id, iNode.key));
    }
}

void FileKeyTable::operator>>(YAML::Node &node) const {
    for (const auto &n:tab) {
        YAML::Node temp;
        FileKeyNode(n.first, n.second) >> temp;
        node.push_back(temp);
    }
}

int FileKeyTable::loadKeyTab() {
    LockGround lockGround(&lock);

    char userIDStr[64]{};
    sprintf(userIDStr, "%s/%d.filePasswd.data", FILE_KEY_PREFIX, userID);

    namespace bf=boost::filesystem;
    bf::path userKeyFile(userIDStr);
    bf::ifstream infile(userKeyFile);
    if (!infile) {
        INFO("file not exists :" << userIDStr)
        tab.insert(std::make_pair(0, "default file key,don't use it!"));
        return 0;
    }
    size_t buffLen = bf::file_size(userKeyFile) * 3;
    char *buff = (char *) malloc(buffLen);
    char *decBuff = (char *) malloc(buffLen);

    size_t dataLen = 0;
    while (!infile.eof()) {
        infile.read(buff + dataLen, buffLen);
        dataLen += infile.gcount();
    }
    if (aesDataToData(get_usr_key(userID), buff, dataLen, decBuff, buffLen, 0)) {
        ERROR_LOG("dec index fail :" << userIDStr)
        free(buff);
        free(decBuff);
        return -1;
    }
    decBuff[buffLen] = '\0';
//    DEBUG("yaml dec is" << decBuff)

    YAML::Node root = YAML::Load(decBuff);
    tab.clear();
    *Instance << root;
    free(buff);
    free(decBuff);
    if(tab.size()==0){
        FileKeyNode keyNode;
        keyNode.id=0;
        keyNode.key="asdfajklshflashdflasdkfjlasdfj";
        insertKey(keyNode);
    }
    return 0;
}

int FileKeyTable::savrKeyTab() {
    LockGround lockGround(&lock);
    //输出字符串
    YAML::Node node;
    *Instance >> node;
    std::ostringstream outString;
    outString << node;
    std::string yamlNode = outString.str();
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

    char userIDStr[64]{};
    sprintf(userIDStr, "%s/%d.filePasswd.data", FILE_KEY_PREFIX, userID);

    bf::ofstream outfile(userIDStr, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!outfile) {
        ERROR_LOG("file don't exists :" << userIDStr)
        free(decBuff);
        return -1;
    }
    outfile.write(decBuff, outLen);
    free(decBuff);
    return 0;
}

