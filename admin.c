#include<stdio.h>
#include "common.h" 

int compareStrings(char str1[],char str2[]) //same as strcmp in string.h
{

    int i=0;

    while(str1[i]!='\0'&&str2[i]!='\0') 
    {
        if(str1[i]!=str2[i])
        {
            return 0;
        }
        i++;
    }

    if(str1[i]=='\0'&&str2[i]=='\0')
    {
        return 1;
    }

    return 0;
}

int adminLogin(void)
{

    char adminID[20];
    char password[20];
    int attempts=3;
    printf("\n===== ADMIN LOGIN =====\n");

    while(attempts>0)
    {
        printf("Enter Admin ID: ");
        scanf("%19s",adminID); //AD001
        printf("Enter Password: ");
        scanf("%19s",password); //admin123

        if(compareStrings(adminID,"AD001")==1&&compareStrings(password,"admin123")==1)
        {
            printf("\nLogin Successful!\n");
            return 1;
        }
        else
        {
            attempts--;
            printf("\nIncorrect Admin ID or Password!\n");
            printf("Remaining Attempts: %d\n",attempts);
        }
    }

    printf("\nAccess Denied!\n");
    return 0;
    }
    
void adminDashboard(void)
{

    int choice;
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

        switch(choice)
        {
            case 1:
                addFoodItem();
                break;
            case 2:
                displayMenu();
                break;
            case 3:
                searchFoodItem();
                break;
            case 4:
                updateFoodItem();
                break;
            case 5:
                deleteFoodItem();
                break;
            case 6:
                displayAllOrders();
                break;
            case 7:
                updateOrderStatus();
                break;
            case 8:
                printf("\nLogging out...\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while(choice!=8);
}
// case 1
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
        if (food[i].categoryID==categoryID)
        {
            if(food[i].foodID%100>maxItemNumber)
            {
                maxItemNumber=food[i].foodID%100;
            }
        }
    }
    // Generate new Food ID
    if(maxItemNumber>=99)
    {
        printf("This category has reached its ID limit!\n");
        return;
    }
    newFoodID=categoryID*100+maxItemNumber+1;
    // Ask for food name
    printf("Enter Food Name: ");
    if(scanf(" %49[^\n]",newFoodName)!=1)
    {
        printf("Invalid food name!\n");
        return;
    }
    // Ask for food price
    printf("Enter Food Price: Rs. ");
    if(scanf("%f",&newPrice)!=1||newPrice<=0)
    {
        printf("Invalid price!\n");
        return;
    }
    // Store the new food item
    food[foodCount].foodID=newFoodID;
    food[foodCount].categoryID=categoryID;
    for (i=0;newFoodName[i]!='\0';i++)
    {
        food[foodCount].foodName[i]=newFoodName[i];
    }
    food[foodCount].foodName[i]='\0';
    food[foodCount].price=newPrice;
    // Increase food count
    foodCount++;
    saveFoodData(); //file write "w"
    printf("\nFood item added successfully!\n");
    printf("Generated Food ID: %d\n",newFoodID);

}
   
//search food item selected(case 3)
void searchFoodItem(void)
{

    int choice;
    int searchID;
    int found=0;
    int i;
    char searchName[50];
    printf("\n========== SEARCH FOOD ITEM ==========\n");
    printf("1. Search by Food ID\n");;
    printf("\nEnter your choice: ");
    scanf("%d",&choice);
    // Search by Food ID
    if(choice==1)
    {
        printf("\nEnter Food ID: ");
        scanf("%d",&searchID);
        for (i=0; i<foodCount;i++)
        {
            if (food[i].foodID==searchID)
            {
                found=1;
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
    if(found==0)
    {
        printf("\nFood Item Not Found!\n");
    }
}

// Update food item(case 4)
void updateFoodItem(void)
{

    int searchID;
    int i;
    int j;
    int found=0;
    char newFoodName[50];
    float newPrice;
    printf("\n========== UPDATE FOOD ITEM ==========\n");
    printf("Enter Food ID: ");
    if(scanf("%d",&searchID)!=1)
    {
        printf("\nInvalid Food ID!\n");
        return;
    }
    // Search food item
    for(i=0;i<foodCount;i++)
    {
        if(food[i].foodID==searchID)
        {
            found=1;
            printf("\nFood Item Found!\n");
            printf("Food ID: %d\n",food[i].foodID);
            printf("Food Name: %s\n",food[i].foodName);
            printf("Current Price: Rs. %.2f\n",food[i].price);
            // Enter new food name
            printf("\nEnter New Food Name: ");
            if (scanf(" %49[^\n]", newFoodName)!=1)
            {
                printf("\nInvalid Food Name!\n");
                return;
            }
            // Enter new food price
            printf("Enter New Food Price: Rs. ");
            if (scanf("%f",&newPrice)!=1||newPrice<=0)
            {
                printf("\nInvalid Price! Update Cancelled.\n");
                return;
            }
            // Update food name
            for(j=0;newFoodName[j]!='\0';j++)
            {
                food[i].foodName[j]=newFoodName[j];
            }
            food[i].foodName[j] = '\0';
            // Update food price
            food[i].price=newPrice;
            saveFoodData();
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

//case 5
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
    // Search food item by ID
    for(i=0;i<foodCount;i++)
    {
        if(food[i].foodID==searchID)
        {
            found=1;
            // Display food item details
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

void updateOrderStatus(void)
{
    int searchID,found=0,choice;
    char *status;
    loadOrderData();
    printf("\n========== UPDATE ORDER STATUS ==========\n");
    printf("Enter Order ID: ");
    scanf("%d",&searchID);

    for(int i=0;i<orderCount;i++)
    {
        if(orders[i].orderID==searchID)
        {
            found=1;
            printf("\nCustomer Name: %s\n", orders[i].customerName);
            printf("Current Status: %s\n", orders[i].orderStatus);
            printf("\n1. Pending\n");
            printf("2. Confirmed\n");
            printf("3. Preparing\n");
            printf("4. Completed\n");
            printf("5. Cancelled\n");
            printf("\nEnter new status: ");
            scanf("%d",&choice);

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
            int j;
            for(j=0;status[j]!='\0';j++)
            {
                orders[i].orderStatus[j] = status[j];
            }
            orders[i].orderStatus[j] = '\0';
            if(saveAllOrderData())
                printf("\nStatus saved successfully!\n");
            else
                printf("\nUnable to save status!\n");
            break;
        }
    }
    if(found==0)
        printf("\nOrder ID not found!\n");
}
void displayAllOrders(void)
{
    loadOrderData();
    printf("\n========== ALL CUSTOMER ORDERS ==========\n");
    if(orderCount==0)
    {
        printf("\nNo orders found!\n");
        return;
    }
    for(int i=0;i<orderCount;i++)
    {
        printf("\n-----------------------------\n");
        printf("Order ID: %d\n", orders[i].orderID);
        printf("Customer Name: %s\n", orders[i].customerName);
        printf("Order Status: %s\n", orders[i].orderStatus);
        printf("Total Amount: %.2f\n", orders[i].totalAmount);
        printf("Final Amount: %.2f\n", orders[i].finalAmount);
        printf("\nOrdered Items:\n");
        for(int j=0;j<orders[i].itemCount;j++)
        {
            printf("Food ID: %d\n",orders[i].items[j].foodID);
            printf("Quantity: %d\n",orders[i].items[j].quantity);
            printf("Price: %.2f\n",orders[i].items[j].price);
        }
    }
    printf("\n-----------------------------\n");
}