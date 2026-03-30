#ifndef PASSWORDREPOSITORY_H
#define PASSWORDREPOSITORY_H

#include <QSqlDatabase>
#include <QList>
#include "passwordmodel.h"

class PasswordRepository
{
public:
    explicit PasswordRepository(const QSqlDatabase &db);

    QList<PasswordEntry> loadAll() const;
    bool insert(const PasswordEntry &entry);
    bool update(const PasswordEntry &entry);
    bool remove(int id);

private:
    QSqlDatabase m_db;
};

#endif
