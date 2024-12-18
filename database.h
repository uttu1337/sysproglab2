#include <stdio.h>

#define MAX_NAME 50
#define MAX_CLIENT 50
#define MAX_ADDRESS 100
#define MAX_STATUS 20

//Добавление товара
// Структура для продукта
typedef struct {
    int id;
    char name[MAX_NAME];
    int quantity;
} Product;

// Структура для движения товаров
typedef struct {
    int id;
    int product_id;
    char type[10]; // "incoming" или "outgoing"
    int quantity;
    char date[11]; // "YYYY-MM-DD"
} Movement;

// Структура для доставок
typedef struct {
    int id;
    int product_id;
    char client[MAX_CLIENT];
    char address[MAX_ADDRESS];
    char status[MAX_STATUS]; // "pending", "shipped", "delivered"
} Delivery;

void add_product(const char *filename, Product product);

void list_products(const char *filename);


//Движение товара

void add_movement(const char *filename, Movement movement);

void list_movements(const char *filename);


//Доставка товара

void add_delivery(const char *filename, Delivery delivery);

void list_deliveries(const char *filename);

