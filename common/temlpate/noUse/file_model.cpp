//
// Created by tao on 19-3-25.
//

#include "file_model.h"

void EncModel::operator<<(const YAML::Node &node) {
    this->encFileType = emFileType(node["encFileType"].as<int>());
    this->encName = node["encName"].as<std::string>();
    this->encMode = node["encMode"].as<int>();

}

void EncModel::operator>>(YAML::Node &node) const {
    int type = this->encFileType;
    node["encFileType"] = type;
    node["encName"] = this->encName;
    node["encMode"] = this->encMode;
}

void EncFile::operator<<(const YAML::Node &node) {
    EncModel::operator<<(node);
    this->encFileName = node["encFileName"].as<std::string>();
    this->encFileKey = node["encFileKey"].as<int>();
}

void EncFile::operator>>(YAML::Node &node) const {
    EncModel::operator>>(node);
    node["encFileName"] = this->encFileName;
    node["encFileKey"] = this->encFileKey;
}

void EncDir::operator<<(const YAML::Node &node) {
    EncModel::operator<<(node);
    YAML::Node temp = node["encChildren"];
    for (const auto &n : temp) {
        EncModel model(n);
        model.encParent = this;
        this->encChildren.insert(std::make_pair(n["encName"].as<std::string>(), model));
    }
}

void EncDir::operator>>(YAML::Node &node) const {
    EncModel::operator>>(node);
    for (const std::pair<std::string, EncModel> &n:this->encChildren) {
        YAML::Node tmp;
        const EncModel *model = &(n.second);

        if (n.second.encFileType == FILE_TYPE_FILE) {
            auto *file = (const EncFile *) model;
            (*file) >> tmp;
        }
        if (n.second.encFileType == FILE_TYPE_DIR) {
            auto dir = (const EncDir *) model;
            (*dir) >> tmp;
        }
        node["encChildren"].push_back(tmp);
    }
}

bool EncDir::exists(const std::string &name) {
    return this->encChildren.find(name) != this->encChildren.end();
}

int EncDir::insert(EncModel &file) {
    file.encParent = this;
    return this->encChildren.insert(std::make_pair(file.encName, file)).second;
}

int EncDir::deleteFileOrDir(const std::string &name) {
    return this->encChildren.erase(name) == 1;
}

int EncDir::change(const std::string &name, const EncModel &file) {
    EncModel *p = select(name);
    if (p == nullptr) {
        return -1;
    }
    if (p->encMode != file.encMode) {
        return -2;
    }
    (*p) = file;
    return 0;
}

EncModel *EncDir::select(const std::string &name) {
    auto p = this->encChildren.find(name);
    if (p == this->encChildren.end()) {
        return nullptr;
    } else {
        return &((*p).second);
    }
}
