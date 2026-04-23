#include "articledao.h"
#include "sqlsession.h"


ArticleDao::ArticleDao(QSqlDatabase& db)
    : BaseDao(db), mVariantDao(db)
{
}

int ArticleDao::insert(Article &article)
{
    if (!m_db.transaction())
    {
        qWarning() << "Failed to start transaction for Article Insertion";
        return -1;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO Article (uuid, name, description, creation_date, modification_date, supplier_id, subcategory_id)
        VALUES :uuid, :name, :description, :creation_date, :modification_date, :supplier_id, :subcategory_id
    )");

    if (article.uuid.isNull()) article.uuid = QUuid::createUuid();

    query.bindValue(":uuid", article.uuid);
    query.bindValue(":name", article.name);
    query.bindValue(":description", article.description);
    query.bindValue(":creation_date", article.creationDate);
    query.bindValue(":modification_date", article.modificationDate);
    query.bindValue(":supplier_id", article.supplierId);
    query.bindValue(":subcategory_id", article.subCategoryId);

    if (!query.exec())
    {
        m_db.rollback();
        logError(query, "INSERT Article");
        return -1;
    }

    article.id = query.lastInsertId().toInt();

    if (!insertVariants(article.id, article.variants))
    {
        m_db.rollback();
        return -1;
    }

    if (!m_db.commit())
    {
        qWarning() << "Failed to commit transaction for Article Insertion";
        m_db.rollback();
        return -1;
    }

    return article.id;
}

std::optional<Article> ArticleDao::getById(int id)
{
    if (id == -1) return std::nullopt;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT (id, uuid, name, description, creation_date, modification_date, supplier_id, subcategory_id)
        FROM Article
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        logError(query, "SELECT Article by ID");
        return std::nullopt;
    }

    Article article;
    article.id = query.value("id").toInt();
    article.uuid = query.value("uuid").toUuid();
    article.name = query.value("name").toString();
    article.description = query.value("description").toString();
    article.creationDate = query.value("creation_date").toDateTime();
    article.modificationDate = query.value("modification_date").toDateTime();
    article.supplierId = query.value("supplier_id").toInt();
    article.subCategoryId = query.value("subcategory_id").toInt();
    article.variants = getVariantsByArticleId(article.id);

    article.picPath = QString("IMG_%1.jpg").arg(article.uuid.toString(QUuid::WithoutBraces));

    return article;
}

bool ArticleDao::update(Article &article)
{
    if (article.id == -1) return false;

    if (!m_db.transaction())
    {
        qWarning() << "Failed to start Article transaction (Update)";
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE Article
        SET uuid = :uuid,
            name = :name,
            description = :description,
            modification_date = :modification_date,
            supplier_id = :supplier_id,
            subcategory_id = :subcategory_id
        WHERE id = :id
    )");
    query.bindValue(":uuid", article.uuid);
    query.bindValue(":name", article.name);
    query.bindValue(":description", article.description);
    query.bindValue(":modification_date", article.modificationDate);
    query.bindValue(":supplier_id", article.supplierId);
    query.bindValue(":subcategory_id", article.subCategoryId);
    query.bindValue(":id", article.id);

    if (!query.exec())
    {
        logError(query, "UPDATE Article");
        m_db.rollback();
        return false;
    }

    for (ArticleVariant& variant : article.variants)
    {
        if (variant.id == -1)
        {
            // New variant
            variant.articleId = article.id;
            if (mVariantDao.insert(variant) == -1)
            {
                m_db.rollback();
                return false;
            }
        } else {
            // Existing variant
            if (!mVariantDao.update(variant))
            {
                m_db.rollback();
                return false;
            }
        }
    }

    if (!m_db.commit())
    {
        qWarning() << "Failed to commit transaction for Article update.";
        m_db.rollback();
        return false;
    }

    return true;
}

bool ArticleDao::remove(int id)
{
    if (id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM Article WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        logError(query, "DELETE Article");
        return false;
    }

    return query.numRowsAffected() > 0;
}

QVector<Article> ArticleDao::getAll()
{
    QVector<Article> articles;
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, uuid, name, description, creation_date, modification_date, supplier_id, subcategory_id
        FROM Article
    )");

    if (!query.exec())
    {
        logError(query, "SELECT Article (All)");
        return articles;
    }

    articles.reserve(query.size());
    Article a;
    while (query.next())
    {
        a.id = query.value("id").toInt();
        a.uuid = query.value("uuid").toUuid();
        a.name = query.value("name").toString();
        a.description = query.value("description").toString();
        a.creationDate = query.value("creation_date").toDateTime();
        a.modificationDate = query.value("modification_date").toDateTime();
        a.subCategoryId = query.value("subcategory_id").toInt();
        a.supplierId = query.value("supplier_id").toInt();
        a.variants = getVariantsByArticleId(a.id);

        a.picPath = QString("IMG_%1.jpg").arg(a.uuid.toString(QUuid::WithoutBraces));

        articles.append(a);
    }

    return articles;
}


bool ArticleDao::insertVariants(int articleId, QVector<ArticleVariant> &variants)
{
    for (auto & v : variants)
    {
        v.articleId = articleId;
        if (mVariantDao.insert(v) == -1)
        {
            return false;
        }
    }
    return true;
}

bool ArticleDao::updateVariants(int articleId, QVector<ArticleVariant> &variants)
{
    // QVector<ArticleVariant> currentVariants = mVariantDao.getByArticleId(articleId);

    // for (auto& v : variants)
    // {
    //     if (v.id == -1)
    //     {
    //         mVariantDao.insert(v);
    //     }
    //     else
    //     {
    //         auto& cv = currentVariants;
    //         auto variantIt = std::find_if(cv.begin(), cv.end(),
    //                                       [&v](QVector<ArticleVariant>::const_iterator cvIt){
    //                                           return v.id == cvIt->id;
    //         });

    //         if (variantIt != nullptr)
    //         {
    //             if (*variantIt == v)
    //         }

    //     }
    //     v.articleId = articleId;
    //     if (!mVariantDao.update(v))
    //     {
    //         return false;
    //     }
    // }
    return true;
}

QVector<ArticleVariant> ArticleDao::getVariantsByArticleId(int articleId)
{
    QVector<ArticleVariant> variants = mVariantDao.getByArticleId(articleId);
    if (variants.empty())
    {
        qWarning() << "Missing variant. Article with ID" << articleId << "has no variant.";
    }

    return variants;
}

