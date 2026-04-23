#include "saledao.h"
#include "sqlsession.h"

SaleDao::SaleDao(QSqlDatabase& db) : BaseDao(db) {}

int SaleDao::insert(Sale &sale)
{
    if (!m_db.transaction())
    {
        qWarning() << "Failed to start transaction for Sale Insertion";
        return -1;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO Sale (total_price, selling_price, is_edited, selling_date, is_online_sale)
        VALUES (0, 0, :is_edited, :selling_date, :is_online_sale)
    )"); // 0, 0, ... because of sql triggers.
    query.bindValue(":is_edited", sale.isEdited);
    query.bindValue(":selling_date", sale.sellingDate);
    query.bindValue(":is_online_sale", sale.isOnlineSale);

    if (query.exec())
    {
        logError(query, "INSERT Sale");
        m_db.rollback();
        return -1;
    }

    sale.id = query.lastInsertId().toInt();
    if (!insertSaleItems(sale.id, sale.items))
    {
        m_db.rollback();
        return -1;
    }

    if (!m_db.commit())
    {
        qWarning() << "Failed to commit transaction for Sale Insertion";
        m_db.rollback();
        return -1;
    }

    return sale.id;
}

std::optional<Sale> SaleDao::getById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, total_price, selling_price, is_edited, selling_date, is_online_sale
        FROM Sale
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        logError(query, "SELECT Sale by ID");
        return std::nullopt;
    }

    if (query.next())
    {
        Sale s;
        s.id = query.value("id").toInt();
        s.totalPrice = query.value("total_price").toInt();
        s.sellingPrice = query.value("selling_price").toInt();
        s.isEdited = query.value("is_edited").toBool();
        s.sellingDate = query.value("selling_date").toDateTime();
        s.isOnlineSale = query.value("is_online_sale").toBool();
    }
    return std::nullopt;
}

bool SaleDao::update(const Sale &sale)
{
    if (sale.id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE Sale
        SET total_price = :total_price,
            selling_price = :selling_price,
            is_edited = :is_edited,
            selling_date = :selling_date,
            is_online_sale = :is_online_sale
        WHERE id = :id
    )");
    query.bindValue(":total_price", sale.totalPrice);
    query.bindValue(":selling_price", sale.sellingPrice);
    query.bindValue(":is_edited", sale.isEdited);
    query.bindValue(":selling_date", sale.sellingDate);
    query.bindValue(":is_online_sale", sale.isOnlineSale);
    query.bindValue(":id", sale.id);

    if (!query.exec())
    {
        logError(query, "UPDATE Sale");
        return false;
    }
    return query.numRowsAffected() == 1;
}

bool SaleDao::remove(int id)
{
    if (id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM Sale WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        logError(query, "DELETE Sale");
        return false;
    }
    return query.numRowsAffected() > 0;
}

QVector<Sale> SaleDao::getAll()
{
    QVector<Sale> sales;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, total_price, selling_price, is_edited, selling_date, is_online_sale
        FROM Sale
    )");

    if (!query.exec())
    {
        logError(query, "SELECT Sale (All)");
        return sales;
    }

    sales.reserve(query.size());
    Sale s;
    while (query.next())
    {
        s.id = query.value("id").toInt();
        s.totalPrice = query.value("total_price").toInt();
        s.sellingPrice = query.value("selling_price").toInt();
        s.isEdited = query.value("is_edited").toBool();
        s.sellingDate = query.value("selling_date").toDateTime();
        s.isOnlineSale = query.value("is_online_sale").toBool();

        sales.append(s);
    }
    return sales;
}

bool SaleDao::insertSaleItems(int saleId, const QVector<SaleArticleVariant> &items)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO Sale (sale_id, variant_id, quantity, price_at_sale)
        VALUES (:sale_id, :variant_id, :quantity, :price_at_sale)
    )");

    for (const auto & item : items)
    {
        query.bindValue(":sale_id", item.saleId);
        query.bindValue(":variant_id", item.variantId);
        query.bindValue(":quantity", item.quantity);
        query.bindValue(":price_at_sale", item.priceAtSale);

        if (!query.exec())
        {
            logError(query, "INSERT SaleArticleVariant (Line Item)");
            return false;
        }
    }
    return true;
}
