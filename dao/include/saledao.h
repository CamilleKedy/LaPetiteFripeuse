#ifndef SALEDAO_H
#define SALEDAO_H

#include "basedao.h"

struct Sale;
struct SaleArticleVariant;

class SaleDao : public BaseDao
{
public:
    explicit SaleDao(QSqlDatabase& db);

    /**
     * @brief Inserts a Sale and all its associated SaleArticleVariants in a single transaction
     * @param sale The Sale DTO to insert
     * @return The ID of the newly inserted Sale on success, -1 otherwise
     */
    int insert(Sale& sale);

    /**
     * @brief Retrieves a Sale by its ID, including all associated SaleArticleVariants (line items)
     */
    std::optional<Sale> getById(int id);

    /**
     * @brief Updates a Sale's main data
     */
    bool update(const Sale& sale);

    /**
     * @brief Removes a Sale and all its associated line items (DB Foreign Key ON DELETE CASCADE is assumed).
     */
    bool remove(int id);

    /**
     * @brief Retrieves all Sales.
     */
    QVector<Sale> getAll();

private:
    /**
     * @brief Helper to map a QSqlQuery record to a Sale DTO (without items)
     */
    std::optional<Sale> mapFromQuery(QSqlQuery& query);

    /**
     * @brief Helper to map a QSqlQuery record to a SaleArticleVariant DTO
     */
    std::optional<SaleArticleVariant> mapSaleVariantFromQuery(QSqlQuery& query);

    /**
     * @brief Inserts all line items associated with a newly created Sale ID
     * Used within the transactional insert
     */
    bool insertSaleItems(int saleId, const QVector<SaleArticleVariant>& items);
};

#endif // SALEDAO_H
