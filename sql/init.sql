PRAGMA foreign_keys = ON;
-- ;;

-- =====================
-- Category(Hardcoded C++ Enum) & Subcategory
-- =====================
-- ;;
CREATE TABLE IF NOT EXISTS SubCategory (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   category_id INTEGER NOT NULL DEFAULT -1,
   name VARCHAR(128) NOT NULL,
   -- -1:unknown, 0:adornment, 1:bag, 2:cosmetic, 3:garment, 4:perfume, 5:shoes
   CHECK (category_id BETWEEN -1 AND 5),
   UNIQUE(category_id, name) -- prevent duplicate names in the same category
);
-- ;;

-- ============
-- Sale
-- ============
-- ;;
CREATE TABLE IF NOT EXISTS Sale (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   total_price INTEGER NOT NULL,    -- pre-discount price
   selling_price INTEGER NOT NULL,  -- final price if any article price modified at last minute during sale
   is_edited BOOLEAN NOT NULL CHECK (is_edited IN (0,1)) DEFAULT 0, -- true if article price modified during sale
   selling_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
   is_online_sale BOOLEAN NOT NULL CHECK (is_online_sale IN (0,1)) DEFAULT 0
);
-- ;;

-- ============
-- Supplier
-- ============
-- ;;
CREATE TABLE IF NOT EXISTS Supplier (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   name VARCHAR(128) NOT NULL UNIQUE
);
-- ;;

-- ============
-- Articles (Model)
-- ============
-- ;;
CREATE TABLE IF NOT EXISTS Article (
   id INTEGER PRIMARY KEY AUTOINCREMENT,
   uuid VARCHAR(128) NOT NULL UNIQUE CHECK (length(uuid) > 0),
   name VARCHAR(128),
   description TEXT,
   creation_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
   modification_date DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
   supplier_id INTEGER,
   subcategory_id INTEGER NOT NULL,
   FOREIGN KEY(supplier_id) REFERENCES Supplier(id)
       ON DELETE SET NULL
       ON UPDATE CASCADE,
   FOREIGN KEY(subcategory_id) REFERENCES SubCategory(id)
       ON DELETE CASCADE
       ON UPDATE CASCADE
);
-- ;;

-- ============
-- ArticleVariant (SKU)
-- ============
-- ;;
CREATE TABLE IF NOT EXISTS ArticleVariant (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    article_id INTEGER NOT NULL,
    uuid VARCHAR(128) NOT NULL UNIQUE CHECK (length(uuid) > 0),   -- unique SKU code
    price INTEGER NOT NULL CHECK (price >= 0),                     -- current price
    min_price INTEGER NOT NULL CHECK (min_price >= 0),             -- min sale price recommended
    quantity INTEGER NOT NULL CHECK (quantity >= 0),
    attributes TEXT, -- e.g. garmentSize, perfumeVolume, shoeSize, etc.
    FOREIGN KEY(article_id) REFERENCES Article(id)
      ON DELETE CASCADE
      ON UPDATE CASCADE
);
-- ;;

-- ============
-- Sale <-> Article SKU relation
-- ============
-- ;;
CREATE TABLE IF NOT EXISTS SaleArticleVariant (
    sale_id INTEGER NOT NULL,
    variant_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL CHECK (quantity > 0),
    price_at_sale INTEGER NOT NULL CHECK (price_at_sale >= 0),
    PRIMARY KEY(sale_id, variant_id),
    FOREIGN KEY(sale_id) REFERENCES Sale(id)
      ON DELETE CASCADE
      ON UPDATE CASCADE,
    FOREIGN KEY(variant_id) REFERENCES ArticleVariant(id)
      ON DELETE CASCADE
      ON UPDATE CASCADE
);
-- ;;


-- ============
-- View
-- ============
-- ;;
CREATE VIEW IF NOT EXISTS V_SaleReceipt AS
SELECT
    sav.sale_id,
    sav.variant_id,
    sav.quantity,
    sav.price_at_sale,
    a.name AS article_name,
    av.uuid AS variant_uuid,
    av.attributes
FROM SaleArticleVariant sav
JOIN ArticleVariant av ON sav.variant_id = av.id
JOIN Article a ON av.article_id = a.id;
-- ;;


-- ============
-- Indexes
-- ============
-- ;;
CREATE INDEX IF NOT EXISTS idx_article_subcategory ON Article(subcategory_id);
-- ;;
CREATE INDEX IF NOT EXISTS idx_article_supplier ON Article(supplier_id);
-- ;;
CREATE INDEX IF NOT EXISTS idx_salearticlevariant_article ON SaleArticleVariant(variant_id);
-- ;;
CREATE INDEX IF NOT EXISTS idx_salearticlevariant_sale ON SaleArticleVariant(sale_id);
-- ;;
CREATE INDEX IF NOT EXISTS idx_sale_date ON Sale(selling_date);
-- ;;
CREATE INDEX IF NOT EXISTS idx_articlevariant_uuid ON ArticleVariant(uuid);
-- ;;
CREATE INDEX IF NOT EXISTS idx_articlevariant_article ON ArticleVariant(article_id);
-- ;;


-- ============
-- Triggers
-- ============;

-- Trigger to prevent negative stock
-- ;;
CREATE TRIGGER IF NOT EXISTS trg_prevent_negative_stock
BEFORE INSERT ON SaleArticleVariant
FOR EACH ROW
BEGIN
    -- Check if enough stock exists
    SELECT
    CASE
        WHEN (SELECT quantity FROM ArticleVariant WHERE id = NEW.variant_id) < NEW.quantity
        THEN RAISE(ABORT, 'Insufficient stock for this variant')
    END;
END;
-- ;;



-- Triggers to update SKU qty;
-- ;;
CREATE TRIGGER IF NOT EXISTS trg_decrease_stock_on_sale
AFTER INSERT ON SaleArticleVariant
FOR EACH ROW
BEGIN
    UPDATE ArticleVariant
    SET quantity = quantity - NEW.quantity
    WHERE id = NEW.variant_id;
END;
-- ;;

CREATE TRIGGER IF NOT EXISTS trg_adjust_stock_on_sale_update
AFTER UPDATE ON SaleArticleVariant
FOR EACH ROW
BEGIN
    -- Return the old quantity
    UPDATE ArticleVariant
    SET quantity = quantity + OLD.quantity
    WHERE id = OLD.variant_id;

    -- Apply the new quantity
    UPDATE ArticleVariant
    SET quantity = quantity - NEW.quantity
    WHERE id = NEW.variant_id;
END;
-- ;;

CREATE TRIGGER IF NOT EXISTS trg_restore_stock_on_sale_delete
AFTER DELETE ON SaleArticleVariant
FOR EACH ROW
BEGIN
    UPDATE ArticleVariant
    SET quantity = quantity + OLD.quantity
    WHERE id = OLD.variant_id;
END;
-- ;;



-- Triggers to update modification date;
-- ;;
CREATE TRIGGER IF NOT EXISTS trg_article_touch_on_variant_insert
AFTER INSERT ON ArticleVariant
FOR EACH ROW
BEGIN
    UPDATE Article
    SET modification_date = CURRENT_TIMESTAMP
    WHERE id = NEW.article_id;
END;
-- ;;

CREATE TRIGGER IF NOT EXISTS trg_article_touch_on_variant_update
AFTER UPDATE ON ArticleVariant
FOR EACH ROW
BEGIN
    UPDATE Article
    SET modification_date = CURRENT_TIMESTAMP
    WHERE id = NEW.article_id;
END;
-- ;;

-- in case variant is reparented;
-- ;;
CREATE TRIGGER IF NOT EXISTS trg_article_touch_on_variant_reparent
AFTER UPDATE OF article_id ON ArticleVariant
FOR EACH ROW
BEGIN
    -- Update old parent article
    UPDATE Article
    SET modification_date = CURRENT_TIMESTAMP
    WHERE id = OLD.article_id;

    -- Update new parent article
    UPDATE Article
    SET modification_date = CURRENT_TIMESTAMP
    WHERE id = NEW.article_id;
END;
-- ;;


-- Triggers to auto-calculate sale total price (and occasionally selling price);
-- ;;
CREATE TRIGGER IF NOT EXISTS trg_calculate_sale_totals_delete
AFTER DELETE ON SaleArticleVariant
FOR EACH ROW
BEGIN
    UPDATE Sale
    SET
        -- 1. Recalculate the theoretical total (Sum of items)
        total_price = (
            SELECT COALESCE(SUM(quantity * price_at_sale), 0)
            FROM SaleArticleVariant
            WHERE sale_id = OLD.sale_id
        ),
        -- 2. Update selling_price ONLY if the sale wasn't manually edited
        selling_price = CASE
            WHEN is_edited = 0 THEN (
                SELECT COALESCE(SUM(quantity * price_at_sale), 0)
                FROM SaleArticleVariant
                WHERE sale_id = OLD.sale_id
            )
            ELSE selling_price -- Keep the custom price if manually edited
        END
    WHERE id = OLD.sale_id;
END;
-- ;;

CREATE TRIGGER IF NOT EXISTS trg_calculate_sale_totals_update
AFTER UPDATE ON SaleArticleVariant
FOR EACH ROW
BEGIN
    -- Part A: Update the NEW Sale ID (The one receiving the change)
    UPDATE Sale
    SET
        total_price = (
            SELECT COALESCE(SUM(quantity * price_at_sale), 0)
            FROM SaleArticleVariant
            WHERE sale_id = NEW.sale_id
        ),
        selling_price = CASE
            WHEN is_edited = 0 THEN (
                SELECT COALESCE(SUM(quantity * price_at_sale), 0)
                FROM SaleArticleVariant
                WHERE sale_id = NEW.sale_id
            )
            ELSE selling_price
        END
    WHERE id = NEW.sale_id;

    -- Part B: Update the OLD Sale ID (Only runs if the sale_id actually changed)
    UPDATE Sale
    SET
        total_price = (
            SELECT COALESCE(SUM(quantity * price_at_sale), 0)
            FROM SaleArticleVariant
            WHERE sale_id = OLD.sale_id
        ),
        selling_price = CASE
            WHEN is_edited = 0 THEN (
                SELECT COALESCE(SUM(quantity * price_at_sale), 0)
                FROM SaleArticleVariant
                WHERE sale_id = OLD.sale_id
            )
            ELSE selling_price
        END
    WHERE id = OLD.sale_id
      AND OLD.sale_id != NEW.sale_id; -- Optimization: only run if ID changed
END;
-- ;;

CREATE TRIGGER IF NOT EXISTS trg_calculate_sale_totals_insert
AFTER INSERT ON SaleArticleVariant
FOR EACH ROW
BEGIN
    UPDATE Sale
    SET
        total_price = (
            SELECT COALESCE(SUM(quantity * price_at_sale), 0)
            FROM SaleArticleVariant
            WHERE sale_id = NEW.sale_id
        ),
        selling_price = CASE
            WHEN is_edited = 0 THEN (
                SELECT COALESCE(SUM(quantity * price_at_sale), 0)
                FROM SaleArticleVariant
                WHERE sale_id = NEW.sale_id
            )
            ELSE selling_price
        END
    WHERE id = NEW.sale_id;
END;
-- ;;
