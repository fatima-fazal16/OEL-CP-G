#include<stdio.h>
// Provides shared structures, constants, global variables, and function declaration
#include "common.h"

// Compares two character arrays manually.
//Returns 1 if both strings are identical otherwise returns 0.
int compareStrings(char str1[],char str2[]) //same as strcmp in string.h
{
     // i Keeps track of the current character position.
    int i=0; 
    // Continue comparing until either string reaches its null terminator.
    while(str1[i]!='\0'&&str2[i]!='\0') 
    {
        // Check whether the characters at the same index are different.
        if(str1[i]!=str2[i])
        {
            return 0; // Different characters mean the strings are not equal.
        }
        i++;  // Move to the next character in both strings.
    }

    // If both strings end at the same index,their contents are equal.

    if(str1[i]=='\0'&&str2[i]=='\0')
    {
        return 1;
    }
    // One string ended before the other, so they are not identical.
    return 0;
}

// FUNCTION 1: ADMIN LOGIN
// Checks the admin credentials and allows up to three attempts. 
int adminLogin(void)
{

    char adminID[20]; // Stores the ID entered by the administrator.
    char password[20];  // Stores the password entered by the administrator.
    int attempts=3;  // Maximum number of login attempts allowed.
    printf("\n===== ADMIN LOGIN =====\n");

     // Repeat the login process while attempts are still available.

    while(attempts>0)
    {
        printf("Enter Admin ID: ");
        scanf("%19s",adminID);//AD001
         // %19s limits the input to 19 characters, leaving space for '\0'.
        printf("Enter Password: ");
        scanf("%19s",password); //admin123
         // %19s limits the input to 19 characters, leaving space for '\0'.
        
        // Both conditions must be true for successful login.
        // && is the logical AND operator.
        // It means the ID AND password must both match.
        if(compareStrings(adminID,"AD001")==1&&compareStrings(password,"admin123")==1)
        {
            printf("\nLogin Successful!\n");
             // Returning 1 informs the calling function that login succeeded.
            return 1;
        }
        else
        {
            attempts--; // Reduce the remaining attempts after incorrect credentials.
            printf("\nIncorrect Admin ID or Password!\n");
            printf("Remaining Attempts: %d\n",attempts);
        }
    }
    // This statement executes when all three attempts have been used.
    printf("\nAccess Denied!\n");
    return 0;
    }
    
// FUNCTION 2: ADMIN DASHBOARD
// Shows the admin menu and runs the selected operation. 

void adminDashboard(void)
{
    // Stores the option selected from the dashboard menu.
    int choice;
    // The do-while loop displays the dashboard at least once.
    // The menu will keep appearing until the administrator logs out.
    do
    {
        printf("\n========== ADMIN DASHBOARD ==========\n");
        printf("1. Add Food Item\n");
        printf("2. Display Food Items\n");
        printf("3. Search Food Item\n");
        printf("4. Update Food Item\n");
        printf("5. Delete Food Item\n");
        printf("6. Display All Orders\n");
        printf("7. Update Order Status\n");
        printf("8. Logout\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);

          // switch checks the value of choice and executes the matching case.

        switch(choice)
        {
            case 1:
                addFoodItem();
                 // Calls the function responsible for adding a food item.
                break;
            case 2:
                displayMenu();
                 // Displays menu.
                break;
            case 3:
                searchFoodItem();
                 // Allows the administrator to search for a food item.
                break;
            case 4:
                updateFoodItem();
                // Updates a selected food item from the menu.
                break;
            case 5:
                deleteFoodItem();
                // Removes a selected food item from the menu.
                break;
            case 6:
                displayAllOrders();
                  // Displays all saved customer orders.
                break;
            case 7:
                updateOrderStatus();
                  // Displays the customer-order information.
                break;
            case 8:
                printf("\nLogging out...\n");
                 // The loop will end because choice is now 8.
                break;
            default:
             // Executes when the administrator enters an unavailable option.
                printf("\nInvalid choice! Try again.\n");
        }
    } while(choice!=8); // Keep showing the menu until the admin selects Logout.

}

// FUNCTION 3: ADD FOOD ITEM 
// Creates a new food record and saves it in food.txt.

void addFoodItem(void)
{

    int categoryID;
    int i;
    int maxItemNumber=0;//initially, we assume that no item has been found in the selected category
    int newFoodID;
    char newFoodName[50];
    float newPrice;
    // Check whether the food array is full
    if(foodCount>=MAX_FOOD)
    {
        printf("\nFood menu is full! Cannot add more items.\n");
        return;
    }
    // Display categories
    printf("\n========== FOOD CATEGORIES ==========\n");
    for (i = 0;i<MAX_CATEGORIES;i++)
    {
        printf("%d. %s\n",categories[i].categoryID,categories[i].categoryName);
    }
    // Ask admin to select category
   printf("\nEnter Category ID: ");

    if(scanf("%d",&categoryID)!=1)
    {
        printf("Invalid category input!\n");
        return;
    }
    // Validate category ID
    if(categoryID<1||categoryID>MAX_CATEGORIES)
    {
        printf("Invalid Category ID!\n");
        return;
    }
    // Find the highest existing item number in this category
    for(i=0;i<foodCount;i++)
    {
        if(food[i].categoryID==categoryID)
        {
              // % is the modulus operator; it gives the remainder.
            // Example: 203 % 100 = 3.
            // This extracts the item-number portion of the Food ID.
            if(food[i].foodID%100>maxItemNumber)
            {
                // Update maxItemNumber whenever a larger item number is found.
                maxItemNumber=food[i].foodID%100;
            }
        }
    }
    // Generate new Food ID
    if(maxItemNumber>=99)   // Prevent generating an item number beyond the allowed limit.
    {
        printf("This category has reached its ID limit!\n");
        return;
    }

    // Example: Category 2 and item number 3 produce ID 203.
    newFoodID=categoryID*100+maxItemNumber+1;
    // Ask for food name
    printf("Enter Food Name: ");
    // %49[^\n] reads a name containing spaces until Enter is pressed.
    // The leading space skips any leftover whitespace from previous input.
    if(scanf(" %49[^\n]",newFoodName)!=1)
    {
        printf("Invalid food name!\n");
        return;
    }
    // Ask for food price
    printf("Enter Food Price: Rs. ");
    if(scanf("%f",&newPrice)!=1||newPrice<=0) // Food price must be greater than zero.
    {
        printf("Invalid price!\n");
        return;
    }
    // Store the new food item
    food[foodCount].foodID=newFoodID;
    food[foodCount].categoryID=categoryID;
    // Copy the food name one character at a time.
    // Copy name into the food structure.
    // The loop stops when it reaches the null character of newFoodName.
    for (i=0;newFoodName[i]!='\0';i++)
    {
        food[foodCount].foodName[i]=newFoodName[i];
    }
    food[foodCount].foodName[i]='\0'; //Add null terminator to finish string 
                                    // Without it, C may continue reading unwanted characters.
    food[foodCount].price=newPrice;

    // Increase food count
    foodCount++;
    saveFoodData(); //file write "w"
    printf("\nFood item added successfully!\n");
    printf("Generated Food ID: %d\n",newFoodID);

}

// FUNCTION 4: SEARCH FOOD ITEM
// Searches for a food record using its Food ID.
void searchFoodItem(void)
{

    int choice;
    int searchID;
    int found=0; //changes to 1 when the requested record is found.
    int i;
    printf("\n========== SEARCH FOOD ITEM ==========\n");
    printf("1. Search by Food ID\n");;
    printf("\nEnter your choice: ");
    scanf("%d",&choice);
     // Perform the search only if the admin selects option 1.
    if(choice==1)
    {
        printf("\nEnter Food ID: ");
        scanf("%d",&searchID);
        // Check each active food record one by one
        for (i=0;i<foodCount;i++)
        {
            if (food[i].foodID==searchID)
            {
                found=1; // Marks that a matching food record has been found.
                printf("\nFood Item Found!\n");
                printf("Food ID: %d\n", food[i].foodID);
                printf("Food Name: %s\n", food[i].foodName);
                printf("Category ID: %d\n", food[i].categoryID);
                printf("Price: Rs. %.2f\n", food[i].price);
                break;
            }
        }
    }
    else
    {
        printf("\nInvalid choice!\n");
        return;
    }
    // Check if food item was found
    // If found is still 0, no food record matched the entered ID.
    if(found==0)
    {
        printf("\nFood Item Not Found!\n");
    }
}

// FUNCTION 5: UPDATE FOOD ITEM
// Finds a food record, changes its name and price, and saves it.

void updateFoodItem(void)
{
    int searchID; // Stores the ID of the food item to be updated.
    int i;        // Used to search through the food array.
    int j;        // Used to copy the new food name.
    int found=0; // Indicates whether the requested food item exists.
    char newFoodName[50];
    float newPrice;
    printf("\n========== UPDATE FOOD ITEM ==========\n");
    printf("Enter Food ID: ");
    if(scanf("%d",&searchID)!=1)
    {
        printf("\nInvalid Food ID!\n");
        return;
    }

    // Find the food record whose ID matches the admin's input.

    for(i=0;i<foodCount;i++)
    {
        if(food[i].foodID==searchID)
        {
            found=1; // Mark the record as found.
            printf("\nFood Item Found!\n");
            printf("Food ID: %d\n",food[i].foodID);
            printf("Food Name: %s\n",food[i].foodName);
            printf("Current Price: Rs. %.2f\n",food[i].price);

            // Ask for the replacement name; spaces are allowed.
            printf("\nEnter New Food Name: ");

            if (scanf(" %49[^\n]", newFoodName)!=1)
            {
                printf("\nInvalid Food Name!\n");
                return;
            }

            // Ask for the replacement price and ensure it is greater than zero

            printf("Enter New Food Price: Rs. ");
            if (scanf("%f",&newPrice)!=1||newPrice<=0)
            {
                printf("\nInvalid Price! Update Cancelled.\n");
                return;
            }
            // Copy the new name over the old name in the selected array element.
            for(j=0;newFoodName[j]!='\0';j++)
            {
                food[i].foodName[j]=newFoodName[j];
            }
            food[i].foodName[j] = '\0'; //Add null terminator 
            
            // Update food price
            food[i].price=newPrice;

            saveFoodData(); // Save the updated array so the changes remain in food.txt.

            printf("\nFood Item Updated Successfully!\n");
            printf("\nUpdated Food Details:\n");
            printf("Food ID: %d\n",food[i].foodID);
            printf("Food Name: %s\n",food[i].foodName);
            printf("Category ID: %d\n",food[i].categoryID);
            printf("New Price: Rs. %.2f\n",food[i].price);
            break;

        }
    }

    if(found==0)
    {
        printf("\nFood Item Not Found!\n");
    }
}

//FUNCTION 7 : DELETE FOOD ITEM 
// Finds a food record, and delete it

void deleteFoodItem(void)
{

    int searchID;
    int i;
    int j;
    int found=0;
    printf("\n========== DELETE FOOD ITEM ==========\n");
    printf("Enter Food ID to Delete: ");

    if(scanf("%d",&searchID)!=1)
    {
        printf("\nInvalid Food ID!\n");
        return;
    }

    // Find the food record with the ID entered by the admin.
    for(i=0;i<foodCount;i++)
    {
        if(food[i].foodID==searchID)
        {
            found=1;  // Mark the record as found.

            // Display the FOod record that is about to be deleted.

            printf("\nFood Item Found!\n");
            printf("Food ID: %d\n",food[i].foodID);
            printf("Food Name: %s\n",food[i].foodName);
            printf("Category ID: %d\n",food[i].categoryID);
            printf("Price: Rs. %.2f\n",food[i].price);

            // Shift remaining food items to the left

            for(j=i;j<foodCount-1;j++)
            {
                food[j]=food[j+1];
            }
            // Decrease food count
            foodCount--;
            saveFoodData();

            printf("\nFood Item Deleted Successfully!\n");
            break;
        }
    }
    // Check if food item was found

    if(found==0)
    {
        printf("\nFood Item Not Found!\n");
    }

}

//FUNTION 8 UPDATE ORDER STATUS 
// Loads orders, changes a selected status, and saves the orders.

void updateOrderStatus(void)
{
    int searchID;
    int found=0;
    int choice;
    char *status;
     // Load the latest saved orders.
    loadOrderData();
    printf("\n========== UPDATE ORDER STATUS ==========\n");
    printf("Enter Order ID: ");
    scanf("%d",&searchID);

    // Visit each order stored in the orders array.

    for(int i=0;i<orderCount;i++)
    {
        if(orders[i].orderID==searchID)
        {
            found=1; // Mark the record as found.
            printf("\nCustomer Name: %s\n", orders[i].customerName);
            printf("Current Status: %s\n", orders[i].orderStatus);
            printf("\n1. Pending\n");
            printf("2. Confirmed\n");
            printf("3. Preparing\n");
            printf("4. Completed\n");
            printf("5. Cancelled\n");
            printf("\nEnter new status: ");
            scanf("%d",&choice);

            // Choose the new order status based on the admin's selection.

            switch(choice)
            {
                case 1:
                    printf("\nOrder is Pending!\n");
                    status="Pending";
                    break;
                case 2:
                    printf("\nOrder is Confirmed!\n");
                    status="Confirmed";
                    break;
                case 3:
                    printf("\nOrder is Preparing!\n");
                    status="Preparing";
                    break;
                case 4:
                    printf("\nOrder is Completed!\n");
                    status="Completed";
                    break;
                case 5:
                    printf("\nOrder is Cancelled!\n");
                    status="Cancelled";
                    break;
                default:
                    printf("\nInvalid choice!\n");
                    return;
            }
            int j;  // Index used to copy the selected status into the character array.
            // Copy the selected status text character by character.

            for(j=0;status[j]!='\0';j++)
            {
                orders[i].orderStatus[j] = status[j];
            }
            orders[i].orderStatus[j] = '\0';  // Terminate the copied status string.

            if(saveAllOrderData()) // Save the updated order data; check whether saving was successful.
                {
                    printf("\nStatus saved successfully!\n");
                }
            else
                {
                    printf("\nUnable to save status!\n");
                }
            break;
        }
    }

    if(found==0)
        printf("\nOrder ID not found!\n");
}

//FUNCTION 9 DISPLAY ALL ORDERS
// Loads and displays every saved order and its ordered items.

void displayAllOrders(void)
{
    loadOrderData();
    printf("\n========== ALL CUSTOMER ORDERS ==========\n");

    if(orderCount==0) // If no orders are available, display a message and leave the function.
    {
        printf("\nNo orders found!\n");
        return;
    }

     // Outer loop: visits each order stored in the orders array.
    for(int i=0;i<orderCount;i++)
    {
        printf("\n-----------------------------\n");
        printf("Order ID: %d\n", orders[i].orderID);
        printf("Customer Name: %s\n", orders[i].customerName);
        printf("Order Status: %s\n", orders[i].orderStatus);
        printf("Total Amount: %.2f\n", orders[i].totalAmount);
        printf("Final Amount: %.2f\n", orders[i].finalAmount);
        printf("\nOrdered Items:\n");

         // Inner loop: visits an displays every food item belonging to the current order.
        // itemCount tells us how many items are stored in this order.

        for(int j=0;j<orders[i].itemCount;j++)
        {
            printf("Food ID: %d\n",orders[i].items[j].foodID);
            printf("Quantity: %d\n",orders[i].items[j].quantity);
            printf("Price: %.2f\n",orders[i].items[j].price);
        }
    }

    printf("\n-----------------------------\n");
}