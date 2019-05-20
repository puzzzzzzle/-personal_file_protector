/**************************************************
 * @FileName: init_tests_yaml.cpp
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/


#include "init_tests_yaml.h"
#include <iostream>
#include <fstream>
#include "log_init.h"

TestNodeTab tab;
const std::string cfgFile{"configure/crypt_tests.yaml"};

void TestNode::operator<<(const YAML::Node &node) {
    this->testGroup = node["testGroup"].as<std::string>();
    this->testName = node["testName"].as<std::string>();
    this->platform = node["platform"].as<int>();
    this->workMode = node["workMode"].as<int>();
    this->cryptType = node["cryptType"].as<int>();
    this->source = node["source"].as<std::string>();
    this->rightResult = node["rightResult"].as<std::string>();
}

void TestNode::operator>>(YAML::Node &node) const {
    node["testGroup"] = this->testGroup;
    node["testName"] = this->testName;
    node["platform"] = this->platform;
    node["workMode"] = this->workMode;
    node["cryptType"] = this->cryptType;
    node["source"] = this->source;
    node["rightResult"] = this->rightResult;
}

TestNode::TestNode(const YAML::Node &node) {
    *this << node;
}

void TestNodeTab::operator<<(const YAML::Node &node) {
    this->name = node["name"].as<std::string>();
    YAML::Node temp = node["nodes"];
    for (const auto &n : temp) {
        this->nodes.emplace_back(n);
    }
}

void TestNodeTab::operator>>(YAML::Node &node) const {
    node["name"] = this->name;
    for (const auto &n:this->nodes) {
        YAML::Node tmp;
        n >> tmp;
        node["nodes"].push_back(tmp);
    }
}

int load_test() {
    std::ifstream infile(cfgFile, std::ios::in);
    if (!infile) {
        ERROR_LOG("file don't exists :" << cfgFile)
        return -1;
    }
    YAML::Node root = YAML::Load(infile);
    tab << root;
    return 0;
}
