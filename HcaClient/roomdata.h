#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QObject>

class RoomData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
public:
    explicit RoomData(QObject *parent = 0);
    RoomData(const QString &name, const QString &description, int size, QObject *parent = 0);

    QString name();
    void setName(const QString &name);

    QString description();
    void setDescription(const QString &description);

    int size();
    void setSize(int size);

signals:
    void nameChanged();
    void descriptionChanged();
    void sizeChanged();

private:
    QString m_name;
    QString m_description;
    int m_size; //qty of clients
};

#endif // ROOMDATA_H
