#ifndef ARTICLEPROXYMODEL_H
#define ARTICLEPROXYMODEL_H

#include "sqlsession.h"
#include <QCollator>
#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSortFilterProxyModel>

class ArticleProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ArticleProxyModel(QObject *parent = nullptr);

    void setSourceModel(QAbstractItemModel *sourceModel) override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    Q_INVOKABLE void setCategoryFilter(const int categoryId);
    Q_INVOKABLE void setSearchFilter(const QString &searchTxt);
    Q_INVOKABLE void updateSorting(const int columnId);
    Q_PROPERTY(int categoryComboBoxCurrentValue MEMBER mCategoryComboBoxCurrentValue NOTIFY categoryComboBoxCurrentValueChanged FINAL)
    Q_PROPERTY(QString searchTxtField MEMBER mSearchTxtField NOTIFY searchTxtFieldChanged FINAL)
    Q_PROPERTY(int sortColumn READ sortColumn NOTIFY sortColumnChanged FINAL)
    Q_PROPERTY(int sortOrder READ sortOrder NOTIFY sortOrderChanged FINAL)

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

signals:
    void categoryComboBoxCurrentValueChanged();
    void searchTxtFieldChanged();
    void sortColumnChanged();
    void sortOrderChanged();

private:
    QCollator mCollator;
    int mCategoryComboBoxCurrentValue = static_cast<int>(Category::Id::ALL);
    QString mSearchTxtField;
};

#endif // ARTICLEPROXYMODEL_H
