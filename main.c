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

enum ORDER_STATES
{
    NOT_COMPLETED = 1,
    COMPLETED,
    CANCELLED,
};

enum RUNNING_STATES
{
    NOT_RUNNING = 0,
    RUNNING,
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
    double *Part_Pricing;
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
    ware->Part_Pricing = (double *)malloc(sizeof(double));

    if (ware->Part_ID == NULL || ware->Part_Category == NULL || ware->Part_Quantity == NULL || ware->Part_Brand == NULL || ware->Part_Pricing == NULL)
    {
        printf("MEMALLOC FAILED! EXITING PROGRAM\n");
        exit(EXIT_FAILURE);
    }

    *ware->Part_ID = -1;
    *ware->Part_Category = -1;
    *ware->Part_Quantity = -1;
    *ware->Part_Pricing = 0.0;
    strcpy(ware->Part_Brand, " ");
}

void initOrders(struct Order *order)
{
    *order = (struct Order){
        .Order_ID = 0,
        .Warehouse_Order = NULL,
        .Order_State = NOT_COMPLETED,
        .Order_Request_Amount = 0,
        .numItems = 0};
}

void seedRandom()
{
    srand(time(NULL));
}

void addItem(struct Warehouse *ware, struct Warehouse **warehouse_array, size_t *itemCount, const char *itemBrand, int itemCategory, int itemQuantity, double itemPrice)
{

    int generatedID = rand() % (99999 - 10000 + 1) + 10000;

    bool isUnique = true;
    for (size_t i = 0; i < *itemCount; i++)
    {
        if (*(warehouse_array[i]->Part_ID) == generatedID)
        {
            isUnique = false;
            break;
        }
    }

    if (isUnique)
    {
        free(ware->Part_Brand);

        ware->Part_Brand = strdup(itemBrand);

        if (ware->Part_Brand == NULL)
        {
            printf("MEMALLOC FAILED! EXITING PROGRAM\n");
            exit(EXIT_FAILURE);
        }

        *ware->Part_ID = generatedID;
        *ware->Part_Category = itemCategory;
        *ware->Part_Quantity = itemQuantity;
        *ware->Part_Pricing = itemPrice;
    }
    else
    {
        printf("Generated ID is not unique. Please try again.\n");
    }
}

void removeItem(struct Warehouse **warehouse_array, size_t *item_count, int itemID)
{
    for (size_t i = 0; i < *item_count; i++)
    {
        if (*(warehouse_array[i]->Part_ID) == itemID)
        {
            free(warehouse_array[i]->Part_Brand);
            free(warehouse_array[i]->Part_ID);
            free(warehouse_array[i]->Part_Category);
            free(warehouse_array[i]->Part_Quantity);
            free(warehouse_array[i]->Part_Pricing);
            free(warehouse_array[i]);

            for (size_t j = i; j < *item_count - 1; j++)
            {
                warehouse_array[j] = warehouse_array[j + 1];
            }

            (*item_count)--;

            printf("Item successfully removed.\n");
            return;
        }
    }
    printf("Item not found in warehouse.\n");
}

void viewWarehouse(struct Warehouse *ware[], size_t numItems)
{
    printf("\n\nItems in the warehouse:\n\n");
    printf("No. Item Name\tCategory\tItem Amount\tItem Pricing");
    for (size_t i = 0; i < numItems; i++)
    {
        printf("%zu. %s\t%s   \t%d\t%lf\n", i + 1, ware[i]->Part_Brand, categoryNames[*(ware[i]->Part_Category)], *(ware[i]->Part_Quantity), ware[i]->Part_Pricing);
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

void viewOrders(struct Order *order[], int totalOrders)
{
    printf("List of orders:\n");
    printf("No. Order ID\tItem Requested\tTotal Requested\tOrder Status\n");
    for (size_t i = 0; i < totalOrders; i++)
    {
        printf("%d. %d\t%d\t%d\t%s\n", i + 1, order[i]->Order_ID, order[i]->Order_Request_Amount, order[i]->Order_State);
    }
}

void cancelOrder()
{
}

void changeOrderState(struct Order *order, struct Warehouse *ware, int OrderState)
{
    switch (OrderState)
    {
    case NOT_COMPLETED:
        order->Order_State = OrderState;
        break;
    case COMPLETED:
        order->Order_State = OrderState;
        ware->Part_Quantity = ware->Part_Quantity - order->Order_Request_Amount;
        break;
    case CANCELLED:
        order->Order_State = OrderState;
        break;
    default:
        printf("Order State Not Applicable.\nPlease Enter a valid one!\n");
        break;
    }
}

// void promotion_TEMP()
// {

// }

void checkItemCount(size_t *itemCount)
{
    printf("Current item count: %zu\n", *itemCount);
}

void adminMenu(struct Warehouse **warehouse_array, size_t *item_count)
{
    int choice;
    int itemID;
    char itemBrand[MAX_BRAND_BUFFER];
    int itemCategory;
    int itemQuantity;
    double itemPrice;

    do
    {
        printf("\nAdmin Menu:\n");
        printf("1. Add Item\n");
        printf("2. Remove Item\n");
        printf("3. View Warehouse\n");
        printf("4. Exit\n");
        printf("5. Function Check\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter Item Brand: ");
            scanf("%s", itemBrand);
            printf("Enter Item Category (1-5): ");
            scanf("%d", &itemCategory);
            printf("Enter Item Quantity: ");
            scanf("%d", &itemQuantity);
            printf("Enter Item Price: ");
            scanf("%lf", &itemPrice);
            addItem(warehouse_array[*item_count], warehouse_array, item_count, itemBrand, itemCategory, itemQuantity, itemPrice);
            *item_count += 1;
            break;

        case 2:
            printf("Enter Item ID to Remove: ");
            scanf("%d", &itemID);
            removeItem(warehouse_array, item_count, itemID);
            break;

        case 3:
            viewWarehouse(warehouse_array, *item_count);
            break;

        case 4:
            puts("Thanks for using the application!\n");
            exit(EXIT_SUCCESS);
            break;

        case 5:
            checkItemCount(item_count);

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 4);
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
    size_t array_size = 10;

    struct Warehouse **warehouse_array = (struct Warehouse **)malloc(sizeof(struct Warehouse *) * array_size);

    if (warehouse_array == NULL)
    {
        printf("MEMALLOC FAILED! EXITING PROGRAM\n");
        exit(EXIT_FAILURE);
    }

    enum RUNNING_STATES { NOT_RUNNING = 0, RUNNING = 1 };
    enum RUNNING_STATES runningState = RUNNING;

    while (runningState == RUNNING)
    {
        adminMenu(warehouse_array, &item_count);
    }

    freeWarehouseArray(warehouse_array, item_count);

    return 0;
}