#ifndef ARTICLEREPOSITORY_H
#define ARTICLEREPOSITORY_H

#include <QVector>
#include <QSqlDatabase>

#include "articledao.h"
#include "sqlsession.h"
#include "subcategorydao.h"
#include "supplierdao.h"

class ArticleRepository
{
public:
    ArticleRepository(QSqlDatabase &db);

    // Inventory page
    std::optional<Article> getFullArticle(int id);
    QVector<Article> getAllArticles();
    QVector<Article> GetSearchResult(const QVector<Article>& articles, QStringView txtInput);

    /**
     * @brief Advanced filter using Category flags and specific SubCategory IDs.
     * Hydrates articles with SubCategory data if missing.
     */
    QVector<Article> GetFilteredArticles(QVector<Article> &articles, CategoryFlags categoryFilter, const QSet<int>& subCategoryIdFilter = {});
    QVector<Article> GetFilteredArticles(QVector<Article> &articles, const QSet<int>& supplierIdFilter);

    bool Save(Article& article);
    bool Save(QVector<Article>& articles);
    bool Remove(int id);
    bool Remove(std::vector<int> id);

    void SetPicture(Article& article, QStringView picPath);
    void DeletePicture(Article& article, QStringView picPath); // ?

private:
    void loadOptionalSubCategory(QVector<Article> &articles, CategoryFlags categoryFilter = {});
    void loadOptionalSupplier(QVector<Article> &articles, const QSet<int> &supplierIdFilter = {});

    CategoryFlags eFilter;
    ArticleDao mArticleDao;
    ArticleVariantDao mVariantDao;
    SubCategoryDao mSubCategoryDao;
    SupplierDao mSupplierDao;
};

#endif // ARTICLEREPOSITORY_H
