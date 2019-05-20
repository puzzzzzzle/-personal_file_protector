//
// Created by Administrator on 2019/4/18.
//
#ifdef __CYGWIN__
#include "main_win.hpp"
#else
#include "main_unix.hpp"
#endif

int base64Test(){

    std::string iData="hhhhhhhhhhhhhhhhhhhhhh";
    //base64 enc
    Base64Adaptor adaptor(iData.c_str(),iData.length());
    char *base64Buff =(char *) malloc(iData.length()*3);
    size_t base64OutLen=0;
    if(adaptor.encodeToData(iData.length()*3,base64Buff,base64OutLen)->isWrong()){
        ERROR_LOG("base64 is wrong")
        free(base64Buff);
        return -1;
    }
    base64Buff[base64OutLen]='\0';
    std::string iResult(base64Buff);

    //base64 decode
    Base64Adaptor decadaptor(base64Buff,base64OutLen);
    char *decbase64Buff =(char *) malloc(base64OutLen*3);
    size_t decbase64OutLen=0;
    if(decadaptor.decodeToData(iData.size()*3,decbase64Buff,decbase64OutLen)->isWrong()){
        ERROR_LOG("base64 is wrong")
        free(decbase64Buff);
        return -1005;
    }
    decbase64Buff[decbase64OutLen]='\0';
    std::string iResult1(decbase64Buff);

    return 0;
}
int main(int argc,char *argv[]){
    srand(time(NULL));
    base64Test();
    main_diy(argc,argv);

}