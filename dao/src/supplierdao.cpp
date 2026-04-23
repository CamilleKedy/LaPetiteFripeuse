#include "supplierdao.h"
#include "sqlsession.h"

SupplierDao::SupplierDao(QSqlDatabase &db)
    : BaseDao(db)
{
}

int SupplierDao::insert(Supplier &supplier)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO Supplier (name)
        VALUES :name
    )");
    query.bindValue(":name", supplier.name);

    if (query.exec())
    {
        supplier.id = query.lastInsertId().toInt();
        return supplier.id;
    }
    else
    {
        logError(query, "INSERT Supplier");
        return -1;
    }
}

bool SupplierDao::update(const Supplier &supplier)
{
    if (supplier.id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE Supplier
        SET name = :name
        WHERE id = :id
    )");
    query.bindValue(":name", supplier.name);
    query.bindValue(":id", supplier.id);

    if (query.exec())
    {
        return query.numRowsAffected() == 1;
    }
    else
    {
        logError(query, "UPDATE Supplier");
        return false;
    }
}

bool SupplierDao::remove(int id)
{
    if (id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM Supplier WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (query.exec())
    {
        return query.numRowsAffected() > 0;
    }
    else
    {
        logError(query, "DELETE Supplier");
        return false;
    }
}

bool SupplierDao::remove(std::vector<int> ids)
{
    if (!m_db.transaction())
    {
        qWarning() << "Failed to start Transaction to DELETE Supplier";
        return false;
    }

    for (const auto & id : ids)
    {
        if (!remove(id))
        {
            m_db.rollback();
            qWarning() << "Transaction failed for DELETE Supplier.";
            return false;
        }
    }

    if (!m_db.commit())
    {
        qWarning() << "Failed to commit transaction for Supplier DELETE.";
        m_db.rollback();
        return false;
    }

    return true;
}

std::optional<Supplier> SupplierDao::getById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, name FROM Supplier
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (query.exec())
    {
        return mapFromQuery(query);
    }
    else
    {
        logError(query, "SELECT Supplier by ID");
        return std::nullopt;
    }
}

QVector<Supplier> SupplierDao::getById(const QSet<int> &ids)
{
    QVector<Supplier> suppliers;

    if (ids.empty()) return suppliers;
    QStringList suppliersIdsPlaceholder;
    for (const int id : ids)
    {
        suppliersIdsPlaceholder << QString(":supplier_id_%1").arg(id);
    }

    QString sqlStr = QString(R"(
        SELECT id, name FROM Supplier
        WHERE id IN (%1)
    )").arg(suppliersIdsPlaceholder.join(", "));

    QSqlQuery query(m_db);
    query.prepare(sqlStr);

    const QVector<int> idsVector(ids.begin(), ids.end());
    for (int i = 0; i < idsVector.size(); i++)
    {
        query.bindValue(suppliersIdsPlaceholder[i], idsVector[i]);
    }

    if (query.exec())
    {
        suppliers.reserve(query.size());
        Supplier s;
        while (query.next())
        {
            s.id = query.value("id").toInt();
            s.name = query.value("name").toString();

            suppliers.append(s);
        }
    }
    else
    {
        logError(query, "SELECT Suppliers");
    }
    return suppliers;
}

QVector<Supplier> SupplierDao::getAll()
{
    QVector<Supplier> suppliers;
    QSqlQuery query(m_db);
    if (query.exec(R"(SELECT id, name FROM Supplier)"))
    {
        suppliers.reserve(query.size());
        Supplier s;
        while (query.next())
        {
            s.id = query.value("id").toInt();
            s.name = query.value("name").toString();

            suppliers.append(s);
        }
    }
    else
    {
        logError(query, "SELECT Supplier");
    }
    return suppliers;
}

std::optional<Supplier> SupplierDao::mapFromQuery(QSqlQuery &query)
{
    if (query.next())
    {
        Supplier s;
        s.id = query.value("id").toInt();
        s.name = query.value("name").toString();
        return s;
    }
    return std::nullopt;
}
