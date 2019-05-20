#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "pip_common_win.hpp"
#include "protocol_adaptor.hpp"
#include "controller.h"
#include <QQmlContext>
#include "filelist.h"

extern AbstractListModel *dataList;    //QObject model
Q_DECLARE_METATYPE( AbstractListModel* )

int main(int argc, char *argv[])
{
    qmlRegisterType<AbstractListModel>("filelist", 1, 0, "AbstractListModel");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Controller controller;
    if(controller.init()){

        return -1;
    }
    engine.rootContext()->setContextProperty("ctl",&controller);

    engine.rootContext()->setContextProperty("mdata",QVariant::fromValue(dataList));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
