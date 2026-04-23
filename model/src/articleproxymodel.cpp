#include "articleproxymodel.h"
#include "articlemodel.h"


ArticleProxyModel::ArticleProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    mCollator.setNumericMode(true);
    setDynamicSortFilter(true);
    connect(this, &ArticleProxyModel::categoryComboBoxCurrentValueChanged, &ArticleProxyModel::invalidateFilter);
    connect(this, &ArticleProxyModel::searchTxtFieldChanged, &ArticleProxyModel::invalidateFilter);
}

void ArticleProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    ArticleModel *articleModel = qobject_cast<ArticleModel*>(sourceModel);
    Q_ASSERT_X(articleModel != nullptr, "ArticleProxyModel", "Source model must be an ArticleModel !");
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

void ArticleProxyModel::sort(int column, Qt::SortOrder order)
{
    QSortFilterProxyModel::sort(column, order);
    emit sortColumnChanged();
}

void ArticleProxyModel::setCategoryFilter(const int categoryId)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterKeyColumn(ArticleModel::CategoryColumn);
    if (static_cast<Category::Id>(categoryId) == Category::Id::ALL)
    {
        setFilterFixedString(QString());
        return;
    }
    QString categoryName = Category::enumToString(static_cast<Category::Id>(categoryId));
    setFilterFixedString(categoryName);
}

void ArticleProxyModel::setSearchFilter(const QString &searchTxt)
{
    // setFilterCaseSensitivity(Qt::CaseInsensitive);
    // setFilterKeyColumn(ArticleModel::CategoryColumn);
}

void ArticleProxyModel::updateSorting(const int columnId)
{
    Qt::SortOrder order = (sortColumn() == columnId && sortOrder() == Qt::AscendingOrder)
                ? Qt::DescendingOrder : Qt::AscendingOrder;
    sort(columnId, order);
}

bool ArticleProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    auto srcModel = sourceModel();

    // =====================================================
    // Based on index data wrapping/unwrapping
    // Potentially heavy in terms of ressources consumption
    // =====================================================
    // if (mSearchTxtField.isEmpty())
    // {
    //     if (mCategoryComboBoxCurrentValue == static_cast<int>(Category::Id::ALL))
    //     {
    //         return true;
    //     }
    //     QModelIndex catIndex = srcModel->index(sourceRow, ArticleModel::CategoryColumn, sourceParent);
    //     QString categoryName = Category::enumToString(static_cast<Category::Id>(mCategoryComboBoxCurrentValue));
    //     return catIndex.data().toString() == categoryName;
    // }
    // else
    // {
    //     QModelIndex nameIndex = srcModel->index(sourceRow, ArticleModel::NameColumn, sourceParent);
    //     QModelIndex descIndex = srcModel->index(sourceRow, ArticleModel::DescriptionColumn, sourceParent);
    //     if (mCategoryComboBoxCurrentValue == static_cast<int>(Category::Id::ALL))
    //     {
    //         return nameIndex.data().toString().contains(mSearchTxtField)
    //             || descIndex.data().toString().contains(mSearchTxtField);
    //     }
    //     QModelIndex catIndex = srcModel->index(sourceRow, ArticleModel::CategoryColumn, sourceParent);
    //     QString categoryName = Category::enumToString(static_cast<Category::Id>(mCategoryComboBoxCurrentValue));
    //     return (catIndex.data().toString() == categoryName) && (nameIndex.data().toString().contains(mSearchTxtField)
    //             || descIndex.data().toString().contains(mSearchTxtField));
    // }


    // =====================================================
    // Lighter implementation
    // Direct access to SourceModel internal data structure
    // =====================================================
    ArticleModel* articleModel = qobject_cast<ArticleModel*>(srcModel);
    Q_ASSERT_X(articleModel != nullptr, "ArticleProxyModel", "Source model must be an ArticleModel !");
    const Article& article = articleModel->getArticles().at(sourceRow);
    if (mSearchTxtField.isEmpty())
    {
        if (mCategoryComboBoxCurrentValue == static_cast<int>(Category::Id::ALL))
        {
            return true;
        }
        const auto& articleCategoryId = article.getCategoryId();
        Q_ASSERT_X(articleCategoryId.has_value(), "ArticleProxyModel", "Can't get Article Category !");
        return articleCategoryId == static_cast<Category::Id>(mCategoryComboBoxCurrentValue);
    }
    else
    {
        if (mCategoryComboBoxCurrentValue == static_cast<int>(Category::Id::ALL))
        {
            return article.name.contains(mSearchTxtField) ||
                   article.description.contains(mSearchTxtField);
        }
        const auto& articleCategoryId = article.getCategoryId();
        Q_ASSERT_X(articleCategoryId.has_value(), "ArticleProxyModel", "Can't get Article Category !");
        return articleCategoryId == static_cast<Category::Id>(mCategoryComboBoxCurrentValue) &&
               (article.name.contains(mSearchTxtField) ||
                article.description.contains(mSearchTxtField));
    }
}

bool ArticleProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    QVariant leftData = sourceModel()->data(sourceLeft);
    QVariant rightData = sourceModel()->data(sourceRight);

    switch (sourceLeft.column()) {
    case ArticleModel::IdColumn:
    case ArticleModel::MinPriceColumn:
    case ArticleModel::QuantityColumn:
        return leftData.toInt() < rightData.toInt();
    case ArticleModel::NameColumn:
    case ArticleModel::DescriptionColumn:
    case ArticleModel::CategoryColumn:
    case ArticleModel::SubCategoryColumn:
    case ArticleModel::SupplierColumn:
        return mCollator.compare(leftData.toString(), rightData.toString()) < 0;
    case ArticleModel::CreationDateColumn:
    case ArticleModel::ModificationDateColumn:
        return leftData.toDateTime() < rightData.toDateTime();
    case ArticleModel::PriceColumn:
    {
        QMap<QString, QVariant> leftMap = leftData.toMap();
        QMap<QString, QVariant> rightMap = rightData.toMap();
        return leftMap["lowestPrice"].toInt() < rightMap["lowestPrice"].toInt();
    }
    default:
        return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
    }
}


