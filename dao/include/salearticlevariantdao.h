#ifndef SALEARTICLEVARIANTDAO_H
#define SALEARTICLEVARIANTDAO_H

#include "basedao.h"

struct SaleArticleVariant;

class SaleArticleVariantDao : public BaseDao
{
public:
    explicit SaleArticleVariantDao(QSqlDatabase& db);

    /**
     * @brief Inserts a single SaleArticleVariant record. (Usually done via SaleDao transaction)
     */
    bool insert(const SaleArticleVariant& sav);

    /**
     * @brief Retrieves all line items for a specific Sale ID, also hydrate cached fields
     */
    QVector<SaleArticleVariant> getBySaleId(int saleId);

    /**
     * @brief Removes a single SaleArticleVariant by its composite key (saleId, variantId)
     */
    bool remove(int saleId, int variantId);

private:
    /**
     * @brief Helper to map a QSqlQuery record to a SaleArticleVariant DTO, including JOINed fields
     */
    std::optional<SaleArticleVariant> mapFromQuery(QSqlQuery& query, bool includeCached = false);
};

#endif // SALEARTICLEVARIANTDAO_H
