#ifndef ARTICLEMODEL_H
#define ARTICLEMODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSortFilterProxyModel>

#include "articlerepository.h"


// class Article;

class ArticleModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ArticleModel(QSqlDatabase &db, QObject *parent = nullptr);

    enum Columns {
        IdColumn = 0,
        PictureColumn,
        NameColumn,
        DescriptionColumn,
        PriceColumn,
        MinPriceColumn,
        QuantityColumn,
        CategoryColumn,
        SubCategoryColumn,
        SupplierColumn,
        CreationDateColumn,
        ModificationDateColumn,
        ActionColumn, // for UI Items (Buttons, checkbox, ...)
        COLUMN_COUNT
    };
    Q_ENUM(Columns)

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    // QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Q_INVOKABLE void refresh();
    // Q_INVOKABLE void removeAt(int index);
    // Q_INVOKABLE QVariantMap get(int index) const;
    // Q_INVOKABLE Article getArticle(int index) const;

    void loadArticles();

    QList<Article> getArticles() const;

private:
    QList<Article> mArticles;
    ArticleRepository mArticleRepository;
};

#endif // ARTICLEMODEL_H
