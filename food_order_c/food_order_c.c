#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 50
#define MAX_ORDERS 100
#define MAX_NAME_LENGTH 50
#define MAX_INPUT_LENGTH 200
#define MAX_RECEIPT_PATH 100

// Structure for menu item
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
} MenuItem;

// Structure for order
typedef struct {
    int orderId;
    MenuItem items[MAX_ITEMS];
    int itemCount;
    float totalCost;
    time_t orderTime;
} Order;

// Global arrays to store menu and orders
MenuItem menu[] = {
    {1, "Biriyani", 120},
    {2, "Full Meals", 100},
    {3, "Mini Meal", 80},
    {4, "Parotta", 15},
    {5, "Soda", 15},
    {6, "Ice Cream", 50},
    {7, "Chicken Fry", 200},
    {8, "Fish Fry", 150},
    {9, "Paneer Butter Masala", 120},
    {10, "Paneer Tikka", 100}
};
int menuItemCount = 10;

Order orders[MAX_ORDERS];
int orderCount = 0;

// Find menu item by ID
MenuItem* findMenuItem(int id) {
    for (int i = 0; i < menuItemCount; i++) {
        if (menu[i].id == id) {
            return &menu[i];
        }
    }
    return NULL;
}

// Generate receipt for the order
void generateReceipt(Order *order) {
    // Create a filename with order ID
    char filename[MAX_RECEIPT_PATH];
    sprintf(filename, "receipt_order_%d.txt", order->orderId);
    
    FILE *receiptFile = fopen(filename, "w");
    if (receiptFile == NULL) {
        printf("Error: Unable to create receipt file!\n");
        return;
    }
    
    // Format the current time for the receipt
    char timeStr[100];
    struct tm *timeinfo = localtime(&(order->orderTime));
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // Write receipt header
    fprintf(receiptFile, "===================================\n");
    fprintf(receiptFile, "          FOOD ORDER RECEIPT       \n");
    fprintf(receiptFile, "===================================\n\n");
    fprintf(receiptFile, "Order ID: %d\n", order->orderId);
    fprintf(receiptFile, "Date & Time: %s\n\n", timeStr);
    
    // Write items details
    fprintf(receiptFile, "Items Ordered:\n");
    fprintf(receiptFile, "-----------------------------------\n");
    fprintf(receiptFile, "%-25s %5s %10s\n", "Item", "Qty", "Price");
    fprintf(receiptFile, "-----------------------------------\n");
    
    // Count item frequencies for the receipt
    int itemCounts[MAX_ITEMS] = {0};
    int uniqueItems = 0;
    MenuItem uniqueItemList[MAX_ITEMS];
    
    for (int i = 0; i < order->itemCount; i++) {
        int found = 0;
        for (int j = 0; j < uniqueItems; j++) {
            if (strcmp(order->items[i].name, uniqueItemList[j].name) == 0) {
                itemCounts[j]++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            uniqueItemList[uniqueItems] = order->items[i];
            itemCounts[uniqueItems] = 1;
            uniqueItems++;
        }
    }
    
    // Write each unique item with its quantity and price
    for (int i = 0; i < uniqueItems; i++) {
        float itemTotal = uniqueItemList[i].price * itemCounts[i];
        fprintf(receiptFile, "%-25s %2d x ₹%-8.2f ₹%.2f\n", 
                uniqueItemList[i].name, 
                itemCounts[i],
                uniqueItemList[i].price, 
                itemTotal);
    }
    
    // Write total
    fprintf(receiptFile, "-----------------------------------\n");
    fprintf(receiptFile, "%-25s %11s ₹%.2f\n", "Subtotal:", "", order->totalCost);
    fprintf(receiptFile, "%-25s %11s ₹%.2f\n", "GST (5%):", "", order->totalCost * 0.05);
    float totalWithTax = order->totalCost + (order->totalCost * 0.05);
    fprintf(receiptFile, "%-25s %11s ₹%.2f\n", "TOTAL:", "", totalWithTax);
    fprintf(receiptFile, "===================================\n\n");
    fprintf(receiptFile, "Thank you for your order!\n");
    fprintf(receiptFile, "Please visit again!\n");
    
    fclose(receiptFile);
    printf("Receipt generated successfully: %s\n", filename);
}

// Place new orders
void placeOrder() {
    int moreOrders = 1;

    while (moreOrders) {
        if (orderCount >= MAX_ORDERS) {
            printf("Sorry, maximum order limit reached!\n");
            return;
        }

        Order newOrder;
        newOrder.orderId = orderCount + 1;
        newOrder.itemCount = 0;
        newOrder.totalCost = 0.0;
        newOrder.orderTime = time(NULL); // Set current time

        printf("\nAvailable Items:\n");
        printf("----------------------------------------------------\n");
        printf("ID  %-25s  Price\n", "Item");
        printf("----------------------------------------------------\n");
        for (int i = 0; i < menuItemCount; i++) {
            printf("%-3d %-25s  ₹%.2f\n", 
                   menu[i].id, menu[i].name, menu[i].price);
        }
        printf("----------------------------------------------------\n");
        printf("Enter item IDs separated by commas (0 to finish):\n");

        char input[MAX_INPUT_LENGTH];
        while(getchar() != '\n');
        
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0 || strcmp(input, "0") == 0) {
            printf("Order cancelled.\n");
            continue;
        }

        char *token = strtok(input, ",");
        while (token != NULL) {
            int itemId = atoi(token);

            // Proper break condition
            if (itemId == 0) {
                break;
            }

            MenuItem* selectedItem = findMenuItem(itemId);
            
            if (selectedItem) {
                if (newOrder.itemCount < MAX_ITEMS) {
                    newOrder.items[newOrder.itemCount++] = *selectedItem;
                    newOrder.totalCost += selectedItem->price;
                    printf("Added %s to your order.\n", selectedItem->name);
                } else {
                    printf("Maximum items per order reached!\n");
                    break;
                }
            } else {
                printf("Invalid item ID: %d\n", itemId);
            }

            token = strtok(NULL, ",");
        }

        if (newOrder.itemCount > 0) {
            // Add order to orders array
            orders[orderCount++] = newOrder;

            printf("\nOrder placed successfully!\n");
            printf("Order ID: %d\n", newOrder.orderId);
            printf("Total Cost: ₹%.2f\n", newOrder.totalCost);
            
            // Generate and print receipt
            generateReceipt(&orders[orderCount-1]);
        }

        printf("Do you want to place another order? (1 for Yes, 0 for No): ");
        if (scanf("%d", &moreOrders) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            moreOrders = 0;
        }
    }
}

// Main menu and program flow
int main() {
    while (1) {
        printf("\n===== FOOD ORDER MANAGEMENT SYSTEM =====\n");
        printf("1. Place Order\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }

        switch(choice) {
            case 1: placeOrder(); break;
            case 2: 
                printf("Thank you for using the Food Order Management System!\n");
                exit(0);
            default: 
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}