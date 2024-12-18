#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "database.h" // Подключение функций базы данных

#define PORT 8081
#define BUFFER_SIZE 1024

// Обработчик команд
void handle_request(const char *request, char *response) {
    char command[BUFFER_SIZE];
    char arg1[BUFFER_SIZE], arg2[BUFFER_SIZE], arg3[BUFFER_SIZE], arg4[BUFFER_SIZE], arg5[BUFFER_SIZE];
    int args_count;

    // Парсинг команды и аргументов
    args_count = sscanf(request, "%s %s %s %s", command, arg1, arg2, arg3, arg4, arg5);

    if (strcmp(command, "add_product") == 0 && args_count >= 3) {
        Product product = {atoi(arg1), "", atoi(arg3)};
        strncpy(product.name, arg2, MAX_NAME);
        add_product("/root/sysproglab2/goods.txt", product);
        snprintf(response, BUFFER_SIZE, "Product added: ID=%d, Name=%s, Quantity=%d", product.id, product.name, product.quantity);

    } else if (strcmp(command, "list_products") == 0) {
        FILE *file = fopen("/root/sysproglab2/goods.txt", "rb");
        if (!file) {
            snprintf(response, BUFFER_SIZE, "Failed to open products file.");
        } else {
            Product product;
            strcpy(response, "Products:\n");
            while (fread(&product, sizeof(Product), 1, file)) {
                char line[BUFFER_SIZE];
                snprintf(line, BUFFER_SIZE, "ID=%d, Name=%s, Quantity=%d\n", product.id, product.name, product.quantity);
                strcat(response, line);
            }
            fclose(file);
        }

    } else if (strcmp(command, "add_movement") == 0 && args_count >= 5) {
        Movement movement = {atoi(arg1), atoi(arg2), "", atoi(arg4), ""};
        strncpy(movement.type, arg3, 10);
        strncpy(movement.date, arg5, 11);
        add_movement("/root/sysproglab2/goods_movement.txt", movement);
        snprintf(response, BUFFER_SIZE, "Movement added: ID=%d, ProductID=%d, Type=%s, Quantity=%d, Date=%s",
                 movement.id, movement.product_id, movement.type, movement.quantity, movement.date);

    } else if (strcmp(command, "list_movements") == 0) {
        FILE *file = fopen("/root/sysproglab2/goods_movement.txt", "rb");
        if (!file) {
            snprintf(response, BUFFER_SIZE, "Failed to open movements file.");
        } else {
            Movement movement;
            strcpy(response, "Movements:\n");
            while (fread(&movement, sizeof(Movement), 1, file)) {
                char line[BUFFER_SIZE];
                snprintf(line, BUFFER_SIZE, "ID=%d, ProductID=%d, Type=%s, Quantity=%d, Date=%s\n",
                         movement.id, movement.product_id, movement.type, movement.quantity, movement.date);
                strcat(response, line);
            }
            fclose(file);
        }

    } 
    else if (strcmp(command, "add_delivery") == 0 && args_count >= 5) {
        Delivery delivery = {atoi(arg1), atoi(arg2), "", "", ' '};
        strncpy(delivery.address, arg4, 10);
        strncpy(delivery.client, arg3, 10);
        strncpy(delivery.status, arg5, 11);
        add_delivery("/root/sysproglab2/goods_delivery.txt", delivery);
        snprintf(response, BUFFER_SIZE, "Movement added: ID=%d, ProductID=%d, Type=%s, Quantity=%s, Date=%s",
                 delivery.id, delivery.product_id, delivery.client, delivery.address, delivery.status);

    }
    else if (strcmp(command, "unknown") == 0) {
        snprintf(response, BUFFER_SIZE, "Unknown command: %s", command);

    } else {
        snprintf(response, BUFFER_SIZE, "Invalid or unsupported command.");
    }
}

int main(void) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE];
        
        FILE *goods_file = fopen("/root/sysproglab2/goods.txt", "a+b");
    if (!goods_file) {
        snprintf(response, BUFFER_SIZE, "Error: Unable to open or create goods.txt");
        return;
    }
    fclose(goods_file);

    // Проверка файла goods_movement.txt
    FILE *movement_file = fopen("/root/sysproglab2/goods_movement.txt", "a+b");
    if (!movement_file) {
        snprintf(response, BUFFER_SIZE, "Error: Unable to open or create goods_movement.txt");
        return;
    }
    fclose(movement_file);

    // Проверка файла goods_delivery.txt
    FILE *delivery_file = fopen("/root/sysproglab2/goods_delivery.txt", "a+b");
    if (!delivery_file) {
        snprintf(response, BUFFER_SIZE, "Error: Unable to open or create goods_delivery.txt");
        return;
    }
    fclose(delivery_file);
        
    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Привязка сокета
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Прослушивание порта
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);
    int is_client_connected = 0;
    while (1) {
        // Принятие подключения
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        if (is_client_connected) {
            printf("Another client tried to connect. Rejecting...\n");
            close(new_socket);
            continue;
        }
        printf("Client connected\n");
        is_client_connected = 1;
        
        while (1) {
            // Очистка буфера
            memset(buffer, 0, BUFFER_SIZE);
            memset(response, 0, BUFFER_SIZE);

            // Чтение запроса клиента
            int bytes_read = read(new_socket, buffer, BUFFER_SIZE);
            if (bytes_read <= 0) {
                printf("Client disconnected\n");
                break;
            }

            printf("Received: %s\n", buffer);

            // Обработка запроса
            handle_request(buffer, response);

            // Если клиент отправляет "exit", завершаем соединение
            if (strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "exit\r\n") == 0) {
                printf("Client requested to close the connection.\n");
                break;
            }

            // Отправка ответа клиенту
            send(new_socket, response, strlen(response), 0);
            printf("Response sent: %s\n", response);
        }

        // Закрытие соединения с клиентом
        close(new_socket);
        is_client_connected = 0;
    }

    close(server_fd);
    return 0;
}
