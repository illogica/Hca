#ifndef CHATSDATAMODEL_H
#define CHATSDATAMODEL_H

#include <QAbstractListModel>

class Chat{
public:
    Chat(bool isUser, int id, const QString &name){
        m_isUser = isUser;
        m_id = id;
        m_name = name;
    }

    bool isUser() const { return m_isUser;}
    int id() const { return m_id;}
    QString name() const {return m_name;}

private:
    bool m_isUser; //either user or group
    int m_id;  //id of either the user or the group
    QString m_name;
};


class ChatsDataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ChatRoles{
        IsUserRole = Qt::UserRole+1,
        IdRole,
        NameRole
    };

    ChatsDataModel(QObject *parent = 0);
    void addChat(bool isUser, int id, const QString &name);
    void reset();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Chat> m_chats;
};

#endif // CHATSDATAMODEL_H
