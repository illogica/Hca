#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QObject>

class RoomData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString roomName READ roomName WRITE setRoomName NOTIFY roomNameChanged)
    Q_PROPERTY(int roomClients READ roomClients WRITE setRoomClients NOTIFY roomClientsChanged)
public:
    explicit RoomData(QObject *parent = 0);

signals:
    void roomNameChanged(const QString &name);
    void roomClientsChanged(int clients);

public slots:
    void setRoomName(const QString &name);
    void setRoomClients(int clients);
    QString roomName();
    int roomClients();

private:
    QString m_name;
    int m_clients;
};

#endif // ROOMDATA_H
