//
// Created by tao on 19-3-25.
//

#ifndef PERSONAL_FILE_PROTECTOR_FILE_CONTROLLER_H
#define PERSONAL_FILE_PROTECTOR_FILE_CONTROLLER_H

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "common_classes.h"
#include <mutex>
#include "common_includes.h"


enum emFileEncType {
    FILE_ENC_DES_CBC = 1,
    FILE_ENC_AES_CBC = 2,


};
enum emFileType {
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DIR = 2,
};


class EncINode {
public:
    emFileType encFileType{};
    std::string encName{};
    int encMode{};

    std::string encCryptFileName{};
    int encFileKey{};

    int accessBit{};
public:
    EncINode() = default;

    EncINode(const YAML::Node &node) {
        (*this) << node;
    }

    void copy(EncINode &origin) {
        encFileType = origin.encFileType;
        encName = origin.encName;
        encMode = origin.encMode;


        encCryptFileName = origin.encCryptFileName;
        encFileKey = origin.encFileKey;

        accessBit = origin.accessBit;
    }
public:
    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;
};

class EncITable {
public:
    std::map<std::string, EncINode> tab{};
    static std::mutex mLock;
public:
    bool insert(EncINode node);

    EncINode *selectByName(std::string name);

    std::vector<EncINode *> selectAllDirByName(std::string name);

    std::vector<EncINode *> selectDirByName(std::string name);

    std::map<std::string, EncINode> *selectFileWithDirByName(std::string name, std::map<std::string, EncINode> *result);

    int change(std::string key, EncINode node);

    int remove(std::string key);

    int removeDir(std::string key);

public:
    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;

};

#endif //PERSONAL_FILE_PROTECTOR_FILE_CONTROLLER_H
