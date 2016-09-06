#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QObject>

class RoomData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
    Q_PROPERTY(QString motd READ motd WRITE setMotd NOTIFY motdChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
public:
    explicit RoomData(QObject *parent = 0);
    RoomData(const QString &name, const QString &description, int size, QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    int size() const;
    void setSize(int size);

    int id() const;
    void setId(int id);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    QString motd() const;
    void setMotd(const QString &motd);

signals:
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &description);
    void sizeChanged(int size);
    void idChanged(int id);
    void avatarChanged(const QString &avatar);
    void motdChanged(const QString &motd);

private:
    QString m_name;
    QString m_description;
    QString m_avatar;
    QString m_motd;
    int m_id;
    int m_size; //qty of clients
};

#endif // ROOMDATA_H
