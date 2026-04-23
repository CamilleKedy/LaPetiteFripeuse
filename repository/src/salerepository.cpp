#include "salerepository.h"

#include "sqlsession.h"
#include "databasemanager.h"

bool SaleRepository::save(Sale &s)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if(!db.isOpen())
    {
        qWarning() << "Database not open in" << __FUNCTION__;
        return false;
    }

    QSqlQuery query(db);
    if(!db.transaction())
    {
        qWarning() << "Failed to start transaction:" << db.lastError().text();
        return false;
    }

    bool success = true;
    if(s.id == -1)
    {
        query.prepare(R"(
            INSERT INTO Sale (
                total_price, selling_price, is_edited,
                selling_date, is_online_sale
            )
            VALUES (
                :total_price, :selling_price, :is_edited,
                :selling_date, :is_online_sale
            )
        )");
    }
    else
    {
        query.prepare(R"(
            UPDATE Sale
            SET total_price = :total_price,
                selling_price = :selling_price,
                is_edited = :is_edited,
                selling_date = :selling_date,
                is_online_sale = :is_online_sale
            WHERE id = :id
        )");
        query.bindValue(":id", s.id);
    }

    query.bindValue(":total_price", s.totalPrice);
    query.bindValue(":selling_price", s.sellingPrice);
    query.bindValue(":is_edited", s.isEdited);
    query.bindValue(":selling_date", s.sellingDate);
    query.bindValue(":is_online_sale", s.isOnlineSale);

    if(!query.exec())
    {
        qWarning() << "Save Sale failed:" << query.lastError().text();
        db.rollback();
        return false;
    }

    if (s.id == -1)
        s.id = query.lastInsertId().toInt();

    // Delete old items if updating
    QSqlQuery delQuery(db);
    delQuery.prepare("DELETE FROM SaleArticle WHERE sale_id = :sale_id");
    delQuery.bindValue(":sale_id", s.id);
    if(!delQuery.exec())
    {
        qWarning() << "Failed to clear SaleArticle:" << delQuery.lastError().text();
        db.rollback();
        return false;
    }

    // Insert new items
    QSqlQuery itemQuery(db);
    itemQuery.prepare(R"(
        INSERT INTO SaleArticle (sale_id, article_id, quantity, price_at_sale)
        VALUES (:sale_id, :article_id, :quantity, :price_at_sale)
    )");

    for(const auto& item : std::as_const(s.items))
    {
        itemQuery.bindValue(":sale_id", s.id);
        // itemQuery.bindValue(":article_id", item.articleId);
        itemQuery.bindValue(":quantity", item.quantity);
        itemQuery.bindValue(":price_at_sale", item.priceAtSale);

        if(!itemQuery.exec())
        {
            qWarning() << "Insert SaleArticle failed:" << itemQuery.lastError().text();
            success = false;
            break;
        }
    }

    if(!success || !db.commit())
    {
        qWarning() << "Insertion or Commit failed... Starting rollback...";
        db.rollback();
        return false;
    }

    return true;
}

bool SaleRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if(!db.isOpen())
    {
        qWarning() << "Database not open in" << __FUNCTION__;
        return false;
    }

    QSqlQuery q(db);
    q.prepare("DELETE FROM Sale WHERE id = :id");
    q.bindValue(":id", id);

    if(!q.exec())
    {
        qWarning() << "Delete Sale failed:" << q.lastError().text();
        return false;
    }

    return true;
}

Sale SaleRepository::getById(int id)
{
    Sale s;
    QSqlDatabase db = DatabaseManager::instance().database();
    if(!db.isOpen())
    {
        qWarning() << "Database not open in" << __FUNCTION__;
        return s;
    }

    // Fetch Sale
    QSqlQuery saleQuery(db);
    saleQuery.prepare("SELECT * FROM Sale WHERE id = :id");
    saleQuery.bindValue(":id", id);
    if(!saleQuery.exec() || !saleQuery.next())
    {
        qWarning() << "Get Sale failed:" << saleQuery.lastError().text();
        return s;
    }

    s.id = saleQuery.value("id").toInt();
    s.totalPrice = saleQuery.value("total_price").toDouble();
    s.sellingPrice = saleQuery.value("selling_price").toDouble();
    s.isEdited = saleQuery.value("is_edited").toBool();
    s.sellingDate = saleQuery.value("selling_date").toDateTime();
    s.isOnlineSale = saleQuery.value("is_online_sale").toBool();

    // Fetch SaleItems
    QSqlQuery itemQuery(db);
    itemQuery.prepare(R"(
        SELECT sale_id, article_id, quantity, price_at_sale
        FROM SaleArticle
        WHERE sale_id = :sale_id
    )");
    itemQuery.bindValue(":sale_id", id);
    if(!itemQuery.exec())
    {
        qWarning() << "Get SaleItems failed:" << itemQuery.lastError().text();
        return s;
    }

    // while(itemQuery.next())
    // {
    //     SaleItem i;
    //     i.saleId = id;
    //     i.articleId = itemQuery.value("article_id").toInt();
    //     i.quantity = itemQuery.value("quantity").toInt();
    //     i.priceAtSale = itemQuery.value("price_at_sale").toDouble();
    //     s.items.append(i);
    // }

    return s;
}

QVector<Sale> SaleRepository::getByDateRange(const QDateTime &start, const QDateTime &end)
{
    QVector<Sale> sales;
    QSqlDatabase db = DatabaseManager::instance().database();
    if(!db.isOpen())
    {
        qWarning() << "Database not open in" << __FUNCTION__;
        return sales;
    }

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT *
        FROM Sale
        WHERE selling_date BETWEEN :start AND :end
        ORDER BY selling_date DESC
    )");
    q.bindValue(":start", start);
    q.bindValue(":end", end);

    if(!q.exec())
    {
        qWarning() << "Get Sales by date range failed:" << q.lastError().text();
        return sales;
    }

    while(q.next())
    {
        Sale s;
        s.id = q.value("id").toInt();
        s.totalPrice = q.value("total_price").toDouble();
        s.sellingPrice = q.value("selling_price").toDouble();
        s.isEdited = q.value("is_edited").toBool();
        s.sellingDate = q.value("selling_date").toDateTime();
        s.isOnlineSale = q.value("is_online_sale").toBool();
        sales.append(s);
    }

    return sales;
}





