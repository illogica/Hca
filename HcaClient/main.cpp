#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hcaclient.h"
#include "roomdatamodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<RoomDataModel>("Local", 1, 0, "RoomDataModel");

    QCoreApplication::setOrganizationName("Illogica");
    QCoreApplication::setOrganizationDomain("Illogicasoftware.com");
    QCoreApplication::setApplicationName("Hca Client");

    QQmlApplicationEngine engine;
    HcaClient *client = new HcaClient(0, engine.rootContext());
    engine.rootContext()->setContextProperty("HcaClient", client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QStringList emojyList;
    //compilare la lista delle emojy
    engine.rootContext()->setContextProperty("emojyList", QVariant::fromValue(emojyList));

    return app.exec();
}
