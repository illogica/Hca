#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QUuid>
#include <QJsonObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    QJsonObject toFullJsonObject();
    QJsonObject toLightJsonObject();

    QString uuid() const;
    void setUuid(const QString &uuid);

    QString name() const;
    void setName(const QString &name);

    QWebSocket *socket() const;
    void setSocket(QWebSocket *socket);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    int id() const;
    void setId(int id);

    int status() const;
    void setStatus(int status);

    int privilege() const;
    void setPrivilege(int privilege);

    QString description() const;
    void setDescription(const QString &description);

signals:
    void queueTextMessage(const QString &message);

public slots:
    void sendTextMessage(const QString &message);

private:
    int m_id;
    int m_status;
    int m_privilege;
    QString m_uuid;
    QString m_name;
    QString m_avatar;
    QString m_description;

    QWebSocket *m_socket;

};

#endif // CLIENT_H
