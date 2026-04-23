#include "salearticlevariantdao.h"
#include "sqlsession.h"

SaleArticleVariantDao::SaleArticleVariantDao(QSqlDatabase& db) :BaseDao(db) {}

bool SaleArticleVariantDao::insert(const SaleArticleVariant &sav)
{
    QSqlQuery query(m_db);
    // Note: this is typically called within a SaleDao transaction.
    query.prepare("INSERT INTO SaleArticleVariant (sale_id, variant_id, quantity, price_at_sale) "
                  "VALUES (:sale_id, :variant_id, :quantity, :price_at_sale)");

    query.bindValue(":sale_id", sav.saleId);
    query.bindValue(":variant_id", sav.variantId);
    query.bindValue(":quantity", sav.quantity);
    query.bindValue(":price_at_sale", sav.priceAtSale);

    if (!query.exec())
    {
        logError(query, "INSERT SaleArticleVariant (Single)");
        return false;
    }
    return true;
}

QVector<SaleArticleVariant> SaleArticleVariantDao::getBySaleId(int saleId)
{
    QVector<SaleArticleVariant> items;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT sav.sale_id, sav.variant_id, sav.quantity, sav.price_at_sale,
                a.name AS article_name, av.uuid AS variant_uuid
        FROM SaleArticleVariant sav
        INNER JOIN ArticleVariant av ON av.id = sav.variant_id
        INNER JOIN Article a ON a.id = av.article_id
        WHERE sav.sale_id = :sale_id
    )");
    query.bindValue("sale_id", saleId);

    if (query.exec())
    {
        items.reserve(query.size());
        SaleArticleVariant sav;
        while (query.next())
        {
            sav.saleId = query.value("sale_id").toInt();
            sav.variantId = query.value("variant_id").toInt();
            sav.quantity = query.value("quantity").toInt();
            sav.priceAtSale = query.value("price_at_sale").toInt();

            sav._cachedArticleName = query.value("article_name").toString();
            sav._cachedVariantUuid = query.value("variant_uuid").toString();

            items.append(sav);
        }
    }
    else
    {
        logError(query, "SELECT SaleArticleVariant by Sale ID");
    }
    return items;
}

bool SaleArticleVariantDao::remove(int saleId, int variantId)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM SaleArticleVariant
        WHERE sale_id = :sale_id AND variant_id = :variant_id
    )");
    query.bindValue(":sale_id", saleId);
    query.bindValue(":variant_id", variantId);

    if (query.exec())
    {
        return query.numRowsAffected() > 0;
    }
    else
    {
        logError(query, "DELETE SaleArticleVariant");
        return false;
    }
}

std::optional<SaleArticleVariant> SaleArticleVariantDao::mapFromQuery(QSqlQuery& query, bool includeCached)
{
    if (query.next())
    {
        SaleArticleVariant sav;
        sav.saleId = query.value("sale_id").toInt();
        sav.variantId = query.value("variant_id").toInt();
        sav.quantity = query.value("quantity").toInt();
        sav.priceAtSale = query.value("price_at_sale").toInt();

        if (includeCached)
        {
            // These fields are only available when joining tables
            sav._cachedArticleName = query.value("article_name").toString();
            sav._cachedVariantUuid = query.value("variant_uuid").toString();
        }

        return sav;
    }
    return std::nullopt;
}




