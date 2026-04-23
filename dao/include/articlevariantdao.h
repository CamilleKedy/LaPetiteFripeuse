#ifndef ARTICLEVARIANTDAO_H
#define ARTICLEVARIANTDAO_H

#include "basedao.h"

struct ArticleVariant;

class ArticleVariantDao : public BaseDao
{
public:
    explicit ArticleVariantDao(QSqlDatabase& db);

    /**
     * @brief Inserts a new ArticleVariant
     */
    int insert(ArticleVariant& variant);

    /**
     * @brief Updates an existing ArticleVariant
     */
    bool update(const ArticleVariant& variant);

    /**
     * @brief Removes an ArticleVariant by its ID
     */
    bool remove(int id);

    /**
     * @brief Retrieves an ArticleVariant by its ID
     */
    std::optional<ArticleVariant> getById(int id);

    /**
     * @brief Retrieves all variants for a specific Article ID
     */
    QVector<ArticleVariant> getByArticleId(int articleId);

private:
    /**
     * @brief Helper to map a QSqlQuery record to an ArticleVariant DTO
     */
    std::optional<ArticleVariant> mapFromQuery(QSqlQuery& query);
};

#endif // ARTICLEVARIANTDAO_H
