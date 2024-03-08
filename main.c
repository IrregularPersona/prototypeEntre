#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_BRAND_BUFFER 50

struct Warehouse;

int totalOrders = 0;

enum CATEGORY_NAMES
{
    CONSUMER_GOODS = 1,
    RAW_MATERIALS,
    PERISHABLE_GOODS,
    INDUSTRIAL_EQUIPMENT,
    AUTOMOTIVE_PARTS,
    NUM_CATEGORIES
};

enum ORDER_STATES{
    NOT_COMPLETED = 1,
    COMPLETED,
    CANCELLED,
};

// Array of category names
const char *categoryNames[NUM_CATEGORIES] = {
    "Invalid Category",
    "Consumer Goods",
    "Raw Materials",
    "Perishable Goods",
    "Industrial Equipment",
    "Automotive Parts"};

struct Warehouse
{
    int *Part_ID;
    char *Part_Brand;
    int *Part_Category;
    int *Part_Quantity;
};

struct Order
{
    int Order_ID;
    struct Warehouse *Warehouse_Order;
    bool Order_State;
    int Order_Request_Amount;
    size_t numItems;
};

void initWarehouse(struct Warehouse *ware)
{
    ware->Part_ID = (int *)malloc(sizeof(int));
    ware->Part_Category = (int *)malloc(sizeof(int));
    ware->Part_Quantity = (int *)malloc(sizeof(int));
    ware->Part_Brand = (char *)malloc(sizeof(char) * MAX_BRAND_BUFFER);

    if (ware->Part_ID == NULL || ware->Part_Category == NULL || ware->Part_Quantity == NULL || ware->Part_Brand == NULL)
    {
        printf("MEMALLOC FAILED! EXITING PROGRAM\n");
        exit(EXIT_FAILURE);
    }

    *ware->Part_ID = 0;
    *ware->Part_Category = 0;
    *ware->Part_Quantity = 0;
    strcpy(ware->Part_Brand, " ");
}

void initOrders(struct Order* order) {
    *order = (struct Order) {
        .Order_ID = 0,
        .Warehouse_Order = NULL,
        .Order_State = NOT_COMPLETED,
        .Order_Request_Amount = 0,
        .numItems = 0
    };
}

void addItem(struct Warehouse *ware, int itemID, const char *itemBrand, int itemCategory, int itemQuantity)
{
    free(ware->Part_Brand);

    ware->Part_Brand = strdup(itemBrand);

    if (ware->Part_Brand == NULL)
    {
        printf("MEMALLOC FAILED! EXITING PROGRAM\n");
        exit(EXIT_FAILURE);
    }

    *ware->Part_ID = itemID;
    *ware->Part_Category = itemCategory;
    *ware->Part_Quantity = itemQuantity;
}

void viewWarehouse(struct Warehouse *ware[], size_t numItems)
{
    printf("\n\nItems in the warehouse:\n\n");
    printf("No. Item Name\tCategory\tItem Amount\n");
    for (size_t i = 0; i < numItems; i++)
    {
        printf("%zu. %s\t%s   \t%d\n", i + 1, ware[i]->Part_Brand, categoryNames[*(ware[i]->Part_Category)], *(ware[i]->Part_Quantity));
    }
}

struct Warehouse *searchItemID(struct Warehouse *warehouse_array[], size_t numItems, int itemID)
{
    for (size_t i = 0; i < numItems; i++)
    {
        if (*(warehouse_array[i]->Part_ID) == itemID)
        {
            return warehouse_array[i];
        }
    }

    return NULL;
}

ULONGLONG FileTimeToMilliseconds(FILETIME ft)
{
    ULONGLONG ull = ((ULONGLONG)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
    return ull / 10000;
}

void orderItem(struct Warehouse *warehouse_array[], size_t numItems, struct Order *order, int itemId, int itemRequestTotal)
{
    totalOrders++;
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);                        
    ULONGLONG milliseconds = FileTimeToMilliseconds(ft); 
    srand(milliseconds);

    struct Warehouse *item = searchItemID(warehouse_array, numItems, itemId);
    if (item != NULL)
    {

        int randNumID = rand() % 10000 + 1;
        order->Order_ID = randNumID;
        order->Warehouse_Order = item;
        order->Order_Request_Amount = itemRequestTotal;
        order->numItems++;
        order->Order_State = NOT_COMPLETED;

        printf("Your Order ID: %d\n", randNumID);
        puts("Reminder: DO NOT forget your Order ID!");
    }
    else
    {
        printf("Item not found in warehouse.\n");
    }
}

void viewOrders(struct Order* order[], int totalOrders) {
    printf("List of orders:\n");
    printf("No. Order ID\tItem Requested\tTotal Requested\tOrder Status\n");
    for(size_t i = 0; i < totalOrders; i++) {
        printf("%d. %d\t%d\t%d\t%d\n", i + 1, order[i]->Order_ID, order[i]->Order_Request_Amount, order[i]->Order_State);
    }
}

void cancelOrder()
{

}

void changeOrderState()
{
    
}

void changePromotionState()
{
}

void resetPromotionState()
{
}

void freeWarehouseArray(struct Warehouse *ware[], size_t numItems)
{
    for (size_t i = 0; i < numItems; i++)
    {
        free(ware[i]->Part_Brand);
        free(ware[i]);
    }
}

int main()
{
    size_t item_count = 0;
    size_t array_size = 3;

    struct Warehouse **warehouse_array = (struct Warehouse **)malloc(sizeof(struct Warehouse *) * array_size);

    if (warehouse_array == NULL)
    {
        printf("MEMALLOC FAILED! EXITING PROGRAM\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < array_size; i++)
    {
        warehouse_array[i] = (struct Warehouse *)malloc(sizeof(struct Warehouse));
        if (warehouse_array[i] == NULL)
        {
            printf("MEMALLOC FAILED! EXITING PROGRAM\n");
            exit(EXIT_FAILURE);
        }
        initWarehouse(warehouse_array[i]);
    }

    addItem(warehouse_array[0], 1, "Personul", 3, 25);
    addItem(warehouse_array[1], 2, "Actif", 2, 60);
    addItem(warehouse_array[2], 3, "Beyond", 4, 15);

    item_count = array_size;

    viewWarehouse(warehouse_array, item_count);
    array_size = 5;
    struct Warehouse **temp = (struct Warehouse **)realloc(warehouse_array, sizeof(struct Warehouse *) * array_size);

    if (temp == NULL)
    {
        printf("MEMALLOC FAILED! EXITING PROGRAM\n");
        exit(EXIT_FAILURE);
    }

    warehouse_array = temp;

    for (size_t i = item_count; i < array_size; i++)
    {
        warehouse_array[i] = (struct Warehouse *)malloc(sizeof(struct Warehouse));
        if (warehouse_array[i] == NULL)
        {
            printf("MEMALLOC FAILED! EXITING PROGRAM\n");
            exit(EXIT_FAILURE);
        }
        initWarehouse(warehouse_array[i]);
    }

    addItem(warehouse_array[3], 4, "Dynamic1", 1, 30);
    addItem(warehouse_array[4], 5, "Dynamic2", 5, 10);

    item_count = array_size;

    viewWarehouse(warehouse_array, item_count);
    freeWarehouseArray(warehouse_array, item_count);

    return 0;
}