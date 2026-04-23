#include "articlemodel.h"
// #include <QTimer>

ArticleModel::ArticleModel(QSqlDatabase &db, QObject *parent)
    : mArticleRepository(db), QAbstractTableModel{parent}
{
    refresh();
}

int ArticleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mArticles.size();
}

int ArticleModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

QVariant ArticleModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    const auto& a = mArticles[index.row()];

    switch(index.column())
    {
    case IdColumn:              return a.id;
    case PictureColumn:         return a.picPath;
    case NameColumn:            return a.name;
    case DescriptionColumn:     return a.description;
    case PriceColumn:
    {
        auto [lowestPriceVariant, highestPriceVariant] = std::minmax_element(
            a.variants.begin(),
            a.variants.end(),
            [](const ArticleVariant& v1, const ArticleVariant& v2) {
                return v1.price < v2.price;
            });

        QVariantMap map;
        map["lowestPrice"] = -1;
        map["highestPrice"] = -1;
        if (lowestPriceVariant == a.variants.end() || highestPriceVariant == a.variants.end())
            return map;

        map["lowestPrice"] = lowestPriceVariant->price;
        map["highestPrice"] = highestPriceVariant->price;
        return map;
    }
    case MinPriceColumn:
    {
        return (a.variants.empty()) ? -1 : a.variants.front().minPrice;
    }
    case QuantityColumn:
    {
        auto addQuantity = [](int total, const ArticleVariant& variant) {
            return total + variant.quantity;
        };

        return std::accumulate(a.variants.begin(), a.variants.end(), 0, addQuantity);
    }
    case CategoryColumn:        return Category::enumToString(a.getCategoryId().value());
    case SubCategoryColumn:     return a.subCategory.value().name;
    case SupplierColumn:        return a.supplier.value().name;
    case CreationDateColumn:    return a.creationDate.toString("yyyy-MM-dd");
    case ModificationDateColumn:return a.modificationDate.toString("yyyy-MM-dd");
    case ActionColumn:          break;
    }

    return QVariant();
}

QVariant ArticleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
        case IdColumn:                  return "Id";
        case PictureColumn:             return "Pic";
        case NameColumn:                return "Name";
        case DescriptionColumn:         return "Desc";
        case PriceColumn:               return "Price";
        case MinPriceColumn:            return "Min Prc";
        case QuantityColumn:            return "Qty";
        case CategoryColumn:            return "Cat";
        case SubCategoryColumn:         return "Subcat";
        case SupplierColumn:            return "Supplier";
        case CreationDateColumn:        return "Created";
        case ModificationDateColumn:    return "Updated";
        case ActionColumn:              return "";
        }
    }
    return QVariant();
}

void ArticleModel::refresh()
{
    beginResetModel();
    loadArticles();
    endResetModel();
}

void ArticleModel::loadArticles()
{
    mArticles = mArticleRepository.getAllArticles();
}

QList<Article> ArticleModel::getArticles() const
{
    return mArticles;
}

