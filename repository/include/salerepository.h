#ifndef SALEREPOSITORY_H
#define SALEREPOSITORY_H

#include <QVector>
#include <QSqlQuery>
#include <QDateTime>

struct Sale;

class SaleRepository
{
public:
    static bool save(Sale& s); // also saves SaleItems transactionally
    static bool remove(int id);
    static Sale getById(int id);
    static QVector<Sale> getByDateRange(const QDateTime& start, const QDateTime& end);
};

#endif // SALEREPOSITORY_H
