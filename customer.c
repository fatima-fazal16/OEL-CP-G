#include <stdio.h>
// Provides shared structures, constants, global variables, and function declaration
#include "common.h" 
// Save a confirmed order into orders.txt
// Save one confirmed order and its food items by appending them to orders.txt.
static int saveOrderData(const Order *newOrder)
{
    // File pointer used to access the order file.
    FILE *fp;
    // Append mode adds a new order without deleting previously saved orders.
    fp=fopen("orders.txt", "a");
    // fopen returns NULL if the file could not be opened.
    if(fp==NULL)
    {
        printf("\nUnable to save order data!\n");
        return 0;
    }
     // Save the main order details in one line, with | separating the fields.
    fprintf(fp, "%d|%s|%d|%.2f|%.2f|%.2f|%.2f|%s\n",
            newOrder->orderID,
            newOrder->customerName,
            newOrder->itemCount,
            newOrder->totalAmount,
            newOrder->discountPercent,
            newOrder->discountAmount,
            newOrder->finalAmount,
            newOrder->orderStatus);

         // Save every food item that belongs to this order.
    for(int i=0;i<newOrder->itemCount;i++)
    {
        // Save the food ID, quantity, and unit price of this item.
        fprintf(fp, "%d|%d|%.2f\n",newOrder->items[i].foodID,newOrder->items[i].quantity,newOrder->items[i].price);
    } // Close the file after completing the file operation.

    fclose(fp);
    return 1;
}
// Rewrite orders.txt with all orders currently stored in the global array.
int saveAllOrderData(void)
{
    FILE *fp;
    // Write mode replaces the old contents so all orders can be saved again.
    fp=fopen("orders.txt", "w");
    if(fp==NULL)
    {
        printf("\nUnable to update order data!\n");
        return 0;
    }
    for(int i=0;i<orderCount;i++)
    {
        fprintf(fp,"%d|%s|%d|%.2f|%.2f|%.2f|%.2f|%s\n",
                orders[i].orderID,
                orders[i].customerName,
                orders[i].itemCount,
                orders[i].totalAmount,
                orders[i].discountPercent,
                orders[i].discountAmount,
                orders[i].finalAmount,
                orders[i].orderStatus);
        for(int j=0;j<orders[i].itemCount;j++)
        {
            fprintf(fp,"%d|%d|%.2f\n",orders[i].items[j].foodID,orders[i].items[j].quantity,orders[i].items[j].price);
        }
    }
    fclose(fp);
    return 1;
}
// Load previously saved orders from orders.txt
// Load previously saved orders from the file into the global orders array.
void loadOrderData(void)
{
    FILE *fp;
    // Temporary order structure; {0} initializes all its members to zero.
    Order temp={0};
    // Read mode opens the saved order file.
    fp = fopen("orders.txt", "r");
    if(fp==NULL)
    {
        return;
    }
     // Reset the count before loading the saved orders again.
    orderCount=0;
    while(orderCount<MAX_ORDERS)
    {
        // Clear the temporary structure before reading the next order.
        temp = (Order){0};
         // Read eight order fields; %49[^|] reads a full name until the | separator.
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
         // Stop if the saved item count is outside the items array capacity.
        if(temp.itemCount < 0 || temp.itemCount > MAX_ORDER_ITEMS)
        {
            break;
        }
         // Flag: 1 means valid; it becomes 0 if an item record cannot be read.
        int valid=1;

         // Read each food item belonging to this order.

        for(int i=0;i<temp.itemCount;i++)
        {
            
            if(fscanf(fp, "%d|%d|%f\n",
                      &temp.items[i].foodID,
                      &temp.items[i].quantity,
                      &temp.items[i].price) != 3)
            {
                valid=0;
                break;
            }
        }

        // Do not store the order if one of its item records is incomplete.

        if(!valid)
        {
            break;
        }

         // Copy the completed temporary order into the global orders array.

        orders[orderCount]=temp;
        orderCount++;
    }

    fclose(fp);
}

// Function 1: Take Customer Details
// Ask for the customer name and store it in the supplied order structure.

static void takeCustomerDetails(Order *newOrder)
{
    printf("\n----- CUSTOMER DETAILS -----\n");

    printf("Enter your name: ");
     // Read the full name, including spaces, with a maximum of 49 characters.
    scanf(" %49[^\n]", newOrder->customerName);

    printf("\nWelcome, %s!\n", newOrder->customerName);
}

// Function 2: Calculate Discount
// Calculate the discount percentage, discount amount, and final payable amount.

static void calculateDiscount(Order *newOrder)
{
     // Choose the discount percentage according to the subtotal.

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

    // Calculate the amount of money deducted as the discount.
    newOrder->discountAmount=newOrder->totalAmount*newOrder->discountPercent/100;
     // Calculate the final amount payable after subtracting the discount.
    newOrder->finalAmount=newOrder->totalAmount-newOrder->discountAmount;
}


// Function 3: Display Categories
// Display all available food categories.

static void displayCategories(void)
{
    printf("\n========== FOOD CATEGORIES ==========\n");

      // Display every category; i + 1 is used because menu numbering starts at 1.

    for(int i = 0; i < MAX_CATEGORIES; i++)
    {
        printf("%d. %s\n", i + 1, categories[i].categoryName);
    }

    printf("0. Go Back\n");
}

// Function 4: Display Food Items of a Category
// Display only the food items belonging to the supplied category ID.

static void displayCategoryItems(int categoryID)
{
    printf("\n========== CATEGORY FOOD ITEMS ==========\n");

    printf("%-10s %-25s %s\n","Food ID", "Food Name", "Price");

    // Check each food record currently stored in the food array.
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
// Add a selected food item or increase its quantity if it is already in the order.

static void addFoodToOrder(Order *newOrder, int foodIndex)
{
    int quantity;
     // -1 means the selected food item has not been found in the order.
    int existingItem=-1;

    printf("Enter quantity: ");
    scanf("%d", &quantity); 

    if(quantity <= 0)   // Reject zero or negative quantities.
    {
        printf("Invalid quantity!\n");
        return;
    }

    // Check whether item already exists in order
    for(int i = 0; i < newOrder->itemCount; i++)
    {
         // Check whether this food is already present in the order.
        if(newOrder->items[i].foodID == food[foodIndex].foodID)
        {
            // Remember the array position where the existing item was found.
            existingItem = i;
            break;
        }
    }
// If found, increase its quantity instead of adding a duplicate item.
    if(existingItem != -1)
    {
        newOrder->items[existingItem].quantity += quantity;

        printf("Item quantity increased successfully!\n");
    }
    else if(newOrder->itemCount < MAX_ORDER_ITEMS)
    {
         // The item count gives the next free position in the items array.
        int position = newOrder->itemCount;

        newOrder->items[position].foodID =
            food[foodIndex].foodID;

        newOrder->items[position].quantity = quantity;

        newOrder->items[position].price =
            food[foodIndex].price;

        // Increase the number of distinct food items in the order.
        newOrder->itemCount++;

        printf("Item added successfully!\n");
    }
    else
    {
        printf("Your cart is full!\n");
    }
}


// Function 6: Search Food Item by ID
// Search the menu by food ID and add the matching item to the order.

static void searchCustomerFoodItem(Order *newOrder)
{
    int foodID;
    // Search flag: 0 means not found; 1 means a match has been found.
    int found = 0;

    printf("\nEnter Food ID: ");
    scanf("%d", &foodID);
    
    // Check each food record currently stored in the food array.
    for(int i = 0; i < foodCount; i++)
    {
        if(food[i].foodID == foodID)
        {
            found = 1;

            printf("\nFood Item Found!\n");
            printf("Food Name: %s\n", food[i].foodName);
            printf("Price: Rs. %.2f\n", food[i].price);

            // Pass the selected food array index to the function that adds it to the order.

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
// Show the ordering menu and let the customer select food items.

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

         // Execute the menu option selected by the customer.

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

                // Convert the displayed category number to an array index and get its category ID.

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
                    // Both the food ID and the selected category ID must match.
                    if(food[i].foodID == foodID &&food[i].categoryID == selectedCategoryID)
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

    } while(choice != 4); // Repeat the selection menu until the customer chooses option 4

    // Calculate subtotal
    // Reset the subtotal before calculating it again.
    newOrder->totalAmount = 0;

    for(int i = 0; i < newOrder->itemCount; i++)
    {
        // Add unit price multiplied by quantity to the running subtotal.
        newOrder->totalAmount +=
            newOrder->items[i].price *
            newOrder->items[i].quantity;
    }
    // Calculate discount
    calculateDiscount(newOrder);
}

// Function 8: Display Order Summary
// Display selected items, subtotal, discount, and final amount

static void displayOrderSummary(Order *newOrder)
{
    printf("\n========== ORDER SUMMARY ==========\n");

    printf("Customer Name: %s\n", newOrder->customerName);
    printf("\n%-10s %-25s %-10s %-10s\n","Food ID", "Food Name", "Quantity", "Price");

     // Save every food item that belongs to this order.       
    for(int i=0;i<newOrder->itemCount;i++)
    {
        for(int j=0;j<foodCount;j++)
        {
             // Save every food item that belongs to this order.
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
// Update an item quantity or remove the item, then recalculate the total.

static void updateOrder(Order *newOrder)
{
    int updateFoodID;
    int newQuantity;
    //Flag used to check whether the entered food ID is in this order.
    int itemFound = 0;
    printf("\nEnter Food ID to update or remove: ");
    scanf("%d", &updateFoodID);
    // Save every food item that belongs to this order.
    for(int i=0;i<newOrder->itemCount;i++)
    {
        if(newOrder->items[i].foodID==updateFoodID)
        {
            itemFound=1;

            printf("Enter new quantity (0 to remove): ");
            scanf("%d", &newQuantity);

            if(newQuantity<0)
            {
                printf("Invalid quantity!\n");
            }
            else if(newQuantity==0)
            {
                 // Shift later items one position left to fill the gap after removal.
                for(int j = i; j < newOrder->itemCount - 1; j++)
                {
                    newOrder->items[j] = newOrder->items[j + 1];
                }
                 // Reduce the number of active items after removing one item.
                newOrder->itemCount--;

                printf("Item removed successfully!\n");
            }
            else
            {
                newOrder->items[i].quantity=newQuantity;
                printf("Quantity updated successfully!\n");
            }
            break;
        }
    }
    if(itemFound==0)
    {
        printf("Food ID not found in your order!\n");
    }
    // Recalculate subtotal
    newOrder->totalAmount=0;
   // Save every food item that belongs to this order.
    for(int i=0;i<newOrder->itemCount;i++)
    {
        newOrder->totalAmount+=
            newOrder->items[i].price*
            newOrder->items[i].quantity;
    }
    // Recalculate discount
    calculateDiscount(newOrder);
}
// Function 10: Confirm Order
// Assign an ID and status, store the order, and display confirmation details
static void confirmOrder(Order *newOrder)
{
     // Generate an order ID using the current number of saved orders.
    newOrder->orderID=1001+orderCount;

   int i;
   // Every new order starts with Pending status.
char status[]="Pending";
// Copy the status string into the order structure one character at a time.
for(i=0;status[i]!='\0';i++)
{
    newOrder->orderStatus[i]=status[i];
}
newOrder->orderStatus[i]='\0'; // Mark the end of the status string.

  // Save order in global order array
// Copy the complete order structure into the next free position in the global array.
orders[orderCount]=*newOrder;
// Save order in file
// Pass the address of the stored order to the function that saves it in the file.
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
// Control the complete process of creating, reviewing, and confirming an order.
static void placeOrder(void)
{
    // Create a fresh order and initialize all members and items to zero.
    Order newOrder = {0};
    int choice;
    if(orderCount>=MAX_ORDERS)
    {
        printf("\nOrder limit reached!\n");
        return;
    }
       // Pass the address so the function can store the customer name in this order.
    takeCustomerDetails(&newOrder);
     // Let the customer select food and calculate the subtotal and discount.
    selectFoodItems(&newOrder);
    if(newOrder.itemCount == 0)
    {
        printf("\nNo items selected. Order cancelled.\n");
        return;
    }
    do
    {
         // Show the current order before asking whether to confirm, update, or cancel.
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
// Display the details and food items of an already saved order.
static void displaySavedOrderDetails(const Order *savedOrder)
{
    printf("\n========== ORDER DETAILS ==========\n");

    printf("Order ID: %d\n", savedOrder->orderID);
    printf("Customer Name: %s\n", savedOrder->customerName);
    printf("Order Status: %s\n", savedOrder->orderStatus);

    printf("\n%-10s %-25s %-10s %-10s\n",
           "Food ID", "Food Name", "Quantity", "Price");
     // Display every food item stored in this saved order.
    for(int i = 0; i < savedOrder->itemCount; i++)
    {
        int found = 0;
        // Search the current menu for the saved food ID to retrieve its name.
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
        if(found==0)
        {
            printf("%-10d %-25s %-10d Rs. %.2f\n",
                   savedOrder->items[i].foodID,"Item unavailable",savedOrder->items[i].quantity,savedOrder->items[i].price *savedOrder->items[i].quantity);
        }
    }
    printf("-----------------------------------\n");
    printf("Subtotal: Rs. %.2f\n", savedOrder->totalAmount);
    printf("Discount: %.0f%%\n", savedOrder->discountPercent);
    printf("Discount Amount: Rs. %.2f\n", savedOrder->discountAmount);
    printf("Final Amount: Rs. %.2f\n", savedOrder->finalAmount);
}

// Function 13: Track Order Status
// Find an order using its ID and display its saved details and status
static void trackOrderStatus(void)
{
     // Refresh the in-memory orders array from the saved file before searching.
    loadOrderData();
    int searchID;
    int found=0;
    printf("\n----- TRACK ORDER STATUS -----\n");
    printf("Enter your Order ID: ");
    scanf("%d",&searchID);
    // Visit every order stored in the global orders array.
    for(int i=0;i<orderCount;i++)
    {
        if(orders[i].orderID==searchID)
        {
            found=1;
            displaySavedOrderDetails(&orders[i]);
            break;
        }
    }
    if(found==0)
    {
        printf("\nOrder ID not found!\n");
    }
}

//Function for comparing the names
// Compare two names without distinguishing uppercase and lowercase letters.
static int compareNamesIgnoreCase(const char name1[], const char name2[])
{
    int i=0;
    char ch1,ch2;
     // Compare both names character by character until either string ends.
    while(name1[i]!='\0'&&name2[i]!='\0')
    {
        ch1=name1[i];
        ch2=name2[i];
        // Convert an uppercase English letter to lowercase using its ASCII value
        if(ch1>='A'&&ch1<='Z')
            ch1=ch1+32;
        if(ch2>='A'&&ch2<='Z')
            ch2=ch2+32;
        // If the corresponding characters differ, the names do not match.
        if(ch1!=ch2)
            return 0;
        i++;
    }
    return name1[i]=='\0'&&name2[i]=='\0';
}

// Function 14: View Order History
// Find and display all saved orders belonging to the entered customer name.
static void viewOrderHistory(void)
{
    char customerName[50];
    int found=0;
    printf("\n----- CUSTOMER ORDER HISTORY -----\n");
    printf("Enter your customer name: ");
    scanf(" %49[^\n]", customerName);
    for(int i=0;i<orderCount;i++)
    {
       if(compareNamesIgnoreCase(orders[i].customerName, customerName))
        {
            found=1;
            displaySavedOrderDetails(&orders[i]);
        }
    }
    if(found==0)
    {
        printf("\nNo order history found for this name!\n");
    }
}
// Function 15: Customer Dashboard
// Main customer menu for viewing food, placing orders, tracking, and history.
void customerDashboard(void)
{
    int choice;
    // Static local variable keeps its value between calls; 0 means not loaded yet.
    static int loaded=0;
     // Load saved orders only on the first visit to the customer dashboard.
    if(!loaded)
    {
        loadOrderData();
         // Remember that the initial loading has been completed.
        loaded=1;
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
        scanf("%d",&choice);
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
    } while(choice!=5); // Repeat the dashboard until the customer selects option 5 to exit.
}