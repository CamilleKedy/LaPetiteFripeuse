#ifndef ARTICLE_H
#define ARTICLE_H

#include <QString>
#include <QDateTime>
#include <QUuid>

class Article
{
public:
    enum class Category {ADORNMENT=0, BAG, COSMETIC, GARMENT, PERFUME, SHOES, OTHER};
    static QSet<QString> subCategories;

    Article(Category cat = Category::OTHER);

    // Common fields
    int id = -1;
    QUuid uuid;
    QString name;
    int quantity;
    double price; // XAF
    double minPrice; // XAF
    Category category;
    int subCategoryId = -1;
    int supplierId = -1;
    QString pic; // [Category]_yymmdd-HHMMSS_uuid
    QString description; // color, pattern, brand, etc.
    QDateTime creationDate;
    QDateTime modificationDate;

    // Flexible fields
    QMap<QString, QVariant> attributes;

    // Helper functions for flex field
    void setAttribute(const QString& key, const QVariant& value);
    QVariant getAttribute(const QString& key, const QVariant& defaultValue = {}) const;
};

class Garment : public Article
{
public:
    QString size; // M, XL, ...
};

class Shoes : public Article
{
public:
    float size; // 40.5, 45, 47, ...
};

class Perfume : public Article
{
public:
    int volume; // mL
};

#endif // ARTICLE_H
