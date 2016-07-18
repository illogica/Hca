#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QUuid>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    static quint32 idCounter;

    QUuid uuid() const;
    void setUuid(const QUuid &uuid);

    QString name() const;
    void setName(const QString &name);

    QWebSocket *socket() const;
    void setSocket(QWebSocket *socket);

signals:
    void queueTextMessage(const QString &message);

public slots:
    void sendTextMessage(const QString &message);

    //void setSocket(QWebSocket *sck){this->socket = sck;}

private:
    quint32 id;
    QUuid m_uuid;
    QString m_name;
    QWebSocket *m_socket;

};

#endif // CLIENT_H
