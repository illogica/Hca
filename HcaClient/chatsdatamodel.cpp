#include "chatsdatamodel.h"

ChatsDataModel::ChatsDataModel(QObject *parent): QAbstractListModel(parent)
{
}

void ChatsDataModel::addChat(bool isUser, int id, const QString &name)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_chats << Chat(isUser, id, name);
    endInsertRows();
}

void ChatsDataModel::reset()
{
    beginResetModel();
    m_chats.clear();
    endResetModel();
}

int ChatsDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_chats.count();
}

QVariant ChatsDataModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_chats.count())
        return QVariant();

    const Chat &chat = m_chats[index.row()];
    if (role == IsUserRole)
        return chat.isUser();
    else if (role == IdRole)
        return chat.id();
    else if (role == NameRole)
        return chat.name();
    return QVariant();
}

QHash<int, QByteArray> ChatsDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IsUserRole] = "chatIsUser";
    roles[IdRole] = "chatId";
    roles[NameRole] = "chatName";
    return roles;
}
