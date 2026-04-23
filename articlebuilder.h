#ifndef ARTICLEBUILDER_H
#define ARTICLEBUILDER_H

#include "article.h"

class ArticleBuilder
{
public:
    ArticleBuilder(Article::Category cat = Article::Category::OTHER);

    ArticleBuilder& setName(const QString& name);
    ArticleBuilder& setQuantity(int quantity);
    ArticleBuilder& setPrice(int price);
    ArticleBuilder& setMinPrice(int minPrice);
    ArticleBuilder& setSubCategory(const QString& subCat);
    ArticleBuilder& setSupplier(const QString& supplier);
    ArticleBuilder& setPic(const QString& pic);
    ArticleBuilder& setDescription(const QString& desc);
    ArticleBuilder& setAttribute(const QString& key, const QVariant& value);
    Article build();

private:
    Article article;
};

#endif // ARTICLEBUILDER_H
