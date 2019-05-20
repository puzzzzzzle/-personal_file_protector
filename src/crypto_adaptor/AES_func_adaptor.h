//
// Created by tao on 19-3-25.
//

#ifndef PERSONAL_FILE_PROTECTOR_AES_FUNC_ADAPTOR_H
#define PERSONAL_FILE_PROTECTOR_AES_FUNC_ADAPTOR_H

#include <string>

std::string printBinToStr(const unsigned char *uc_pMd, int len);

std::string printBinToStr(const char *uc_pMd, int len);

int checkFile(std::string fileStr);

int trunkFile(std::string fileStr);


int aesDataToData(std::string keyStr, const char *inData, size_t inLen, char *outData, size_t &outlen,int type);


int aesFileToFile(std::string keyStr, std::string inFile,std::string outFile,int type,size_t &curr_size);

int aesFdToFd(std::string keyStr, int inFd,int outFd,int type,size_t &curr_size);

#endif //PERSONAL_FILE_PROTECTOR_AES_FUNC_ADAPTOR_H
