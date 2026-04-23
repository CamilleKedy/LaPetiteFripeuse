#ifndef BASEDAO_H
#define BASEDAO_H

#include <QSqlDatabase>
#include <QSqlQuery>

class BaseDao
{
public:
    explicit BaseDao(QSqlDatabase& db);
    virtual ~BaseDao() = default;

protected:
    QSqlDatabase& m_db;

    /**
     * @brief Helper function for logging query errors
     */
    void logError(const QSqlQuery& query, const QString& operation) const;
};

#endif // BASEDAO_H
