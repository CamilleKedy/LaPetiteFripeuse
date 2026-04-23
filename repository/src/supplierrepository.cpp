#include "supplierrepository.h"

SupplierRepository::SupplierRepository(QSqlDatabase& db)
    : mSupplierDao(db)
{
}

QVector<Supplier> SupplierRepository::GetAll()
{
    return mSupplierDao.getAll();
}

QVector<Supplier> SupplierRepository::GetSearchResult(const QVector<Supplier> &suppliers, QStringView txtInput)
{
    if (txtInput.isEmpty()) return suppliers;

    QVector<Supplier> result;

    std::copy_if(
        suppliers.begin(),
        suppliers.end(),
        std::back_inserter(result),
        [&](const Supplier& s) {
            return s.name.contains(txtInput, Qt::CaseInsensitive);
        }
    );
    return result;
}

int SupplierRepository::Save(Supplier &supplier)
{
    if (supplier.id == -1)
    {
        return mSupplierDao.insert(supplier);
    }
    return mSupplierDao.update(supplier);
}

bool SupplierRepository::Remove(int id)
{
    if (!mSupplierDao.remove(id))
    {
        return false;
    }
    return true;
}

bool SupplierRepository::Remove(std::vector<int> ids)
{
    if (!mSupplierDao.remove(ids))
    {
        return false;
    }
    return true;
}



