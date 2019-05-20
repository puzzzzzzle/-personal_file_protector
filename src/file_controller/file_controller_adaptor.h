//
// Created by tao on 19-3-25.
//

#ifndef PERSONAL_FILE_PROTECTOR_MAIN_FILE_CONTROLLER_H
#define PERSONAL_FILE_PROTECTOR_MAIN_FILE_CONTROLLER_H

#include "file_controller.h"
#include <mutex>
#include <stdlib.h>
#include "log_init.h"
#include "common_classes.h"
#include <boost/filesystem.hpp>


#define DIR_FILE "./filedata/"

class MainFileController {
private:
    MainFileController() = default;

public:
    MainFileController(MainFileController &) = delete;

    MainFileController(MainFileController &&) = delete;

    void destroy(){
        delete this;
    }
private:
    static std::mutex lock;
    ~MainFileController(){

    }
public:
    static MainFileController *Instance;
public:
    EncITable table{};
    int userID{};


public:
    int loadTable();

    int saveTable();

public:
    int encAndInsertFile(size_t &currSize, int encKey, std::string originPath, std::string encName = "");

    int decToFile(size_t &currSize, int encKey, std::string originFileName, std::string destFileName);

    int decToFd(size_t &currSize, int encKey, std::string originFileName, int fd);

    EncINode *selectByName(std::string name) {
        return table.selectByName(name);
    }

    std::vector<EncINode *> selectAllDirByName(std::string name) {
        return table.selectAllDirByName(name);
    }

    std::vector<EncINode *> selectDirFileByName(std::string name) {
        return table.selectDirByName(name);
    }

    std::map<std::string, EncINode> *
    selectFileWithDirByName(std::string name, std::map<std::string, EncINode> *result) {
        return table.selectFileWithDirByName(name, result);
    }

    int change(std::string key, EncINode node) {
        return table.change(key, node);
    }

    int remove(std::string key) {
        return table.remove(key);
    }

    int clear(std::string key);

    int clearAll();

    int removeDir(std::string key) {
        return table.removeDir(key);
    }

public:
    std::string getRandFileName() {
        char filenameTemplate[] = "./filedata/data.XXXXXX";
        mkstemp(filenameTemplate);
        INFO("Template file name:" << std::string(filenameTemplate));
        return std::string(filenameTemplate);
    }

public:
    static std::string writeList(std::map<std::string, EncINode> *data) {
        std::ostringstream ostringstream;
        for (auto &i:(*data)) {
            if(i.second.encFileType==FILE_TYPE_FILE){
                ostringstream << i.second.encName << "*" << i.second.encFileType << "*"
                              << boost::filesystem::file_size(i.second.encCryptFileName) << "|";
            } else{
                ostringstream << i.second.encName << "*" << i.second.encFileType << "*"
                              << "unknow" << "|";
            }

        }
        return ostringstream.str();
    }

    static std::string writeList_yaml(std::map<std::string, EncINode> *data) {
        YAML::Node root;
        std::ostringstream oStr;
        for (auto &i:(*data)) {
            YAML::Node tmp;
            i.second>>tmp;
            root.push_back(tmp);
        }
        oStr << root;

        return oStr.str();
    }

    static std::vector<EncINode> *readFromList_yaml(std::string data, std::vector<EncINode> *res) {
        YAML::Node root = YAML::Load(data);
        for (const auto &n:root) {
            EncINode tmp;
            tmp << n;
            res->push_back(tmp);
        }
        return res;
    }
};

#endif //PERSONAL_FILE_PROTECTOR_MAIN_FILE_CONTROLLER_H
