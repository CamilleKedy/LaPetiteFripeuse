#include "articlevariantdao.h"
#include "sqlsession.h"

ArticleVariantDao::ArticleVariantDao(QSqlDatabase& db)
    : BaseDao(db)
{
}

int ArticleVariantDao::insert(ArticleVariant &variant)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO ArticleVariant (article_id, uuid, price, min_price, quantity, attributes)
        VALUES (:articleId, :uuid, :price, :minPrice, :quantity, :attributes)
    )");

    if(variant.uuid.isNull())
    {
        variant.uuid = QUuid::createUuid();
    }

    QByteArray attributesJson = ArticleVariant::toJson(variant.attributes);
    query.bindValue(":article_id", variant.articleId);
    query.bindValue(":uuid", variant.uuid);
    query.bindValue(":price", variant.price);
    query.bindValue(":min_price", variant.minPrice);
    query.bindValue(":quantity", variant.quantity);
    query.bindValue(":attributes", attributesJson.isEmpty() ? QVariant() : attributesJson);

    if(query.exec())
    {
        variant.id = query.lastInsertId().toInt();
        return variant.id;
    }
    else
    {
        logError(query, "INSERT ArticleVariant");
        return -1;
    }
}

bool ArticleVariantDao::update(const ArticleVariant &variant)
{
    if(variant.id == -1)
    {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE ArticleVariant
        SET article_id = :article_id,
            uuid = :uuid,
            price = :price,
            min_price = :min_price,
            quantity = :quantity,
            attributes = :attributes
        WHERE id = :id
    )");

    QByteArray attributes = ArticleVariant::toJson(variant.attributes);
    query.bindValue(":article_id", variant.articleId);
    query.bindValue(":uuid", variant.uuid);
    query.bindValue(":price", variant.price);
    query.bindValue(":min_price", variant.minPrice);
    query.bindValue(":quantity", variant.quantity);
    query.bindValue(":attributes", attributes);
    query.bindValue(":id", variant.id);

    if(query.exec())
    {
        return query.numRowsAffected() == 1;
    }
    else
    {
        logError(query, "UPDATE ArticleVariant");
        return false;
    }
}

bool ArticleVariantDao::remove(int id)
{
    if(id == -1)
    {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM ArticleVariant WHERE id = :id
    )");
    query.bindValue(":id", id);

    if(query.exec())
    {
        return query.numRowsAffected() > 0;
    }
    else
    {
        logError(query, "DELETE ArticleVariant");
        return false;
    }
}

std::optional<ArticleVariant> ArticleVariantDao::getById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, article_id, uuid, price, min_price, quantity, attributes
        FROM ArticleVariant
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if(query.exec())
    {
        return mapFromQuery(query);
    }
    else
    {
        logError(query, "SELECT ArticleVariant by ID");
        return std::nullopt;
    }
}

QVector<ArticleVariant> ArticleVariantDao::getByArticleId(int articleId)
{
    QVector<ArticleVariant> variants;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, article_id, uuid, price, min_price, quantity, attributes
        FROM ArticleVariant
        WHERE article_id = :article_id
    )");
    query.bindValue(":article_id", articleId);

    if(query.exec())
    {
        variants.reserve(query.size());
        ArticleVariant v;
        while(query.next())
        {
            v.id = query.value("id").toInt();
            v.articleId = query.value("article_id").toInt();
            v.uuid = query.value("uuid").toUuid();
            v.price = query.value("price").toInt();
            v.minPrice = query.value("min_price").toInt();
            v.quantity = query.value("quantity").toInt();

            QByteArray attributesJson = query.value("attributes").toByteArray();
            v.attributes = ArticleVariant::fromJson(attributesJson);

            variants.append(v);
        }
    }
    else
    {
        logError(query, "SELECT ArticleVariant by Article ID");
    }
    return variants;
}

std::optional<ArticleVariant> ArticleVariantDao::mapFromQuery(QSqlQuery &query)
{
    if(query.next())
    {
        ArticleVariant v;
        v.id = query.value("id").toInt();
        v.articleId = query.value("article_id").toInt();
        v.uuid = query.value("uuid").toUuid();
        v.price = query.value("price").toInt();
        v.minPrice = query.value("min_price").toInt();
        v.quantity = query.value("quantity").toInt();

        QByteArray attributesJson = query.value("attributes").toByteArray();
        v.attributes = ArticleVariant::fromJson(attributesJson);

        return v;
    }
    return std::nullopt;
}


