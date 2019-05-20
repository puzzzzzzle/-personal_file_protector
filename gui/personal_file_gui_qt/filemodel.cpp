#include "filemodel.h"

#include "filemodel.h"

DataObject::DataObject(QObject *parent) : QObject(parent)
{

}

DataObject::DataObject(const QString &name,const QString &type,const QString &showName, QString &size,QObject *parent)
    :m_name(name),m_type(type),m_showName(showName),m_size(size){

}
DataObject::DataObject(const QString &&name,const QString &&type,const QString &&showName, QString &&size,QObject *parent)
:m_name(name),m_type(type),m_showName(showName),m_size(size){

}
QString DataObject::name()const{
    return m_name;
}
void DataObject::setName(const QString &name){
    m_name =name;
}
QString DataObject::type()const{
    return m_type;
}
void  DataObject::setType(const QString &type){
    m_type=type;
}
QString DataObject::showName()const{
    return m_showName;
}
void DataObject::setShowName(const QString &showName){
    m_showName=showName;
}
QString DataObject::size()const{
    return m_size;
}
void DataObject::setSize(const QString &size){
    m_size=size;
}

//void DataObject::nameChanged(){

//}
//void DataObject::typeChanged(){

//}
//void DataObject::showNameChanged(){

//}
//void DataObject::sizeChanged(){

//}

//void DataObject::dataChanged(){

//}
