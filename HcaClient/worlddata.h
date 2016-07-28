#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <QObject>
#include <roomdata.h>

class WorldData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
public:
    WorldData(QObject *parent = 0);
    WorldData(int id, const QString &name, const QString &description, int size, QObject *parent = 0);

    void resetRooms();
    void addRoom(RoomData *r);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    int size() const;
    void setSize(int size);

    int id() const;
    void setId(int id);

    QList<QObject *> roomsModel() const;

signals:
    void nameChanged();
    void descriptionChanged();
    void sizeChanged();
    void idChanged();

private:
    QString m_name;
    QString m_description;
    int m_id;
    int m_size; //qty of rooms

    QList<RoomData *> m_rooms;
    QList<QObject *> m_roomsModel;

};

#endif // WORLDDATA_H
