#include "filelist.h"

AbstractList::AbstractList(QObject *parent)
{

}

AbstractList::AbstractList(const QString &name,const QString &type,const QString &showName, QString &size,QObject *parent)
    :m_name(name),m_type(type),m_showName(showName),m_size(size){

}
AbstractList::AbstractList(const QString &&name,const QString &&type,const QString &&showName, QString &&size,QObject *parent)
:m_name(name),m_type(type),m_showName(showName),m_size(size){

}
QString AbstractList::name()const{
    return m_name;
}
void AbstractList::setName(const QString &name){
    m_name =name;
}
QString AbstractList::type()const{
    return m_type;
}
void  AbstractList::setType(const QString &type){
    m_type=type;
}
QString AbstractList::showName()const{
    return m_showName;
}
void AbstractList::setShowName(const QString &showName){
    m_showName=showName;
}
QString AbstractList::size()const{
    return m_size;
}
void AbstractList::setSize(const QString &size){
    m_size=size;
}


AbstractListModel::AbstractListModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void AbstractListModel::addList(const AbstractList &list)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_abstractList.append(list);
    endInsertRows();
}

int AbstractListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_abstractList.count();
}

QVariant AbstractListModel::data(const QModelIndex &index, int role) const
{
  if(index.row()<0||index.row()>=m_abstractList.count())
      return QVariant();
  const AbstractList &abstractList=m_abstractList[index.row()];
  if(role==AbstractListRoles::NameRole)
      return abstractList.name();
  else if(role==AbstractListRoles::TypeRole)
      return abstractList.type();
  else if(role==AbstractListRoles::ShowNameRole)
          return abstractList.showName();
  else if(role==AbstractListRoles::SizeRole)
          return abstractList.size();
  return QVariant();
}
QHash<int, QByteArray> AbstractListModel::roleNames() const
{
    QHash<int,QByteArray> roles;
    //use operator[]
//    roles[AbstractListRoles::NameRole]="name";            //定义对应角色值
//    roles[AbstractListRoles::ColorRole]="color1";
//    roles[AbstractListRoles::NumberRole]="number";
    //use insert
    roles.insert(AbstractListRoles::NameRole,"name");
    roles.insert(AbstractListRoles::TypeRole,"type");
    roles.insert(AbstractListRoles::ShowNameRole,"showName");
    roles.insert(AbstractListRoles::SizeRole,"size");
    return roles;
}

