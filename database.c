#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
// Максимальные размеры строк




//Добавление продукта

void add_product(const char *filename, Product product) {
    FILE *file = fopen(filename, "ab"); // Открытие файла для добавления (бинарный режим)
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fwrite(&product, sizeof(Product), 1, file); // Запись структуры в файл
    fclose(file);
}

void list_products(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Открытие файла для чтения (бинарный режим)
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    Product product;
    printf("ID\tName\t\tQuantity\n");
    printf("--------------------------------\n");
    while (fread(&product, sizeof(Product), 1, file)) {
        printf("%d\t%s\t\t%d\n", product.id, product.name, product.quantity);
    }

    fclose(file);
}


//Движение товара

void add_movement(const char *filename, Movement movement) {
    FILE *file = fopen(filename, "ab");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fwrite(&movement, sizeof(Movement), 1, file);
    fclose(file);
}

void list_movements(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    Movement movement;
    printf("ID\tProduct ID\tType\t\tQuantity\tDate\n");
    printf("------------------------------------------------------------\n");
    while (fread(&movement, sizeof(Movement), 1, file)) {
        printf("%d\t%d\t\t%s\t\t%d\t\t%s\n", movement.id, movement.product_id, movement.type, movement.quantity, movement.date);
    }

    fclose(file);
}


//Доставка товара

void add_delivery(const char *filename, Delivery delivery) {
    FILE *file = fopen(filename, "ab");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fwrite(&delivery, sizeof(Delivery), 1, file);
    fclose(file);
}

void list_deliveries(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    Delivery delivery;
    printf("ID\tProduct ID\tClient\t\tAddress\t\t\tStatus\n");
    printf("----------------------------------------------------------------------\n");
    while (fread(&delivery, sizeof(Delivery), 1, file)) {
        printf("%d\t%d\t\t%s\t\t%s\t\t%s\n", delivery.id, delivery.product_id, delivery.client, delivery.address, delivery.status);
    }

    fclose(file);
}

