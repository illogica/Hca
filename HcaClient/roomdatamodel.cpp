#include "roomdatamodel.h"
#include <QDebug>

RoomDataModel::RoomDataModel(QObject *parent):QAbstractListModel(parent)
{
}

int RoomDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return roomsList.count();
}

QVariant RoomDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0)
        return QVariant();

    if (index.row() >= roomsList.count()) {
        qWarning() << "RoomDataModel: Index out of bounds";
        return QVariant();
    }

    RoomData *rd = roomsList.at(index.row());

    switch (role) {
    case IdRole:
        return rd->id();
    case NameRole:
        return rd->name();
    case DescriptionRole:
        return rd->description();
    case MotdRole:
        return rd->motd();
    case SizeRole:
        return rd->size();
    case AvatarRole:
        return rd->avatar();
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> RoomDataModel::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(IdRole, "roomId");
    roleNames.insert(NameRole, "roomName");
    roleNames.insert(DescriptionRole, "roomDescription");
    roleNames.insert(MotdRole, "roomMotd");
    roleNames.insert(SizeRole, "roomSize");
    roleNames.insert(AvatarRole, "roomAvatar");
    return roleNames;
}

void RoomDataModel::addRoom(int id, const QString &name, const QString &description, const QString &motd, int size, const QString &avatar)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    RoomData* rd = new RoomData(this);
    rd->setId(id);
    rd->setName(name);
    rd->setDescription(description);
    rd->setSize(size);
    rd->setMotd(motd);
    rd->setAvatar(avatar);
    roomsList.append(rd);
    endInsertRows();
    qWarning() << "Just added a new room to the data model";
}

void RoomDataModel::reset()
{
    while (!roomsList.isEmpty()) {
        roomsList.takeFirst()->deleteLater();
    }
}
