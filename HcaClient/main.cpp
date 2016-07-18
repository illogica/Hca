#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hcaclient.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Illogica");
    QCoreApplication::setOrganizationDomain("Illogicasoftware.com");
    QCoreApplication::setApplicationName("Hca Client");

    HcaClient *client = new HcaClient();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("HcaClient", client);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
