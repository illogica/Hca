#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    quint32 id;
    QString name;
    QWebSocket *socket;

    static quint32 idCounter;

signals:

public slots:
    //void setSocket(QWebSocket *sck){this->socket = sck;}

private:

};

#endif // CLIENT_H
