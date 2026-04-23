PRAGMA foreign_keys = ON;
-- ;;

-- =========================
-- Insert SubCategories
-- =========================
-- Adornment
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (0, 'Bracelet'),
  (0, 'Necklace'),
  (0, 'Earrings'),
  (0, 'Ring'),
  (0, 'Sets'),
  (0, 'Brooch'),
  (0, 'Belt'),
  (0, 'Hat'),
  (0, 'Bibi'),
  (0, 'Silk scarf/square');
-- ;;

-- Bag
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (1, 'Handbag');
-- ;;

-- Cosmetic
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (2, 'Cleansing milk'),
  (2, 'Soap'),
  (2, 'Mask'),
  (2, 'Cream'),
  (2, 'Gloss'),
  (2, 'Lip balm');
-- ;;

-- Garment
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (3, 'Regular outfit'),
  (3, 'Suited outfit'),
  (3, 'Dress'),
  (3, 'Top'),
  (3, 'Skirt'),
  (3, 'Pants'),
  (3, 'Lingerie'),
  (3, 'Bodysuit'),
  (3, 'Nightdress');
-- ;;

-- Perfume
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (4, 'Roll-on'),
  (4, 'Deodorant'),
  (4, 'Eau de Parfum'),
  (4, 'Mist');
-- ;;

-- Shoes
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (5, 'Sandals'),
  (5, 'Ballerinas'),
  (5, 'Pumps'),
  (5, 'Mules');
-- ;;

-- Other
-- ;;
INSERT INTO SubCategory (category_id, name) VALUES
  (-1, 'Other');
-- ;;

-- =========================
-- Insert Suppliers
-- =========================
-- ;;
INSERT INTO Supplier (name) VALUES
  ('Elegant Adornments Co.'),
  ('Urban Bags Ltd.'),
  ('Natural Beauty Cosmetics'),
  ('Fashion Garments Intl.'),
  ('Luxury Perfumes House'),
  ('ShoeWorld Inc.'),
  ('Miscellaneous Supplier');
-- ;;

-- =========================
-- Insert Articles
-- =========================
-- ;;
INSERT INTO Article (uuid, name, description, supplier_id, subcategory_id) VALUES
  ('00000000-0000-0000-0000-000000000001', 'Golden Chain Bracelet',   '18k gold plated bracelet',     1, 1),
  ('00000000-0000-0000-0000-000000000002', 'Pearl Necklace',          'Freshwater pearl necklace',    1, 2),
  ('00000000-0000-0000-0000-000000000003', 'Leather Tote Bag',        'Genuine leather everyday bag', 2, 11),
  ('00000000-0000-0000-0000-000000000004', 'Organic Cleansing Milk',  'Aloe vera based cleanser',     3, 12),
  ('00000000-0000-0000-0000-000000000005', 'Silk Evening Dress',      'Floor length silk dress',      4, 20),
  ('00000000-0000-0000-0000-000000000006', 'Cotton Summer Top',       'Breathable cotton top',        4, 21),
  ('00000000-0000-0000-0000-000000000007', 'Midnight Rose Perfume',   '50ml Eau de Parfum',           5, 29),
  ('00000000-0000-0000-0000-000000000008', 'Leather Sandals',         'Handcrafted summer sandals',   6, 31),
  ('00000000-0000-0000-0000-000000000009', 'Silver Hoop Earrings',    'Sterling silver hoops',        1, 3),
  ('00000000-0000-0000-0000-000000000010', 'Velvet Evening Bag',      'Small clutch for events',      2, 11),
  ('00000000-0000-0000-0000-000000000011', 'Hydrating Night Cream',   'Deep repair cream',            3, 15),
  ('00000000-0000-0000-0000-000000000012', 'Slim Fit Denim',          'High-waist blue jeans',        4, 23),
  ('00000000-0000-0000-0000-000000000013', 'Citrus Mist',             'Refreshing body mist',         5, 30),
  ('00000000-0000-0000-0000-000000000014', 'Classic Black Pumps',     'Professional office shoes',    6, 33),
  ('00000000-0000-0000-0000-000000000015', 'Diamond Solitaire Ring',  '0.5 carat diamond ring',       1, 4),
  ('00000000-0000-0000-0000-000000000016', 'Lace Lingerie Set',       'Two-piece lace set',           4, 24),
  ('00000000-0000-0000-0000-000000000017', 'Shea Butter Soap',        'Handmade natural soap',        3, 13),
  ('00000000-0000-0000-0000-000000000018', 'Wool Fedora Hat',         'Classic winter accessory',     1, 8),
  ('00000000-0000-0000-0000-000000000019', 'Lavender Deodorant',      'Aluminum-free stick',          5, 28),
  ('00000000-0000-0000-0000-000000000020', 'Canvas Weekender',        'Large travel bag',             2, 11);
-- ;;

-- =========================
-- Insert ArticleVariants (SKUs)
-- =========================
-- Note: Prices are in XAF
-- ;;
INSERT INTO ArticleVariant (article_id, uuid, price, min_price, quantity, attributes) VALUES
  (1,  '00000000-0000-0000-0000-000000000001', 5000,   4000,    10, '{"color": "gold"}'),
  (1,  '00000000-0000-0000-0000-000000000002', 4500,   4000,    10, '{"color": "pink gold"}'),
  (2,  '00000000-0000-0000-0000-000000000003', 8500,   7500,    20, '{"color": "white"}'),
  (3,  '00000000-0000-0000-0000-000000000004', 12000,  10000,   15, '{"color": "black"}'),
  (3,  '00000000-0000-0000-0000-000000000005', 12000,  10000,   5,  '{"color": "white"}'),
  (3,  '00000000-0000-0000-0000-000000000006', 12000,  10000,   10, '{"color": "blue"}'),
  (4,  '00000000-0000-0000-0000-000000000007', 1800,   1500,    12, '{"sizeNumeric": "200ml"}'),
  (5,  '00000000-0000-0000-0000-000000000008', 21000,  20000,   5,  '{"color": "red", "sizeAlpha": "S"}'),
  (5,  '00000000-0000-0000-0000-000000000009', 25000,  20000,   8,  '{"color": "red", "sizeAlpha": "M"}'),
  (5,  '00000000-0000-0000-0000-000000000010', 28000,  20000,   1,  '{"color": "black", "sizeAlpha": "L"}'),
  (5,  '00000000-0000-0000-0000-000000000011', 35000,  20000,   2,  '{"color": "black", "sizeAlpha": "XL"}'),
  (5,  '00000000-0000-0000-0000-000000000012', 25000,  20000,   9,  '{"color": "yellow", "sizeAlpha": "M"}'),
  (5,  '00000000-0000-0000-0000-000000000013', 25000,  20000,   7,  '{"color": "blue", "sizeAlpha": "M"}'),
  (6,  '00000000-0000-0000-0000-000000000014', 3500,   3000,    6,  '{"color": "white", "sizeAlpha": "L"}'),
  (6,  '00000000-0000-0000-0000-000000000015', 4000,   3000,    4,  '{"color": "green", "sizeAlpha": "M"}'),
  (7,  '00000000-0000-0000-0000-000000000016', 9500,   9000,    25, '{"volume": "50ml"}'),
  (8,  '00000000-0000-0000-0000-000000000017', 5500,   5000,    12, '{"color": "brown", "sizeNumeric": 38}'),
  (9,  '00000000-0000-0000-0000-000000000018', 3000,   2500,    16, '{"material": "silver"}'),
  (10, '00000000-0000-0000-0000-000000000019', 7500,   6500,    10, '{"material": "velvet"}'),
  (11, '00000000-0000-0000-0000-000000000020', 4200,   3800,    30, '{"sizeNumeric": "50ml"}'),
  (12, '00000000-0000-0000-0000-000000000021', 6500,   5500,    20, '{"color": "blue"}'),
  (13, '00000000-0000-0000-0000-000000000022', 1500,   1200,    22, '{"scent": "citrus"}'),
  (14, '00000000-0000-0000-0000-000000000023', 8900,   8000,    15, '{"color": "black", "sizeNumeric": 39}'),
  (15, '00000000-0000-0000-0000-000000000024', 150000, 140000,  3,  NULL),
  (16, '00000000-0000-0000-0000-000000000025', 4500,   4000,    13, '{"sizeAlpha": "M"}'),
  (17, '00000000-0000-0000-0000-000000000026', 800,    600,     15, '{"scent": "shea"}'),
  (18, '00000000-0000-0000-0000-000000000027', 5000,   4500,    10, '{"color": "grey"}'),
  (19, '00000000-0000-0000-0000-000000000028', 1200,   1000,    11, '{"scent": "lavender"}'),
  (20, '00000000-0000-0000-0000-000000000029', 9500,   8500,    18, '{"material": "canvas"}');
-- ;;

-- =========================
-- Insert Sales
-- =========================;
-- We initialize these with 0; the triggers will calculate totals based on items added.
-- ;;
INSERT INTO Sale (total_price, selling_price, is_online_sale) VALUES
  (0, 0, 1),
  (0, 0, 0),
  (0, 0, 0);
-- ;;

-- =========================
-- Insert SaleArticle Relations
-- =========================;
-- Sale 1: Online purchase of a dress and a perfume
-- ;;
INSERT INTO SaleArticleVariant (sale_id, variant_id, quantity, price_at_sale) VALUES
  (1, 5, 1, 15000),
  (1, 8, 1, 21000);
-- ;;

-- Sale 2: In-store purchase of multiple soaps and a mist
-- ;;
INSERT INTO SaleArticleVariant (sale_id, variant_id, quantity, price_at_sale) VALUES
  (2, 18, 5, 3500),
  (2, 14, 1, 5000);
-- ;;

-- Sale 3: Purchase of a ring
-- ;;
INSERT INTO SaleArticleVariant (sale_id, variant_id, quantity, price_at_sale) VALUES
  (3, 24, 1, 150000);
-- ;;
