#include "articlerepository.h"

ArticleRepository::ArticleRepository(QSqlDatabase &db)
    : mArticleDao(db), mVariantDao(db), mSubCategoryDao(db), mSupplierDao(db)
{
}

std::optional<Article> ArticleRepository::getFullArticle(int id)
{
    return mArticleDao.getById(id);
}

QVector<Article> ArticleRepository::getAllArticles()
{
    auto articles = mArticleDao.getAll();
    loadOptionalSubCategory(articles);
    loadOptionalSupplier(articles);

    return articles;
}

QVector<Article> ArticleRepository::GetSearchResult(const QVector<Article> &articles, QStringView txtInput)
{
    if (txtInput.empty()) return articles;

    QVector<Article> result;
    std::copy_if(
        articles.begin(),
        articles.end(),
        std::back_inserter(result),
        [&](const Article & a) {
            return (a.name.contains(txtInput, Qt::CaseInsensitive)
                    || a.description.contains(txtInput, Qt::CaseInsensitive));
        });
    return result;
}

QVector<Article> ArticleRepository::GetFilteredArticles(QVector<Article> &articles, CategoryFlags categoryFilter, const QSet<int> &subCategoryIdFilter)
{
    QVector<Article> result;

    // find articles with null subcategory field and fill them
    loadOptionalSubCategory(articles, categoryFilter);

    std::copy_if(
        articles.begin(),
        articles.end(),
        std::back_inserter(result),
        [&](const Article & a) {
            if (subCategoryIdFilter.empty()) {
                return categoryFilter.testFlag(a.getCategoryId().value());
            }
            return subCategoryIdFilter.contains(a.subCategoryId);
        });
    return result;
}

QVector<Article> ArticleRepository::GetFilteredArticles(QVector<Article> &articles, const QSet<int> &supplierIdFilter)
{
    QVector<Article> result;

    // const auto nullSupplierCheckerIt = std::find_if(
    //     articles.begin(),
    //     articles.end(),
    //     [](const Article & a) {
    //         return !a.supplier.has_value();
    //     });
    // if (nullSupplierCheckerIt != articles.end())
    // {
    //     const auto suppliersById = mSupplierDao.getById(supplierIdFilter);
    //     for (const auto & s : suppliersById)
    //     {
    //         for (auto & a : articles)
    //         {
    //             if (a.supplierId != s.id) continue;
    //             a.supplier = s;
    //         }
    //     }
    // }

    std::copy_if(
        articles.begin(),
        articles.end(),
        std::back_inserter(result),
        [&](const Article & a) {
            return supplierIdFilter.contains(a.supplierId);
        });
    return result;
}

bool ArticleRepository::Save(Article &article)
{
    bool result = false;
    if (article.id == -1)
    {
        result = static_cast<bool>(mArticleDao.insert(article));
    }
    else
    {
        result = mArticleDao.update(article);
    }
    return result;
}

bool ArticleRepository::Save(QVector<Article> &articles)
{
    bool result = false;
    for (auto & a : articles)
    {
        result = Save(a);
        if (!result) break;
    }
    return result;
}

void ArticleRepository::loadOptionalSubCategory(QVector<Article> &articles, CategoryFlags categoryFilter)
{
    const auto nullSubCatCheckerIt = std::find_if(
        articles.begin(),
        articles.end(),
        [](const Article & a) {
            return !a.subCategory.has_value();
        });
    if (nullSubCatCheckerIt != articles.end())
    {
        QVector<SubCategory> subCatByCat;
        if (categoryFilter == 0) subCatByCat = mSubCategoryDao.getAll();
        else subCatByCat = mSubCategoryDao.getByCategory(categoryFilter);

        for (const auto & sc : std::as_const(subCatByCat))
        {
            for (auto & a : articles)
            {
                if (a.subCategoryId != sc.id) continue;
                a.subCategory = sc;
            }
        }
    }
}

void ArticleRepository::loadOptionalSupplier(QVector<Article> &articles, const QSet<int> &supplierIdFilter)
{
    const auto nullSupplierCheckerIt = std::find_if(
        articles.begin(),
        articles.end(),
        [](const Article & a) {
            return !a.supplier.has_value();
        });
    if (nullSupplierCheckerIt != articles.end())
    {
        QVector<Supplier> suppliersById;
        if (supplierIdFilter.isEmpty()) suppliersById = mSupplierDao.getAll();
        else suppliersById = mSupplierDao.getById(supplierIdFilter);

        for (const auto & s : std::as_const(suppliersById))
        {
            for (auto & a : articles)
            {
                if (a.supplierId != s.id) continue;
                a.supplier = s;
            }
        }
    }
}



