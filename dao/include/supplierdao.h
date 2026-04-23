#ifndef SUPPLIERDAO_H
#define SUPPLIERDAO_H

#include "basedao.h"

struct Supplier;

class SupplierDao : public BaseDao
{
public:
    explicit SupplierDao(QSqlDatabase& db);

    /**
     * @brief Inserts a new Supplier into the database
     * @param supplier The DTO to insert
     * @return The ID of the newly inserted Supplier on success, -1 otherwise
     */
    int insert(Supplier& supplier);

    /**
     * @brief Updates an existing Supplier in the database
     * @param supplier The DTO with updated data
     * @return true on success, false otherwise
     */
    bool update(const Supplier& supplier);

    /**
     * @brief Removes a Supplier by its ID
     * @param id The ID of the Supplier to remove
     * @return true on success, false otherwise
     */
    bool remove(int id);

    /**
     * @brief Removes many Suppliers by their ID
     * @param ids The ID list of the Suppliers to remove
     * @return true on success, false otherwise
     */
    bool remove(std::vector<int> ids);

    /**
     * @brief Retrieves a Supplier by its ID
     * @param id The ID to retrieve
     * @return std::optional<Supplier> containing the DTO if found, empty otherwise
     */
    std::optional<Supplier> getById(int id);

    /**
     * @brief Retrieves a list of Suppliers by their ID
     */
    QVector<Supplier> getById(const QSet<int>& ids);

    /**
     * @brief Retrieves all Suppliers from the database
     */
    QVector<Supplier> getAll();

private:
    /**
     * @brief Helper to map a QSqlQuery record to a Supplier DTO
     */
    std::optional<Supplier> mapFromQuery(QSqlQuery& query);
};

#endif // SUPPLIERDAO_H
