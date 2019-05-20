#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include<QDebug>
#include "filelist.h"
#include "pip_common_win.hpp"
#include "protocol_adaptor.hpp"
#include "work_mode.h"
#include <iostream>
#include <fstream>
#include <direct.h>

#define PIP_FILE "../../cmake-build-debug/Bin/sysdata/pipe.data"

enum OPERATIONS {
    NONE = 0,
    REGISTER,
    LOGIN,
    LS,
    MV,
    CP,
    RM,
    ADD,
    DEC = 8,
    ADD_KEY = 9,
    DEL_KEY = 10,
    SYNC = 11,
};


extern AbstractListModel *dataList;
class Controller : public QObject
{
    Q_OBJECT
public:
    PipAdaptor pipe;

    std::string pipe_file;
    QString pwd;
public:
    int init(){
        char buff[2048]{};
        _getcwd(buff,sizeof (buff));
        qDebug()<<"_getcwd is "<<QString(buff);

        int  iRet=0;

        std::ifstream in_pipe;
        in_pipe.open(PIP_FILE);
        if(!in_pipe){
            qDebug()<<"error in open file";
            return -1;
        }
        in_pipe>>pipe_file;
        qDebug()<<"pip file is "<<QString(pipe_file.c_str());
        iRet = pipe.pip_creat_cli(pipe_file.c_str());
        if(iRet){
            qDebug()<<"pip_creat_cli is "<<iRet;
//            return iRet;
        }
        return 0;
    }

    //gui call
public:
    Q_INVOKABLE void cppMethod(const QString &msg) {
        qDebug() << "Called the C++ method with" << msg;
    }

    Q_INVOKABLE QString getpwd(){
        return pwd;
    }

    Q_INVOKABLE int login(const QString &name,const QString &passwd) {
        qDebug() << "login" << name<<passwd;
        CommonReq req;
        CommonRsp rsp;
        req.cmd="login";
        req.para1=name.toStdString();
        req.para2=passwd.toStdString();
        pipe.pip_write(req.enc());
        std::string rspStr= pipe.pip_read_single();
        rsp.dec(rspStr);
        qDebug() <<rsp.result;
        if(rsp.result==0){
            pwd="/";
            ls(pwd);
            return 0;
        }
        return -1;

    }

    Q_INVOKABLE int registerUser(const QString &name,const QString &passwd) {
        qDebug() << "registerUser" << name<<passwd;
        CommonReq req;
        CommonRsp rsp;
        req.cmd="register";
        req.para1=name.toStdString();
        req.para2=passwd.toStdString();
        pipe.pip_write(req.enc());
        std::string rspStr= pipe.pip_read_single();
        rsp.dec(rspStr);
        qDebug() <<rsp.result;
        if(rsp.result>0){
            return 0;
        }
        return -1;
    }
    Q_INVOKABLE int ls(const QString &path){
        CommonReq req;
        CommonRsp rsp;
        req.cmd="ls";
        req.para1=path.toStdString();
        pipe.pip_write(req.enc());
        std::string rspStr= pipe.pip_read_single();
        rsp.dec(rspStr);
        if(rsp.result==0){
            if(rsp.data=="none"){
                dataList->m_abstractList.clear();
                emit dataList->dataChanged(dataList->index(0),dataList->index(dataList->rowCount()));
                return 0;
            }else {
                auto list = req.split(rsp.data,"|");
                for(const auto &i:list){
                    auto file = req.split(i,"*");
                    QString name = file[0].c_str();
                    QString type = file[1]=="1"?"文件":"文件夹";
                    QString size = file[2].c_str();
                    auto paths = req.split(file[0],"/");
                    QString showName = paths.at(paths.size()-1).c_str();
                    dataList->addList(AbstractList(name,type,showName,size));
                    emit dataList->dataChanged(dataList->index(0),dataList->index(dataList->rowCount()));
                }
                return 0;
            }
        }
        return -1;
    }


public slots:
    void cppSlot(int number) {
        //        dataList->addList(AbstractList("/fff/000.jpg","文件","000.jpg","16546541"));
        //        emit dataList->dataChanged(dataList->index(0),dataList->index(dataList->rowCount()));
        qDebug() << "Called the C++ slot with" << number;
    }
    void reload(){

    }
};


#endif // CONTROLLER_H
