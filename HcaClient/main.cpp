#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hcaclient.h"
#include "roomdatamodel.h"
#include "chatsdatamodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<RoomDataModel>("Local", 1, 0, "RoomDataModel");
    qmlRegisterType<ChatsDataModel>("Local", 1, 0, "ChatsDataModel");

    QCoreApplication::setOrganizationName("Illogica");
    QCoreApplication::setOrganizationDomain("Illogicasoftware.com");
    QCoreApplication::setApplicationName("Hca Client");

    QQmlApplicationEngine engine;
    HcaClient *client = new HcaClient(0, engine.rootContext());
    engine.rootContext()->setContextProperty("HcaClient", client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //Compiles a list of emojys
    QStringList emojyList;
#include "emojys.h"
    engine.rootContext()->setContextProperty("emojyList", QVariant::fromValue(emojyList));

    return app.exec();
}
