#ifndef HCATHREAD_H
#define HCATHREAD_H

#include <QThread>
#include <QtSql>

class HcaThread : public QThread
{
public:
    HcaThread(const QString &name);

private:
    QSqlDatabase db;

};

#endif // HCATHREAD_H
