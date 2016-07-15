#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    QString name;
    QWebSocket *socket;

signals:

public slots:
    //void setSocket(QWebSocket *sck){this->socket = sck;}

private:

};

#endif // CLIENT_H
