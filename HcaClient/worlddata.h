#ifndef WORLDDATA_H
#define WORLDDATA_H

#include <QObject>

class WorldData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)
public:
    explicit WorldData(QObject *parent = 0);

signals:
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &name);
    void sizeChanged(int size);

public slots:
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setSize(int size);
    QString name();
    QString description();
    int size();

private:
    QString m_name;
    QString m_description;
    int m_size; //qty of rooms
};

#endif // WORLDDATA_H
