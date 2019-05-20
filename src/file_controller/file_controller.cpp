//
// Created by tao on 19-3-25.
//

#include <algorithm>
#include "file_controller.h"
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>

std::mutex EncITable::mLock{};

void EncINode::operator<<(const YAML::Node &node) {
    this->encFileType = emFileType(node["encFileType"].as<int>());
    this->encName = node["encName"].as<std::string>();
    this->encMode = node["encMode"].as<int>();
    this->encCryptFileName = node["encCryptFileName"].as<std::string>();
    this->encFileKey = node["encFileKey"].as<int>();
    this->accessBit = node["accessBit"].as<int>();
}

void EncINode::operator>>(YAML::Node &node) const {
    int type = this->encFileType;
    node["encFileType"] = type;
    node["encName"] = this->encName;
    node["encMode"] = this->encMode;
    node["encCryptFileName"] = this->encCryptFileName;
    node["encFileKey"] = this->encFileKey;
    node["accessBit"] = this->accessBit;
}

bool EncITable::insert(EncINode node) {
    LockGround lockGround(&mLock);
    return tab.insert(std::make_pair(node.encName, node)).second;
}

EncINode *EncITable::selectByName(std::string name) {
    auto it = tab.find(name);
    if (it == tab.end()) {
        return nullptr;
    }
    return &(*it).second;
}

std::vector<EncINode *> EncITable::selectDirByName(std::string name) {
    std::vector<EncINode *> res;
    int sp_count = std::count(name.begin(), name.end(), '/');

    for (auto &n:tab) {
        if ((n.first.find(name) == 0)) {
            int sub_count = std::count(n.first.begin(), n.first.end(), '/');
            if ((sub_count - sp_count) <= 1) {
                res.push_back(&n.second);
            }
        }
    }
    return res;
}

std::map<std::string, EncINode> *
EncITable::selectFileWithDirByName(std::string name, std::map<std::string, EncINode> *result) {
    if (result == nullptr) {
        ERROR_LOG("result is null!")
        return nullptr;
    }
    if (name.empty()) {
        ERROR_LOG("name is null")
        return nullptr;
    }
    long sp_count = std::count(name.begin(), name.end(), '/');

    for (auto &n:tab) {
        if ((n.first.find(name) == 0)) {
            long sub_count = std::count(n.first.begin(), n.first.end(), '/');
            if ((sub_count - sp_count) == 0) {
                //文件
                result->insert(n);
            } else {
                //文件夹
                long dirNameLen = n.first.find('/', sp_count);
                std::string dirName = n.first.substr(0, dirNameLen+1);
                EncINode tmp;
                tmp.encName = dirName;
                tmp.encFileType = FILE_TYPE_DIR;
                result->insert(std::make_pair(dirName, tmp));
            }

        }
    }
    return result;
}

void EncITable::operator<<(const YAML::Node &node) {
    for (auto n:node) {
        EncINode iNode;
        iNode << n;
        tab.insert(std::make_pair(iNode.encName, iNode));
    }
}

void EncITable::operator>>(YAML::Node &node) const {
    for (const auto &n:tab) {
        YAML::Node temp;
        n.second >> temp;
        node.push_back(temp);
    }
}

int EncITable::change(std::string key, EncINode node) {
    LockGround lockGround(&mLock);

    //file
    if (node.encFileType == FILE_TYPE_FILE) {
        auto origin = selectByName(key);
        if (origin->encCryptFileName != node.encCryptFileName) {
            boost::filesystem::remove(origin->encCryptFileName);
        }
        if (origin == nullptr) {
            return -1;
        }

        if (tab.erase(key) != 1) {
            return -1;
        }
        if (insert(node)) {
            return -1;
        }
    }
    //dir
    if (node.encFileType == FILE_TYPE_DIR) {
        auto dir = selectDirByName(key);
        if (dir.size() <= 0) {
            return -1;
        }
        for (auto n:dir) {
            n->encName.replace(0, node.encName.length(), node.encName);
        }
    }
    return 0;
}

int EncITable::remove(std::string key) {
    LockGround lockGround(&mLock);

    return tab.erase(key);
}

std::vector<EncINode *> EncITable::selectAllDirByName(std::string name) {
    std::vector<EncINode *> res;
    for (auto &n:tab) {
        if ((n.first.find(name) == 0)) {
            res.push_back(&n.second);
        }
    }
    return res;
}

int EncITable::removeDir(std::string key) {
    LockGround lockGround(&mLock);

    auto dir = selectAllDirByName(key);
    if (dir.size() <= 0) {
        return -1;
    }
    for (auto n:dir) {
        remove(n->encName);
    }
    return 0;
}




