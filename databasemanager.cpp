#include "databasemanager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QDir>
#include <qregularexpression.h>


/* Helper function to execute SQL script from a separate file
 * For now it doesn't handle comment block (/\*...*\/) style
*/
bool execSqlScript(QSqlDatabase &db, const QString &resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Could not open SQL file:" << file.errorString();
        return false;
    }

    QString sqlScript = file.readAll();
    file.close();

    // Split by semicolons into individual statements
    QStringList statements = sqlScript.split("-- ;;", Qt::SkipEmptyParts);

    if (!db.transaction())
    {
        qInfo() << "execSqlScript Transaction failed";
        return false;
    }
    QSqlQuery query(db);

    for (QString statement : std::as_const(statements))
    {
        statement = statement.trimmed();

        // Skip empty lines and SQL comments
        if (statement.isEmpty() || statement.startsWith("--"))
            continue;

        if (!query.exec(statement))
        {
            db.rollback();
            qWarning() << "SQL execution error:" << query.lastError().text();
            qWarning() << "Failed statement:" << statement;
            return false;
        }
    }

    if (!db.commit())
    {
        qInfo() << "execSqlScript Transaction Commit failed";
        return false;
    }

    qInfo() << "Script successfully executed";
    return true;
}

DatabaseManager::DatabaseManager() {}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkdir(dbPath);
    dbPath += "/LaPetiteFripeuse.db";
    m_db.setDatabaseName(dbPath);
    qInfo() << "DB Location:" << dbPath;

    if(!m_db.open())
    {
        qWarning() << __FILE__ << __LINE__ << ":" << m_db.lastError().text();
        return false;
    }
    qInfo() << "Connection established with database";

    if(!execSqlScript(m_db, ":/sql/init.sql"))
    {
        qWarning() << __FILE__ << __LINE__ << ": init sql script error";
        return false;
    }
#if 0
    if(!execSqlScript(m_db, ":/sql/dummy.sql"))
    {
        qWarning() << __FILE__ << __LINE__ << ": dummy sql script error";
        return false;
    }
#endif
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (m_db.isOpen())
    {
        QString connName = m_db.connectionName();
        m_db.close();
        m_db = QSqlDatabase(); // clear handle
        QSqlDatabase::removeDatabase(connName);
    }
}

QSqlDatabase& DatabaseManager::database()
{
    return m_db;
}

QSqlError DatabaseManager::lastError() const
{
    return m_lastError;
}

bool DatabaseManager::beginTransaction()
{
    if(!m_db.isOpen())
    {
        return false;
    }
    return m_db.transaction();
}

bool DatabaseManager::commit()
{
    if(!m_db.isOpen())
    {
        return false;
    }
    return m_db.commit();
}

bool DatabaseManager::rollback()
{
    if(!m_db.isOpen())
    {
        return false;
    }
    return m_db.rollback();
}
