#ifndef PASSWORMMODEL_H
#define PASSWORMMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDateTime>

struct PasswordEntry
{
    int id;
    QString title;
    QString username;
    QString password;
    QString website;
    QString category;
    QDateTime updatedAt;
};

class PasswordTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {
        IdColumn = 0,
        TitleColumn,
        UsernameColumn,
        PasswordColumn,
        WebsiteColumn,
        CategoryColumn,
        UpdatedAtColumn,
        ColumnCount
    };

    explicit PasswordTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setEntries(const QList<PasswordEntry> &entries);
    void addEmptyEntry();
    void removeEntry(int row);
    PasswordEntry entryAt(int row) const;

private:
    QList<PasswordEntry> m_entries;
    int m_nextId;

    void updateNextId();
};

#endif
