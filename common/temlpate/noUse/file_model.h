//
// Created by tao on 19-3-25.
//

#ifndef PERSONAL_FILE_PROTECTOR_FILE_MODEL_H
#define PERSONAL_FILE_PROTECTOR_FILE_MODEL_H

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

enum emFileEncType {
    FILE_ENC_DES_CBC = 1,

};
enum emFileType {
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DIR = 2,
};

class EncDir;

class EncFile;

class EncModel {
public:
    emFileType encFileType{};
    EncModel *encParent{};
    std::string encName{};
    int encMode{};
public:
    EncModel() = default;

    EncModel(const YAML::Node &node) {
        (*this) << node;
    }

public:
    virtual void operator<<(const YAML::Node &node);

    virtual void operator>>(YAML::Node &node) const;
};

class EncFile : public EncModel {
public:
    std::string encFileName{};
    int encFileKey{};
public:
    void operator<<(const YAML::Node &node) override;

    void operator>>(YAML::Node &node) const override;

public:
    EncFile() = default;

    EncFile(const YAML::Node &node) {
        (*this) << node;
    }
};

class EncDir : public EncModel {
public:
    std::map<std::string, EncModel> encChildren;
public:
    int insert(EncModel &file);

    int deleteFileOrDir(const std::string &name);

    int change(const std::string &name, const EncModel &file);

    EncModel *select(const std::string &name);

    bool exists(const std::string &name);


public:
    void operator<<(const YAML::Node &node) override;

    void operator>>(YAML::Node &node) const override;

public:
    EncDir() = default;

    EncDir(const YAML::Node &n) {
        (*this) << n;
    }
};

#endif //PERSONAL_FILE_PROTECTOR_FILE_MODEL_H
