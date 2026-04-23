#include "subcategorydao.h"

SubCategoryDao::SubCategoryDao(QSqlDatabase& db) : BaseDao(db) {}

int SubCategoryDao::insert(SubCategory &subCategory)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        INSERT INTO SubCategory (category_id, name)
        VALUES (:category_id, :name)
    )");
    query.bindValue(":category_id", static_cast<int>(subCategory.categoryId));
    query.bindValue(":name", subCategory.name);

    if (query.exec())
    {
        subCategory.id = query.lastInsertId().toInt();
        return subCategory.id;
    }
    else
    {
        logError(query, "INSERT SubCategory");
        return -1;
    }
}

bool SubCategoryDao::update(const SubCategory &subCategory)
{
    if (subCategory.id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        UPDATE SubCategory
        SET category_id = :category_id
            name = :name
        WHERE id = :id
    )");
    query.bindValue(":category_id", static_cast<int>(subCategory.categoryId));
    query.bindValue(":name", subCategory.name);
    query.bindValue(":id", subCategory.id);

    if (query.exec())
    {
        return query.numRowsAffected() == 1;
    }
    else
    {
        logError(query, "UPDATE SubCategory");
        return false;
    }
}

bool SubCategoryDao::remove(int id)
{
    if (id == -1) return false;

    QSqlQuery query(m_db);
    query.prepare(R"(
        DELETE FROM SubCategory WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (query.exec())
    {
        return query.numRowsAffected() > 0;
    }
    else
    {
        logError(query, "DELETE SubCategory");
        return false;
    }
}

std::optional<SubCategory> SubCategoryDao::getById(int id)
{
    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, category_id, name
        FROM SubCategory
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (query.exec())
    {
        SubCategory subCat;
        subCat.id = query.value("id").toInt();
        subCat.categoryId = static_cast<Category::Id>(query.value("category_id").toInt());
        subCat.name = query.value("name").toString();

        return subCat;
    }
    else
    {
        logError(query, "SELECT SubCategory by ID");
        return std::nullopt;
    }
}

QVector<SubCategory> SubCategoryDao::getAll()
{
    QVector<SubCategory> subCategories;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, category_id, name
        FROM SubCategory
    )");

    if (!query.exec())
    {
        logError(query, "SELECT All SubCategory");
        return subCategories;
    }

    subCategories.reserve(query.size());
    SubCategory subCat;
    while (query.next())
    {
        subCat.id = query.value("id").toInt();
        subCat.categoryId = static_cast<Category::Id>(query.value("category_id").toInt());
        subCat.name = query.value("name").toString();

        subCategories.append(subCat);
    }
    return subCategories;
}

QVector<SubCategory> SubCategoryDao::getByCategory(Category::Id categoryId)
{
    QVector<SubCategory> subCategories;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, category_id, name
        FROM SubCategory
        WHERE category_id = :category_id
    )");
    query.bindValue(":category_id", static_cast<int>(categoryId));

    if (!query.exec())
    {
        logError(query, "SELECT SubCategory by Category");
        return subCategories;
    }

    subCategories.reserve(query.size());
    SubCategory subCat;
    while (query.next())
    {
        subCat.id = query.value("id").toInt();
        subCat.categoryId = static_cast<Category::Id>(query.value("category_id").toInt());
        subCat.name = query.value("name").toString();

        subCategories.append(subCat);
    }
    return subCategories;
}

QVector<SubCategory> SubCategoryDao::getByCategory(const QSet<Category::Id>& categoryIds)
{
    QVector<SubCategory> subCategories;

    if (categoryIds.empty()) return subCategories;

    QStringList categoryIdsPlaceholder;
    for (const auto & catId : categoryIds)
    {
        categoryIdsPlaceholder << QString(":category_id_%1").arg(static_cast<int>(catId));
    }
    QString sqlStr = QString(R"(
        SELECT id, category_id, name
        FROM SubCategory
        WHERE category_id IN (%1)
    )").arg(categoryIdsPlaceholder.join(", "));

    QSqlQuery query(m_db);
    query.prepare(sqlStr);

    const QVector<Category::Id> categoryIdsVector(categoryIds.begin(), categoryIds.end());
    for (int i = 0; i < categoryIds.size(); i++)
    {
        query.bindValue(categoryIdsPlaceholder[i], static_cast<int>(categoryIdsVector[i]));
    }

    if (!query.exec())
    {
        logError(query, "SELECT SubCategory by Category Set");
        return subCategories;
    }

    subCategories.reserve(query.size());
    SubCategory subCat;
    while (query.next())
    {
        subCat.id = query.value("id").toInt();
        subCat.categoryId = static_cast<Category::Id>(query.value("category_id").toInt());
        subCat.name = query.value("name").toString();

        subCategories.append(subCat);
    }
    return subCategories;
}

QVector<SubCategory> SubCategoryDao::getByCategory(CategoryFlags categoryIdFlags)
{
    using Id = Category::Id;
    QSet<Id> set;
    if (categoryIdFlags.testFlag(Id::UNKNOWN)) set.insert(Id::UNKNOWN);
    if (categoryIdFlags.testFlag(Id::ADORNMENT)) set.insert(Id::ADORNMENT);
    if (categoryIdFlags.testFlag(Id::BAG)) set.insert(Id::BAG);
    if (categoryIdFlags.testFlag(Id::COSMETIC)) set.insert(Id::COSMETIC);
    if (categoryIdFlags.testFlag(Id::GARMENT)) set.insert(Id::GARMENT);
    if (categoryIdFlags.testFlag(Id::PERFUME)) set.insert(Id::PERFUME);
    if (categoryIdFlags.testFlag(Id::SHOES)) set.insert(Id::SHOES);

    return getByCategory(set);
}

std::optional<SubCategory> SubCategoryDao::mapFromQuery(QSqlQuery &query)
{
    if (query.next())
    {
        SubCategory subCat;
        subCat.id = query.value("id").toInt();
        subCat.categoryId = static_cast<Category::Id>(query.value("category_id").toInt());
        subCat.name = query.value("name").toString();

        return subCat;
    }
    return std::nullopt;
}
