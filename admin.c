#include<stdio.h>
#include "common.h" 
int compareStrings(char str1[],char str2[])
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
        printf("6. View Customer Orders\n");
        printf("7. Assign Orders to Waiters\n");
        printf("8. View Sales Report\n");
        printf("9. Logout\n");
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
                printf("\nSearch Food Item selected.\n");
                break;
            case 4:
                printf("\nUpdate Food Item selected.\n");
                break;
            case 5:
                printf("\nDelete Food Item selected.\n");
                break;
            case 6:
                printf("\nView Customer Orders selected.\n");
                break;
            case 7:
                printf("\nAssign Orders to Waiters selected.\n");
                break;
            case 8:
                printf("\nView Sales Report selected.\n");
                break;
            case 9:
                printf("\nLogging out...\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
        }
    } while(choice!=9);
}
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
    for (i=0;i<foodCount;i++)
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
    if (scanf(" %49[^\n]",newFoodName)!=1)
    {
        printf("Invalid food name!\n");
        return;
    }
    // Ask for food price
    printf("Enter Food Price: Rs. ");
    if (scanf("%f",&newPrice)!=1||newPrice<=0)
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
    food[foodCount].foodName[i] = '\0';
    food[foodCount].price = newPrice;
    // Increase food count
    foodCount++;
    printf("\nFood item added successfully!\n");
    printf("Generated Food ID: %d\n",newFoodID);
}