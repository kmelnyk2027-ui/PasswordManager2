#include "passwordmodel.h"

PasswordTableModel::PasswordTableModel(QObject *parent)
    : QAbstractTableModel(parent), m_nextId(1)
{
}

int PasswordTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_entries.size();
}

int PasswordTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant PasswordTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole) return QVariant();
    int row = index.row();
    int col = index.column();
    if (row < 0 || row >= m_entries.size()) return QVariant();
    const PasswordEntry &e = m_entries.at(row);
    switch (col) {
    case IdColumn: return e.id;
    case TitleColumn: return e.title;
    case UsernameColumn: return e.username;
    case PasswordColumn: return e.password;
    case WebsiteColumn: return e.website;
    case CategoryColumn: return e.category;
    case UpdatedAtColumn: return e.updatedAt.toString("yyyy-MM-dd hh:mm");
    default: return QVariant();
    }
}

QVariant PasswordTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) return QVariant();
    if (role != Qt::DisplayRole) return QVariant();
    switch (section) {
    case IdColumn: return "ID";
    case TitleColumn: return "Title";
    case UsernameColumn: return "Username";
    case PasswordColumn: return "Password";
    case WebsiteColumn: return "Website";
    case CategoryColumn: return "Category";
    case UpdatedAtColumn: return "Updated At";
    default: return QVariant();
    }
}

Qt::ItemFlags PasswordTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (index.column() != IdColumn) {
        f |= Qt::ItemIsEditable;
    }
    return f;
}

bool PasswordTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;
    int row = index.row();
    int col = index.column();
    if (row < 0 || row >= m_entries.size()) return false;

    PasswordEntry &e = m_entries[row];
    switch (col) {
    case TitleColumn: e.title = value.toString(); break;
    case UsernameColumn: e.username = value.toString(); break;
    case PasswordColumn: e.password = value.toString(); break;
    case WebsiteColumn: e.website = value.toString(); break;
    case CategoryColumn: e.category = value.toString(); break;
    case UpdatedAtColumn: e.updatedAt = QDateTime::currentDateTime(); break;
    default: return false;
    }
    if (col != UpdatedAtColumn) {
        e.updatedAt = QDateTime::currentDateTime();
    }
    emit dataChanged(index, index);
    return true;
}

void PasswordTableModel::setEntries(const QList<PasswordEntry> &entries)
{
    beginResetModel();
    m_entries = entries;
    updateNextId();
    endResetModel();
}

void PasswordTableModel::addEmptyEntry()
{
    beginInsertRows(QModelIndex(), m_entries.size(), m_entries.size());
    PasswordEntry e;
    e.id = m_nextId++;
    e.title = "";
    e.username = "";
    e.password = "";
    e.website = "";
    e.category = "";
    e.updatedAt = QDateTime::currentDateTime();
    m_entries.append(e);
    endInsertRows();
}

void PasswordTableModel::removeEntry(int row)
{
    if (row < 0 || row >= m_entries.size()) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_entries.removeAt(row);
    endRemoveRows();
}

PasswordEntry PasswordTableModel::entryAt(int row) const
{
    if (row < 0 || row >= m_entries.size()) return PasswordEntry();
    return m_entries.at(row);
}

void PasswordTableModel::updateNextId()
{
    int maxId = 0;
    for (const PasswordEntry &e : m_entries) {
        if (e.id > maxId) maxId = e.id;
    }
    m_nextId = maxId + 1;
}
