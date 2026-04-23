#include "article.h"

Article::Article(Category cat)
    :category(cat),
    uuid(QUuid::createUuid()),
    quantity(0), price(0), minPrice(0),
    creationDate(QDateTime::currentDateTime()),
    modificationDate(QDateTime::currentDateTime())
{
    switch (cat) {
    case Category::ADORNMENT:
        break;
    case Category::BAG:
        break;
    case Category::COSMETIC:
        break;
    case Category::GARMENT:
        setAttribute("sizeEU", "M");
        break;
    case Category::PERFUME:
        setAttribute("volume", 0);
        break;
    case Category::SHOES:
        setAttribute("size", 0);
        break;
    default:
        break;
    }
}

void Article::setAttribute(const QString &key, const QVariant &value)
{
    attributes[key] = value;
}

QVariant Article::getAttribute(const QString &key, const QVariant& defaultValue) const
{
    return attributes.value(key, defaultValue);
}


