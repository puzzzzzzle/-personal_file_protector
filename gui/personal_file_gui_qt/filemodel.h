#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QObject>
class DataObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString showName READ showName WRITE setShowName NOTIFY showNameChanged)
    Q_PROPERTY(QString size READ size WRITE setSize NOTIFY sizeChanged)
//    Q_PROPERTY(QString name READ name WRITE setName )
//    Q_PROPERTY(QString type READ type WRITE setType )
//    Q_PROPERTY(QString showName READ showName WRITE setShowName)
//    Q_PROPERTY(QString size READ size WRITE setSize )
public:
    DataObject(QObject *parent = nullptr);
    DataObject(const QString &name,const QString &type,const QString &showName, QString &size,QObject *parent=nullptr);
    DataObject(const QString &&name,const QString &&type,const QString &&showName, QString &&size,QObject *parent=nullptr);
    QString name()const;
    void setName(const QString &name);
    QString type()const;
    void  setType(const QString &type);
    QString showName()const;
    void setShowName(const QString &showName);
    QString size()const;
    void setSize(const QString &size);
signals:
    void nameChanged();
    void typeChanged();
    void showNameChanged();
    void sizeChanged();

    void dataChanged();
private:
    QString m_name;
    QString m_type;
    QString m_showName;
    QString m_size;
};
#endif // FILEMODEL_H
