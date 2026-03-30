#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>

DatabaseManager::DatabaseManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DatabaseManager::open(const QString &path)
{
    m_db.setDatabaseName(path);
    return m_db.open();
}

bool DatabaseManager::initializeSchema()
{
    QSqlQuery query(m_db);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS passwords ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "username TEXT,"
        "password TEXT,"
        "website TEXT,"
        "category TEXT,"
        "updated_at TEXT NOT NULL"
        ")"
        );
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}
