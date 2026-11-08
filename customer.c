#include <stdio.h>
#include "common.h"



// Save a confirmed order into orders.txt
static int saveOrderData(const Order *newOrder)
{
    FILE *fp;

    fp = fopen("orders.txt", "a");

    if(fp == NULL)
    {
        printf("\nUnable to save order data!\n");
        return 0;
    }

    fprintf(fp, "%d|%s|%d|%.2f|%.2f|%.2f|%.2f|%s\n",
            newOrder->orderID,
            newOrder->customerName,
            newOrder->itemCount,
            newOrder->totalAmount,
            newOrder->discountPercent,
            newOrder->discountAmount,
            newOrder->finalAmount,
            newOrder->orderStatus);

    for(int i = 0; i < newOrder->itemCount; i++)
    {
        fprintf(fp, "%d|%d|%.2f\n",
                newOrder->items[i].foodID,
                newOrder->items[i].quantity,
                newOrder->items[i].price);
    }

    fclose(fp);
    return 1;
}


// Load previously saved orders from orders.txt
static void loadOrderData(void)
{
    FILE *fp;
    Order temp = {0};

    fp = fopen("orders.txt", "r");

    if(fp == NULL)
    {
        return;
    }

    while(orderCount < MAX_ORDERS)
    {
        temp = (Order){0};

        if(fscanf(fp, "%d|%49[^|]|%d|%f|%f|%f|%f|%19s\n",
                  &temp.orderID,
                  temp.customerName,
                  &temp.itemCount,
                  &temp.totalAmount,
                  &temp.discountPercent,
                  &temp.discountAmount,
                  &temp.finalAmount,
                  temp.orderStatus) != 8)
        {
            break;
        }

        if(temp.itemCount < 0 || temp.itemCount > MAX_ORDER_ITEMS)
        {
            break;
        }

        int valid = 1;

        for(int i = 0; i < temp.itemCount; i++)
        {
            if(fscanf(fp, "%d|%d|%f\n",
                      &temp.items[i].foodID,
                      &temp.items[i].quantity,
                      &temp.items[i].price) != 3)
            {
                valid = 0;
                break;
            }
        }

        if(!valid)
        {
            break;
        }

        orders[orderCount] = temp;
        orderCount++;
    }

    fclose(fp);
}








// Function 1: Take Customer Details
static void takeCustomerDetails(Order *newOrder)
{
    printf("\n----- CUSTOMER DETAILS -----\n");

    printf("Enter your name: ");
    scanf(" %49[^\n]", newOrder->customerName);

    printf("\nWelcome, %s!\n", newOrder->customerName);
}


// Function 2: Calculate Discount
static void calculateDiscount(Order *newOrder)
{
    if(newOrder->totalAmount >= 10000)
    {
        newOrder->discountPercent = 15;
    }
    else if(newOrder->totalAmount >= 5000)
    {
        newOrder->discountPercent = 10;
    }
    else if(newOrder->totalAmount >= 2000)
    {
        newOrder->discountPercent = 5;
    }
    else
    {
        newOrder->discountPercent = 0;
    }

    newOrder->discountAmount =
        newOrder->totalAmount * newOrder->discountPercent / 100;

    newOrder->finalAmount =
        newOrder->totalAmount - newOrder->discountAmount;
}


// Function 3: Display Categories
static void displayCategories(void)
{
    printf("\n========== FOOD CATEGORIES ==========\n");

    for(int i = 0; i < MAX_CATEGORIES; i++)
    {
        printf("%d. %s\n", i + 1, categories[i].categoryName);
    }

    printf("0. Go Back\n");
}


// Function 4: Display Food Items of a Category
static void displayCategoryItems(int categoryID)
{
    printf("\n========== CATEGORY FOOD ITEMS ==========\n");

    printf("%-10s %-25s %s\n",
           "Food ID", "Food Name", "Price");

    for(int i = 0; i < foodCount; i++)
    {
        if(food[i].categoryID == categoryID)
        {
            printf("%-10d %-25s Rs. %.2f\n",
                   food[i].foodID,
                   food[i].foodName,
                   food[i].price);
        }
    }
}


// Function 5: Add Food Item to Order
static void addFoodToOrder(Order *newOrder, int foodIndex)
{
    int quantity;
    int existingItem = -1;

    printf("Enter quantity: ");
    scanf("%d", &quantity);

    if(quantity <= 0)
    {
        printf("Invalid quantity!\n");
        return;
    }

    // Check whether item already exists in order
    for(int i = 0; i < newOrder->itemCount; i++)
    {
        if(newOrder->items[i].foodID == food[foodIndex].foodID)
        {
            existingItem = i;
            break;
        }
    }

    if(existingItem != -1)
    {
        newOrder->items[existingItem].quantity += quantity;

        printf("Item quantity increased successfully!\n");
    }
    else if(newOrder->itemCount < MAX_ORDER_ITEMS)
    {
        int position = newOrder->itemCount;

        newOrder->items[position].foodID =
            food[foodIndex].foodID;

        newOrder->items[position].quantity = quantity;

        newOrder->items[position].price =
            food[foodIndex].price;

        newOrder->itemCount++;

        printf("Item added successfully!\n");
    }
    else
    {
        printf("Your cart is full!\n");
    }
}


// Function 6: Search Food Item by ID
static void searchCustomerFoodItem(Order *newOrder)
{
    int foodID;
    int found = 0;

    printf("\nEnter Food ID: ");
    scanf("%d", &foodID);

    for(int i = 0; i < foodCount; i++)
    {
        if(food[i].foodID == foodID)
        {
            found = 1;

            printf("\nFood Item Found!\n");
            printf("Food Name: %s\n", food[i].foodName);
            printf("Price: Rs. %.2f\n", food[i].price);

            addFoodToOrder(newOrder, i);

            break;
        }
    }

    if(found == 0)
    {
        printf("\nFood ID not found! Please try again.\n");
    }
}


// Function 7: Select Food Items

static void selectFoodItems(Order *newOrder)
{
    int choice;
    int categoryChoice;
    int selectedCategoryID;
    int foodID;
    int found;

    do
    {
        printf("\n====================================\n");
        printf("          ORDERING MENU\n");
        printf("====================================\n");

        printf("1. Select Category\n");
        printf("2. Search Food Item by ID\n");
        printf("3. View Complete Menu\n");
        printf("4. Review Order / Finish Selection\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            // OPTION 1: SELECT CATEGORY
            case 1:

                displayCategories();

                printf("\nEnter Category Number: ");
                scanf("%d", &categoryChoice);

                if(categoryChoice == 0)
                {
                    break;
                }

                if(categoryChoice < 1 ||
                   categoryChoice > MAX_CATEGORIES)
                {
                    printf("\nInvalid category choice!\n");
                    break;
                }

                selectedCategoryID =
                    categories[categoryChoice - 1].categoryID;

                displayCategoryItems(selectedCategoryID);

                printf("\nEnter Food ID (0 to go back): ");
                scanf("%d", &foodID);

                if(foodID == 0)
                {
                    break;
                }

                found = 0;

                for(int i = 0; i < foodCount; i++)
                {
                    if(food[i].foodID == foodID &&
                       food[i].categoryID == selectedCategoryID)
                    {
                        found = 1;

                        addFoodToOrder(newOrder, i);

                        break;
                    }
                }

                if(found == 0)
                {
                    printf("\nFood ID not found in this category!\n");
                }

                break;


            // OPTION 2: SEARCH FOOD ITEM BY ID
            case 2:

                searchCustomerFoodItem(newOrder);

                break;


            // OPTION 3: VIEW COMPLETE MENU AND SELECT FOOD
            case 3:

                displayMenu();

                printf("\nEnter Food ID (0 to go back): ");
                scanf("%d", &foodID);

                if(foodID == 0)
                {
                    break;
                }

                found = 0;

                // Search Food ID in the complete menu
                for(int i = 0; i < foodCount; i++)
                {
                    if(food[i].foodID == foodID)
                    {
                        found = 1;

                        printf("\nFood Item Found!\n");
                        printf("Food Name: %s\n", food[i].foodName);
                        printf("Price: Rs. %.2f\n", food[i].price);

                        addFoodToOrder(newOrder, i);

                        break;
                    }
                }

                if(found == 0)
                {
                    printf("\nFood ID not found!\n");
                }

                break;


            // OPTION 4: FINISH SELECTING
            case 4:

                if(newOrder->itemCount == 0)
                {
                    printf("\nYou haven't selected any food items yet!\n");
                    choice = 0;
                }
                else
                {
                    printf("\nReturning to Order Summary...\n");
                }

                break;


            default:

                printf("\nInvalid choice! Please try again.\n");
        }

    } while(choice != 4);

    // Calculate subtotal
    newOrder->totalAmount = 0;

    for(int i = 0; i < newOrder->itemCount; i++)
    {
        newOrder->totalAmount +=
            newOrder->items[i].price *
            newOrder->items[i].quantity;
    }

    // Calculate discount
    calculateDiscount(newOrder);
}


// Function 8: Display Order Summary
static void displayOrderSummary(Order *newOrder)
{
    printf("\n========== ORDER SUMMARY ==========\n");

    printf("Customer Name: %s\n", newOrder->customerName);

    printf("\n%-10s %-25s %-10s %-10s\n",
           "Food ID", "Food Name", "Quantity", "Price");

    for(int i = 0; i < newOrder->itemCount; i++)
    {
        for(int j = 0; j < foodCount; j++)
        {
            if(newOrder->items[i].foodID == food[j].foodID)
            {
                printf("%-10d %-25s %-10d Rs. %.2f\n",
                       newOrder->items[i].foodID,
                       food[j].foodName,
                       newOrder->items[i].quantity,
                       newOrder->items[i].price *
                       newOrder->items[i].quantity);

                break;
            }
        }
    }

    printf("-----------------------------------\n");
    printf("Subtotal: Rs. %.2f\n", newOrder->totalAmount);
    printf("Discount: %.0f%%\n", newOrder->discountPercent);
    printf("Discount Amount: Rs. %.2f\n", newOrder->discountAmount);
    printf("-----------------------------------\n");
    printf("Final Amount: Rs. %.2f\n", newOrder->finalAmount);
}


// Function 9: Update Order
static void updateOrder(Order *newOrder)
{
    int updateFoodID;
    int newQuantity;
    int itemFound = 0;

    printf("\nEnter Food ID to update or remove: ");
    scanf("%d", &updateFoodID);

    for(int i = 0; i < newOrder->itemCount; i++)
    {
        if(newOrder->items[i].foodID == updateFoodID)
        {
            itemFound = 1;

            printf("Enter new quantity (0 to remove): ");
            scanf("%d", &newQuantity);

            if(newQuantity < 0)
            {
                printf("Invalid quantity!\n");
            }
            else if(newQuantity == 0)
            {
                for(int j = i; j < newOrder->itemCount - 1; j++)
                {
                    newOrder->items[j] = newOrder->items[j + 1];
                }

                newOrder->itemCount--;

                printf("Item removed successfully!\n");
            }
            else
            {
                newOrder->items[i].quantity = newQuantity;

                printf("Quantity updated successfully!\n");
            }

            break;
        }
    }

    if(itemFound == 0)
    {
        printf("Food ID not found in your order!\n");
    }

    // Recalculate subtotal
    newOrder->totalAmount = 0;

    for(int i = 0; i < newOrder->itemCount; i++)
    {
        newOrder->totalAmount +=
            newOrder->items[i].price *
            newOrder->items[i].quantity;
    }

    // Recalculate discount
    calculateDiscount(newOrder);
}


// Function 10: Confirm Order
static void confirmOrder(Order *newOrder)
{
    newOrder->orderID = 1001 + orderCount;

   int i;
char status[] = "Pending";

for(i = 0; status[i] != '\0'; i++)
{
    newOrder->orderStatus[i] = status[i];
}
newOrder->orderStatus[i] = '\0';

  // Save order in global order array
orders[orderCount] = *newOrder;

// Save order in file
if(saveOrderData(&orders[orderCount]))
{
    printf("\nOrder saved successfully!\n");
}

orderCount++;

    printf("\n========== ORDER CONFIRMATION ==========\n");

    printf("Customer Name: %s\n", newOrder->customerName);
    printf("Order ID: %d\n", newOrder->orderID);
    printf("Subtotal: Rs. %.2f\n", newOrder->totalAmount);
    printf("Discount: Rs. %.2f\n", newOrder->discountAmount);
    printf("Final Amount: Rs. %.2f\n", newOrder->finalAmount);
    printf("Order Status: %s\n", newOrder->orderStatus);

    printf("\nYour order has been placed successfully!\n");
}


// Function 11: Place New Order
static void placeOrder(void)
{
    Order newOrder = {0};
    int choice;

    if(orderCount >= MAX_ORDERS)
    {
        printf("\nOrder limit reached!\n");
        return;
    }

    takeCustomerDetails(&newOrder);

    selectFoodItems(&newOrder);

    if(newOrder.itemCount == 0)
    {
        printf("\nNo items selected. Order cancelled.\n");
        return;
    }

    do
    {
        displayOrderSummary(&newOrder);

        printf("\n1. Confirm Order\n");
        printf("2. Update Order\n");
        printf("3. Cancel Order\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        if(choice == 1)
        {
            confirmOrder(&newOrder);
        }
        else if(choice == 2)
        {
            updateOrder(&newOrder);

            if(newOrder.itemCount == 0)
            {
                printf("\nYour order has no items remaining. Order cancelled.\n");
            }
        }
        else if(choice == 3)
        {
            printf("\nOrder cancelled!\n");
        }
        else
        {
            printf("\nInvalid choice! Try again.\n");
        }

    } while(choice != 1 && choice != 3 && newOrder.itemCount > 0);
}


// Function 12: Display Saved Order Details
static void displaySavedOrderDetails(const Order *savedOrder)
{
    printf("\n========== ORDER DETAILS ==========\n");

    printf("Order ID: %d\n", savedOrder->orderID);
    printf("Customer Name: %s\n", savedOrder->customerName);
    printf("Order Status: %s\n", savedOrder->orderStatus);

    printf("\n%-10s %-25s %-10s %-10s\n",
           "Food ID", "Food Name", "Quantity", "Price");

    for(int i = 0; i < savedOrder->itemCount; i++)
    {
        int found = 0;

        for(int j = 0; j < foodCount; j++)
        {
            if(savedOrder->items[i].foodID == food[j].foodID)
            {
                printf("%-10d %-25s %-10d Rs. %.2f\n",
                       savedOrder->items[i].foodID,
                       food[j].foodName,
                       savedOrder->items[i].quantity,
                       savedOrder->items[i].price *
                       savedOrder->items[i].quantity);

                found = 1;
                break;
            }
        }

        if(found == 0)
        {
            printf("%-10d %-25s %-10d Rs. %.2f\n",
                   savedOrder->items[i].foodID,
                   "Item unavailable",
                   savedOrder->items[i].quantity,
                   savedOrder->items[i].price *
                   savedOrder->items[i].quantity);
        }
    }

    printf("-----------------------------------\n");
    printf("Subtotal: Rs. %.2f\n", savedOrder->totalAmount);
    printf("Discount: %.0f%%\n", savedOrder->discountPercent);
    printf("Discount Amount: Rs. %.2f\n", savedOrder->discountAmount);
    printf("Final Amount: Rs. %.2f\n", savedOrder->finalAmount);
}


// Function 13: Track Order Status
static void trackOrderStatus(void)
{
    int searchID;
    int found = 0;

    printf("\n----- TRACK ORDER STATUS -----\n");

    printf("Enter your Order ID: ");
    scanf("%d", &searchID);

    for(int i = 0; i < orderCount; i++)
    {
        if(orders[i].orderID == searchID)
        {
            found = 1;

            displaySavedOrderDetails(&orders[i]);

            break;
        }
    }

    if(found == 0)
    {
        printf("\nOrder ID not found!\n");
    }
}


//Function for comparing the names
static int compareNamesIgnoreCase(const char name1[], const char name2[])
{
    int i = 0;
    char ch1, ch2;

    while(name1[i] != '\0' && name2[i] != '\0')
    {
        ch1 = name1[i];
        ch2 = name2[i];

        if(ch1 >= 'A' && ch1 <= 'Z')
            ch1 = ch1 + 32;

        if(ch2 >= 'A' && ch2 <= 'Z')
            ch2 = ch2 + 32;

        if(ch1 != ch2)
            return 0;

        i++;
    }

    return name1[i] == '\0' && name2[i] == '\0';
}
// Function 14: View Order History
static void viewOrderHistory(void)
{
    char customerName[50];
    int found = 0;

    printf("\n----- CUSTOMER ORDER HISTORY -----\n");

    printf("Enter your customer name: ");
    scanf(" %49[^\n]", customerName);

    for(int i = 0; i < orderCount; i++)
    {
       if(compareNamesIgnoreCase(orders[i].customerName, customerName))
        {
            found = 1;

            displaySavedOrderDetails(&orders[i]);
        }
    }

    if(found == 0)
    {
        printf("\nNo order history found for this name!\n");
    }
}


// Function 15: Customer Dashboard
void customerDashboard(void)
{
    int choice;
    static int loaded = 0;

    if(!loaded)
    {
        loadOrderData();
        loaded = 1;
    }

    do
    {
        printf("\n====================================\n");
        printf("          CUSTOMER PORTAL\n");
        printf("====================================\n");

        printf("1. View Food Menu\n");
        printf("2. Place New Order\n");
        printf("3. Track Order Status\n");
        printf("4. View Order History\n");
        printf("5. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                displayMenu();
                break;

            case 2:
                placeOrder();
                break;

            case 3:
                trackOrderStatus();
                break;

            case 4:
                viewOrderHistory();
                break;

            case 5:
                printf("\nThank you for visiting our restaurant!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while(choice != 5);
}
