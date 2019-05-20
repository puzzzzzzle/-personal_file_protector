#ifndef FILELIST_H
#define FILELIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QQmlExtensionPlugin>
#include <QtQml>

class  AbstractList   //抽象数据列表类
{
public:
    AbstractList(QObject *parent = nullptr);
    AbstractList(const QString &name,const QString &type,const QString &showName, QString &size,QObject *parent=nullptr);
    AbstractList(const QString &&name,const QString &&type,const QString &&showName, QString &&size,QObject *parent=nullptr);
    QString name()const;
    void setName(const QString &name);
    QString type()const;
    void  setType(const QString &type);
    QString showName()const;
    void setShowName(const QString &showName);
    QString size()const;
    void setSize(const QString &size);
private:
    QString m_name;
    QString m_type;
    QString m_showName;
    QString m_size;

};
class AbstractListModel : public  QAbstractListModel
{
    Q_OBJECT
public:
    enum AbstractListRoles{
        NameRole=Qt::UserRole+1,
        TypeRole,
        ShowNameRole,
        SizeRole,
    };                               //定义抽象类成员角色
    AbstractListModel(QObject *parent=nullptr);
    void addList(const AbstractList &list);
    int rowCount(const QModelIndex &parent=QModelIndex()) const;  //返回给定父项行数
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;//返回索引所在项给定角色的存储数据
protected:
    QHash<int,QByteArray> roleNames() const;  //返回模型的角色名
public:
    QList<AbstractList> m_abstractList;     //抽象列表类容器
};
//class QExampleQmlPlugin : public QQmlExtensionPlugin {
//  Q_OBJECT
//  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtension.AbstractListModel")

//  public:
//  void registerTypes(char const * uri) {
//    qmlRegisterType<AbstractListModel>(uri, 1, 0, "AbstractListModel");
//  }
//};

#endif // FILELIST_H
