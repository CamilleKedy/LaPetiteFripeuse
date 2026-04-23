#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    ~DatabaseManager();

    bool openDatabase();
    void closeDatabase();
    QSqlDatabase &database();
    QSqlError lastError() const;

    bool beginTransaction();
    bool commit();
    bool rollback();

private:
    DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    QSqlError m_lastError;
};

#endif // DATABASEMANAGER_H
