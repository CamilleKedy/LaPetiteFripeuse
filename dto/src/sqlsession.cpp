#include "sqlsession.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QMetaEnum>

Category::Id Category::stringToEnum(const QString &name)
{
    bool ok;
    int value = QMetaEnum::fromType<Id>().keyToValue(name.toUpper().toUtf8().constData(), &ok);

    if (ok) return static_cast<Id>(value);

    return Id::UNKNOWN;
}

QString Category::enumToString(Id catEnum)
{
    const char* key = QMetaEnum::fromType<Id>().valueToKey(static_cast<int>(catEnum));
    return key ? QString::fromUtf8(key) : QStringLiteral("UNKNOWN");
}

const QVariantList Category::getList()
{
    QVariantList categoryList;
    QMetaEnum me = QMetaEnum::fromType<Id>();
    for (int i = 0; i < me.keyCount(); i++)
    {
        QVariantMap item;
        item["text"] = QString(me.key(i));
        item["value"] = me.value(i);
        if (item["value"] == static_cast<int>(Id::UNKNOWN))
            continue; // We don't want UNKNOWN to be available in UI
        if (item["value"] == static_cast<int>(Id::ALL))
        {
            item["text"] = QStringLiteral("----");
            categoryList.push_front(item);
            continue;
        }
        categoryList.append(item);
    }
    return categoryList;
}

Article::Article(Category::Id catId)
    : uuid(QUuid::createUuid()) {}

ArticleVariant::ArticleVariant()
    : uuid(QUuid::createUuid()) {}

QString ArticleVariant::attributeEnumToString(Attribute attr)
{
    switch (attr)
    {
    case ArticleVariant::Attribute::color: return "color";
    case ArticleVariant::Attribute::material: return "material";
    case ArticleVariant::Attribute::scent: return "scent";
    case ArticleVariant::Attribute::sizeAlpha: return "sizeAlphabetical";
    case ArticleVariant::Attribute::sizeNumeric: return "sizeNumeric";
    case ArticleVariant::Attribute::volume: return "volume";
    }
    return QString();
}

std::optional<ArticleVariant::Attribute> ArticleVariant::attributeStringToEnum(const QString &str)
{
    if (str == "color") return ArticleVariant::Attribute::color;
    if (str == "material") return ArticleVariant::Attribute::material;
    if (str == "scent") return ArticleVariant::Attribute::scent;
    if (str == "sizeAlpha") return ArticleVariant::Attribute::sizeAlpha;
    if (str == "sizeNumeric") return ArticleVariant::Attribute::sizeNumeric;
    if (str == "volume") return ArticleVariant::Attribute::volume;
    return std::nullopt;
}

QByteArray ArticleVariant::toJson(const QHash<Attribute, QVariant> &attributes)
{
    QJsonObject obj;
    for (auto it = attributes.cbegin(); it != attributes.cend(); ++it)
    {
        QString key = attributeEnumToString(it.key());
        if (!key.isEmpty())
        {
            obj.insert(key, QJsonValue::fromVariant(it.value()));
        }
    }
    if (obj.isEmpty())
    {
        return QByteArray();
    }
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

QHash<ArticleVariant::Attribute, QVariant> ArticleVariant::fromJson(const QByteArray &json)
{
    QHash<ArticleVariant::Attribute, QVariant> attributes;
    if (json.isEmpty())
    {
        return attributes;
    }

    QJsonDocument doc = QJsonDocument::fromJson(json);
    if (doc.isNull() || !doc.isObject())
    {
        qWarning() << "Failed to deserialize ArticleVariant attributes from JSON:" << json;
        return attributes;
    }

    QJsonObject obj = doc.object();
    for (auto it = obj.constBegin(); it != obj.constEnd(); ++it)
    {
        std::optional<ArticleVariant::Attribute> attrOpt = attributeStringToEnum(it.key());
        if (attrOpt.has_value())
        {
            attributes.insert(attrOpt.value(), it.value().toVariant());
        }
        else
        {
            qWarning() << "Unknown attribute key during deserialization:" << it.key();
        }
    }
    return attributes;
}

std::optional<Category::Id> Article::getCategoryId() const
{
    if (subCategory.has_value())
    {
        return subCategory.value().categoryId;
    }
    return std::nullopt;
}
