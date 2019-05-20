//
// Created by tao on 19-3-25.
//

#ifndef PERSONAL_FILE_PROTECTOR_GET_USR_PASSWD_H
#define PERSONAL_FILE_PROTECTOR_GET_USR_PASSWD_H

#include <string>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "common_classes.h"
#include <mutex>


#define USER_KEY_INDEX_ROW 30
#define USER_KEY_INDEX_LINE 2

class UserKeyNode{
public:
    int userId{};
    std::string userName{};
    std::string userPasswdHash{};

    int tableIndex[USER_KEY_INDEX_ROW][USER_KEY_INDEX_LINE];
    unsigned char sortHave[USER_KEY_INDEX_ROW]{};
    unsigned int mask{};

public:
    UserKeyNode() = default;

    UserKeyNode(const YAML::Node &node) {
        (*this) << node;
    }

public:
    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;

public:
    std::string getFinalUserKay()const;
};

class UserKeyTable {
public:
    static UserKeyTable *Instance;
    static std::mutex lock;

public:
    std::map<std::string, UserKeyNode> tab{};
    int isLoad{0};
public:
    std::string getKey(int id)  const;

    bool insertKey(const UserKeyNode &node);

    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;

    int loadKeyTab();

    int savrKeyTab();

    int checkLogin(std::string name,std::string passed);

    int registor(std::string &name, std::string &passwd);

    int randKey();
public:
    std::string toYamlString(){
        std::stringstream stream;
        YAML::Node root;
        *this>>root;
        stream<<root;
        return stream.str();
    }
};

std::string get_usr_key(int para);


#endif //PERSONAL_FILE_PROTECTOR_GET_USR_PASSWD_H
