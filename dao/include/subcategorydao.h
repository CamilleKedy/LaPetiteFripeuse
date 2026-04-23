#ifndef SUBCATEGORYDAO_H
#define SUBCATEGORYDAO_H

#include "basedao.h"
#include "sqlsession.h"

class SubCategoryDao : public BaseDao
{
public:
    explicit SubCategoryDao(QSqlDatabase& db);

    /**
     * @brief Inserts a new SubCategory into the database
     * @param subCategory The DTO to insert
     * @return The ID of the newly inserted SubCategory on success, -1 otherwise
     */
    int insert(SubCategory& subCategory);

    /**
     * @brief Updates an existing SubCategory in the database
     * @param subCategory The DTO with updated data
     * @return true on success, false otherwise
     */
    bool update(const SubCategory& subCategory);

    /**
     * @brief Removes a SubCategory by its ID.
     * @param id The ID of the SubCategory to remove
     * @return true on success, false otherwise
     */
    bool remove(int id);

    /**
     * @brief Retrieves a SubCategory by its ID
     * @param id The ID to retrieve.
     * @return std::optional<SubCategory> containing the DTO if found, empty otherwise
     */
    std::optional<SubCategory> getById(int id);

    /**
     * @brief Retrieves all SubCategories from the database
     */
    QVector<SubCategory> getAll();

    /**
     * @brief Retrieves all SubCategories associated with a specific Category ID.
     */
    QVector<SubCategory> getByCategory(Category::Id categoryId);

    /**
     * @brief Retrieves all SubCategories associated with a specific Category ID within a Set
     */
    QVector<SubCategory> getByCategory(const QSet<Category::Id>& categoryIds);

    /**
     * @brief Retrieves all SubCategories associated with a specific Category ID within a QFlags
     */
    QVector<SubCategory> getByCategory(CategoryFlags categoryIdFlags);

private:
    /**
     * @brief Helper to map a QSqlQuery record to a SubCategory DTO.
     */
    std::optional<SubCategory> mapFromQuery(QSqlQuery& query);
};

#endif // SUBCATEGORYDAO_H
