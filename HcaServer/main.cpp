#include <QCoreApplication>
#include "hcaserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    HcaServer *srv = new HcaServer();
    Q_UNUSED(srv);
    return a.exec();
}
