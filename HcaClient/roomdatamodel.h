#ifndef ROOMDATAMODEL_H
#define ROOMDATAMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QtQml/qqml.h>
#include "roomdata.h"

class RoomDataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit RoomDataModel(QObject *parent=0);

    enum{
        IdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        MotdRole,
        SizeRole,
        AvatarRole
    };

    //From QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

protected:
    QHash<int, QByteArray> roleNames() const;

public slots:
    void addRoom(int id, const QString &name, const QString &description, const QString &motd, int size, const QString &avatar);
    void reset();

private:
    QList<RoomData*> roomsList;
};

QML_DECLARE_TYPE(RoomDataModel)

#endif // ROOMDATAMODEL_H
