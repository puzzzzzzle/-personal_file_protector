//
// Created by tao on 19-3-25.
//

#ifndef PERSONAL_FILE_PROTECTOR_FILE_KEY_CONTROLLER_H
#define PERSONAL_FILE_PROTECTOR_FILE_KEY_CONTROLLER_H

#include <string>
#include <yaml-cpp/yaml.h>
#include <map>
#include <mutex>

class FileKeyNode {
public:
    FileKeyNode() = default;
    FileKeyNode(int _id, std::string _key) {
        this->id = _id;
        this->key = _key;
    }

public:
    int id{};
    std::string key{};

public:
    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;

public:
    void clear(){
        this->id=-1;
        this->key="";
    }
};

class FileKeyTable {
public:
    static FileKeyTable *Instance;
    static std::mutex lock;

public:
    std::map<int, std::string> tab{};
    int userID{-1};
public:
    std::string getKey(int id);

    int insertKey(const FileKeyNode &node);

    int insertKey(const std::string &key);

    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;

    int loadKeyTab();

    int savrKeyTab();

public:
    std::string toYamlString(){
        std::stringstream stream;
        YAML::Node root;
        *this>>root;
        stream<<root;
        return stream.str();
    }
};

std::string get_file_key(int user, int para);

#endif //PERSONAL_FILE_PROTECTOR_FILE_KEY_CONTROLLER_H
