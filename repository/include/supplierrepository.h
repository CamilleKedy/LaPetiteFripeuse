#ifndef SUPPLIERREPOSITORY_H
#define SUPPLIERREPOSITORY_H

#include <QVector>
#include <QSqlQuery>

#include "supplierdao.h"
#include "sqlsession.h"

class SupplierRepository
{
public:
    SupplierRepository(QSqlDatabase &db);

    QVector<Supplier> GetAll();
    QVector<Supplier> GetSearchResult(const QVector<Supplier>& suppliers, QStringView txtInput);

    int Save(Supplier &supplier);
    bool Remove(int id);
    bool Remove(std::vector<int> ids);

private:
    SupplierDao mSupplierDao;
};

#endif // SUPPLIERREPOSITORY_H
