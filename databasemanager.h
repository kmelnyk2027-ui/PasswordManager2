#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool open(const QString &path);
    bool initializeSchema();
    QSqlDatabase database() const;

private:
    QSqlDatabase m_db;
};

#endif
