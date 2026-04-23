#include "articlebuilder.h"

ArticleBuilder::ArticleBuilder(Article::Category cat)
{
    article.category = cat;
    article.uuid = QUuid::createUuid();
    article.creationDate = QDateTime::currentDateTime();
    article.modificationDate = article.creationDate;
}

ArticleBuilder& ArticleBuilder::setName(const QString& name) {
    article.name = name;
    return *this;
}

ArticleBuilder& ArticleBuilder::setQuantity(int quantity) {
    article.quantity = quantity;
    return *this;
}

ArticleBuilder& ArticleBuilder::setPrice(int price) {
    article.price = price;
    return *this;
}

ArticleBuilder& ArticleBuilder::setMinPrice(int minPrice) {
    article.minPrice = minPrice;
    return *this;
}

ArticleBuilder& ArticleBuilder::setSubCategory(const QString& subCat) {
    // article.subCategory = subCat;
    return *this;
}

ArticleBuilder& ArticleBuilder::setSupplier(const QString& supplier) {
    // article.supplier = supplier;
    return *this;
}

ArticleBuilder& ArticleBuilder::setPic(const QString& pic) {
    article.pic = pic;
    return *this;
}

ArticleBuilder& ArticleBuilder::setDescription(const QString& desc) {
    article.description = desc;
    return *this;
}

ArticleBuilder& ArticleBuilder::setAttribute(const QString& key, const QVariant& value) {
    article.setAttribute(key, value);
    return *this;
}

Article ArticleBuilder::build() {
    article.modificationDate = QDateTime::currentDateTime();
    return article;
}

