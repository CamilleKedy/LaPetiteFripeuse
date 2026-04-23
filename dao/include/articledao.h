#ifndef ARTICLEDAO_H
#define ARTICLEDAO_H

#include "articlevariantdao.h"
#include "basedao.h"

struct Article;
struct ArticleVariant;

class ArticleDao : public BaseDao
{
public:
    explicit ArticleDao(QSqlDatabase& db);

    /**
     * @brief Inserts an Article and all its associated ArticleVariants in a single transaction
     * @param article The Article DTO to insert
     * @return The ID of the newly inserted Article on success, -1 otherwise
     */
    int insert(Article& article);

    /**
     * @brief Retrieves an Article by its ID, including all associated ArticleVariants
     */
    std::optional<Article> getById(int id);

    /**
     * @brief Updates an Article's main data (variants are currently not handled in update)
     */
    bool update(Article &article);

    /**
     * @brief Removes an Article and all its associated ArticleVariants (DB Foreign Key ON DELETE CASCADE is assumed).
     */
    bool remove(int id);

    /**
     * @brief Retrieves all Articles
     */
    QVector<Article> getAll();

private:
    /**
     * @brief Helper to map a QSqlQuery record to an Article DTO
     */
    std::optional<Article> mapFromQuery(QSqlQuery& query);

    /**
     * @brief Helper to map a QSqlQuery record to an ArticleVariant DTO.
     */
    std::optional<ArticleVariant> mapVariantFromQuery(QSqlQuery& query);

    /**
     * @brief Helper to insert all variants associated with a newly created Article ID
     * Used within the transactional insert.
     */
    bool insertVariants(int articleId, QVector<ArticleVariant>& variants);

    /**
     * @brief Helper function to update all variants associated with an Article ID
     * Used within the transactional update
     */
    bool updateVariants(int articleId, QVector<ArticleVariant>& variants);

    QVector<ArticleVariant> getVariantsByArticleId(int articleId);

    ArticleVariantDao mVariantDao;
};

#endif // ARTICLEDAO_H
