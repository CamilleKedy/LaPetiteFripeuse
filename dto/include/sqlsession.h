#ifndef SQLSESSION_H
#define SQLSESSION_H

#include <QString>
#include <QDateTime>
#include <QUuid>

class Category
{
    Q_GADGET
public:
    enum class Id : int {
        UNKNOWN = -1, // Default value. Shouldn't be available in App UI
        ADORNMENT,
        BAG,
        COSMETIC,
        GARMENT,
        PERFUME,
        SHOES,
        ALL // UI purpose (e.g ComboBox model)
    };
    Q_ENUM(Id)

    static Id stringToEnum(const QString& name);
    static QString enumToString(Id catEnum);
    Q_INVOKABLE static const QVariantList getList();
};
Q_DECLARE_FLAGS(CategoryFlags, Category::Id)
Q_DECLARE_OPERATORS_FOR_FLAGS(CategoryFlags)

struct SubCategory
{
    int id = -1;
    Category::Id categoryId = Category::Id::UNKNOWN;
    QString name;
};

struct Supplier
{
    int id = -1;
    QString name;
};

struct ArticleVariant
{
    enum class Attribute
    {
        color,
        material,
        scent,
        sizeAlpha,
        sizeNumeric,
        volume
    };

    ArticleVariant();

    int id = -1;
    int articleId = -1;
    QUuid uuid;
    int price = 0;
    int minPrice = 0;
    int quantity = 0;

    // Optional fields
    QHash<Attribute, QVariant> attributes;

    QVariant getSpecificAttribute(Attribute attr) const;

    static QString attributeEnumToString(Attribute attr);
    static std::optional<Attribute> attributeStringToEnum(const QString& str);

    /**
     * @brief Converts the C++ QHash<Attribute, QVariant> into a JSON QByteArray for DB storage.
     * @param attributes The hash to serialize.
     * @return The serialized JSON data.
     */
    static QByteArray toJson(const QHash<Attribute, QVariant>& attributes);

    /**
     * @brief Converts a JSON QByteArray from the DB back into a QHash<Attribute, QVariant>.
     * @param json The JSON data to deserialize.
     * @return The resulting attribute hash.
     */
    static QHash<Attribute, QVariant> fromJson(const QByteArray& json);
};

struct Article
{
    Article(Category::Id catId = Category::Id::UNKNOWN);

    int id = -1;
    QUuid uuid;
    QString name;
    QString description; // any note (color, pattern, brand, etc.)
    QDateTime creationDate;
    QDateTime modificationDate;
    int subCategoryId = -1;
    int supplierId = -1;

    std::optional<SubCategory> subCategory;
    std::optional<Supplier> supplier;

    QVector<ArticleVariant> variants;
    QString picPath; // [uuid].jpeg; Not a database field

    // Helper function to get category corresponding Enum
    std::optional<Category::Id> getCategoryId() const;
};

struct SaleArticleVariant
{
    int saleId = -1;
    int variantId = -1;
    int quantity = 0;
    int priceAtSale = 0;

    // Helper fields (Not in this table, but usually needed for UI/Receipts)
    // These would be populated via a JOIN query
    QString _cachedArticleName;
    QString _cachedVariantUuid;
};

struct Sale
{
    int id = -1;
    int totalPrice = 0; // Calculated by Trigger
    int sellingPrice = 0; // Calculated by Trigger or manual override
    bool isEdited = false;
    QDateTime sellingDate;
    bool isOnlineSale = false;

    QVector<SaleArticleVariant> items;
};

#endif // SQLSESSION_H
