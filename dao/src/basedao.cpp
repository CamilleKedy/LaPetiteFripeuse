#include "basedao.h"
#include <QSqlError>

BaseDao::BaseDao(QSqlDatabase &db)
    : m_db(db)
{
}

void BaseDao::logError(const QSqlQuery &query, const QString &operation) const
{
    qWarning() << "DAO Error during" << operation << "on table"
               << query.lastQuery().split(' ')[2].toUpper() << ":";
    qWarning() << "  Query:" << query.lastQuery();
    qWarning() << "  Error:" << query.lastError().text();
}
