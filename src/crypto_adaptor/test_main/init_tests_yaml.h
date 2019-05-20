/**************************************************
 * @FileName: init_tests_yaml.h
 * @Author: zhangtao
 * @Description:
 * @Version: 1.0
 * @History:
 * <author>      <time>         <version>   <desc>
 * zhangtao   19-3-17-上午12:32  1.0         create
**************************************************/


#ifndef PERSONAL_FILE_PROTECTOR_INIT_TESTS_YAML_H
#define PERSONAL_FILE_PROTECTOR_INIT_TESTS_YAML_H

#include <string>
#include <yaml-cpp/yaml.h>

class TestNode {
public:
    std::string testGroup{};
    std::string testName{};
    int platform{};
    int workMode{};
    int cryptType{};
    std::string source{};       //文本,文件名,数据块的映射名或base64编码
    std::string rightResult{};     //标准的文本,文件名,数据块的映射名或base64编码
public:

    TestNode() = default;

    TestNode(std::string &&_testGroup, std::string &&_testName, int _platform, int _workMode, int _cryptType,
             std::string &&_source, std::string &&_rightResult) :
            testGroup(_testGroup), testName(_testName),
            platform(_platform), workMode(_workMode), cryptType(_cryptType), source(_source),
            rightResult(_rightResult) {}

    TestNode(std::string &_testGroup, std::string &_testName, int _platform, int _workMode, int _cryptType,
             std::string &_source, std::string &_rightResult) :
            testGroup(_testGroup), testName(_testName),
            platform(_platform), workMode(_workMode), cryptType(_cryptType), source(_source),
            rightResult(_rightResult) {}

    TestNode(const YAML::Node &node);

    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;
};

class TestNodeTab {
public:
    TestNodeTab() = default;

    std::string name{"hash"};
    std::vector<TestNode> nodes{};
public:
    void operator<<(const YAML::Node &node);

    void operator>>(YAML::Node &node) const;
};

int load_test();
#endif //PERSONAL_FILE_PROTECTOR_INIT_TESTS_YAML_H
