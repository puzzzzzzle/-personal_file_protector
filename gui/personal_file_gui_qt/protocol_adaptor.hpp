//
// Created by Administrator on 2019/4/22.
//

#ifndef PERSONAL_FILE_PROTECTOR_PROTOCOL_ADAPTOR_HPP
#define PERSONAL_FILE_PROTECTOR_PROTOCOL_ADAPTOR_HPP

#include <string>
#include "base64.h"
#include <vector>
#include <cstring>
#include <sstream>
#include <string>

#define PUBLIC_KEY "sfasdjfasldfjsakdfj"
#define SPLIE_CHAR "#"


class CommonReq {
public:
    std::string cmd;
    std::string para1;
    std::string para2;
    std::string para3;
    std::string otherYamlData;
public:
    CommonReq() {
        cmd="none";
        para1="none";
        para2="none";
        para3="none";
        otherYamlData="none";
    };

    CommonReq(std::string &_cmd, std::string &_para1, std::string &_para2, std::string &_para3, std::string &_other) :
        cmd(_cmd), para1(_para1), para2(_para2), para3(_para3), otherYamlData(_other) {

    }

    CommonReq(std::string &_cmd, std::string &_para1, std::string &_para2) :
        cmd(_cmd), para1(_para1), para2(_para2) {

    }

    CommonReq(std::string &_cmd, std::string &_para1, std::string &_para2, std::string &_para3) :
        cmd(_cmd), para1(_para1), para2(_para2), para3(_para3) {

    }

    std::vector<std::string> split(const std::string &str, const std::string &delim) {
        std::vector<std::string> res;
        if ("" == str) return res;
        //先将要切割的字符串从string类型转换为char*类型
        char *strs = new char[str.length() + 1]; //不要忘了
        strcpy(strs, str.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(strs, d);
        while (p) {
            std::string s = p; //分割得到的字符串转换为string类型
            res.push_back(s); //存入结果数组
            p = strtok(NULL, d);
        }

        return res;
    }

public:

    std::string enc() {
        return enc_plain();
    }

    int dec(std::string data) {
        return dec_plain(data);
    }
    std::string enc_plain() {
        std::ostringstream ostringstream;
        ostringstream << cmd << SPLIE_CHAR << para1 << SPLIE_CHAR << para2 << SPLIE_CHAR << para3;
        std::string enc = ostringstream.str();
        std::string encoded = base64_encode(reinterpret_cast<const unsigned char *>(enc.c_str()), enc.length());
        return encoded;
    }

    int dec_plain(std::string data) {
        //base64
        std::string decoded = base64_decode(data);
        std::vector<std::string> all = split(decoded, SPLIE_CHAR);
        if (all.size() != 4) {
            return -1;
        }
        cmd = all[0];
        para1 = all[1];
        para2 = all[2];
        para3 = all[3];
        return 0;
    }

};

class CommonRsp {
public:
    int result;
    std::string data;
public:
    CommonRsp() {
        result=-1;
        data="none";
    }
    CommonRsp(int _result, std::string &_data) :
        result(_result), data(_data) {

    }

    std::vector<std::string> split(const std::string &str, const std::string &delim) {
        std::vector<std::string> res;
        if ("" == str) return res;
        //先将要切割的字符串从string类型转换为char*类型
        char *strs = new char[str.length() + 1]; //不要忘了
        strcpy(strs, str.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(strs, d);
        while (p) {
            std::string s = p; //分割得到的字符串转换为string类型
            res.push_back(s); //存入结果数组
            p = strtok(NULL, d);
        }

        return res;
    }

public:
    std::string enc() {
        return enc_plain();
    }

    int dec(std::string data) {
        return dec_plain(data);
    }

    std::string enc_plain() {
        std::ostringstream ostringstream;
        ostringstream << result << SPLIE_CHAR << data;
        std::string enc = ostringstream.str();
        std::string encoded = base64_encode(reinterpret_cast<const unsigned char *>(enc.c_str()), enc.length());
        return encoded;
    }

    int dec_plain(std::string data) {
        //base64
        std::string decoded = base64_decode(data);
        std::vector<std::string> all = split(decoded, SPLIE_CHAR);
        if (all.size() != 2) {
            return -1;
        }
        this->result = atoi(all[0].c_str());
        this->data = all[1];
        return 0;
    }
};

#endif //PERSONAL_FILE_PROTECTOR_PROTOCOL_ADAPTOR_HPP
