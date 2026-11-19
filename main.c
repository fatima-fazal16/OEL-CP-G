#include <stdio.h>
#include "common.h"
// Includes the user-defined header file.
// It contains structures, macros, global variable declarations,
// and function prototypes shared by different project files.


// INITIALIZATION OF CATEGORIES

// Initialize all 7 categories.
// Each category contains a category ID and a category name.
// The values are assigned in the same order as the Category structure.
Category categories[MAX_CATEGORIES]={
    {1,"Starters"},
    {2,"Soups & Salads"},
    {3,"Fast Food"},
    {4,"Chinese Cuisine"},
    {5,"Pakistani Cuisine & BBQ"},
    {6,"Pizza & Pasta"},
    {7,"Desserts & Beverages"}
}; 

// INITIALIZATION OF FOOD ITEMS

// Initialize all 35 food items.
// Each food record contains:
// Food ID, Category ID, Food Name, and Price.
//
// The first digit(s) of the Food ID identify the category.
// The last two digits identify the individual food item.
// For example, Food ID 101 belongs to Category 1.
Food food[MAX_FOOD]=
{
    // Category 1: Starters
    {101,1,"Chicken Wings",650},
    {102,1,"Chicken Nuggets",500},
    {103,1,"French Fries",350},
    {104,1,"Dynamite Prawns",850},
    {105,1,"Cheese Balls",450},
    // Category 2: Soups & Salads
    {201,2,"Chicken Corn Soup",300},
    {202,2,"Hot & Sour Soup",350},
    {203,2,"Caesar Salad",650},
    {204,2,"Russian Salad",500},
    {205,2,"Fresh Garden Salad",350},
    // Category 3: Fast Food
    {301,3,"Zinger Burger",650},
    {302,3,"Beef Burger",750},
    {303,3,"Chicken Shawarma",400},
    {304,3,"Club Sandwich",550},
    {305,3,"Chicken Wrap",500},
    // Category 4: Chinese Cuisine
    {401,4,"Chicken Manchurian",950},
    {402,4,"Chicken Chow Mein",850},
    {403,4,"Chicken Fried Rice",750},
    {404,4,"Beef Chilli Dry",1200},
    {405,4,"Szechuan Chicken",1000},
    // Category 5: Pakistani Cuisine & BBQ
    {501,5,"Chicken Biryani",450},
    {502,5,"Chicken Karahi", 1400},
    {503,5,"Beef Nihari", 900},
    {504,5,"Chicken Tikka", 450},
    {505,5,"Malai Boti", 750},
    // Category 6: Pizza & Pasta
    {601,6,"Chicken Tikka Pizza",1200},
    {602,6,"Fajita Pizza",1300},
    {603,6,"Creamy Alfredo Pasta",850},
    {604,6,"Arrabbiata Pasta",800},
    {605,6,"Lasagna",1000},
    // Category 7: Desserts & Beverages
    {701,7,"Chocolate Lava Cake",550},
    {702,7,"Brownie with Ice Cream",600},
    {703,7,"Cheesecake",650},
    {704,7,"Chocolate Milkshake",450},
    {705,7,"Mint Margarita",400}
};

    int foodCount=35;         // Stores the current number of food items in the food array.
        // Initially, all 35 food items are available.


// GLOBAL ORDER STORAGE

// Creates an array capable of storing MAX_ORDERS Order structures.
// This array is shared by different functions of the project.
    Order orders[MAX_ORDERS];
    int orderCount=0;

// FUNCTION SAVE FOOD DATA
// Writes all food records from the food array into food.txt.
// The file is opened in write mode, so its previous contents are replaced.   

void saveFoodData(void)
{
    // File pointer used to access the food file.
    FILE *fp;
    int i;  // Loop counter used to access each food record.

    // Open food.txt in write mode.
    // "w" creates the file if it does not exist.
    // If it already exists, its previous contents are overwritten.
    fp=fopen("food.txt","w");
    // fopen returns NULL if the file could not be opened.
    if(fp==NULL)
    {
        printf("\nUnable to save food data!\n");
        return;
    }
     // Traverse all currently stored food records.
    for(i=0;i<foodCount;i++)
    {
        // Write the details of each food item into the file.
        // The | symbol separates individual fields.
        // \n moves the file pointer to the next line.
        fprintf(fp,"%d|%d|%s|%.2f\n",food[i].foodID,
            food[i].categoryID,
            food[i].foodName,
            food[i].price);
    }
     // Close the file after completing the file operation.
       // This ensures that the file is properly closed and data is flushed.
    fclose(fp);
}

// FUNCTION LOAD FOOD DATA
// Reads previously saved food records from food.txt
// and stores them in the food array.

void loadFoodData(void)
{
     // File pointer used to read data from food.txt.
    FILE *fp;
    // Starts at index 0 because the first record will be stored
    // at the first position of the food array.
    int i=0;
     // Open food.txt in read mode.
    // "r" allows the program to read existing file contents.
    fp=fopen("food.txt","r");
    if(fp==NULL)
    {
        saveFoodData();
          // Create the file and write the currently initialized food items.
        return;
          // Exit because there is no existing file to read.
    }
    while (i<MAX_FOOD)
    {
        // Read one food record from the file.
        // fscanf() returns the number of successfully read values.
        // Here, 4 means all four fields were read successfully.

        if (fscanf(fp,"%d|%d|%49[^|]|%f",&food[i].foodID,
            &food[i].categoryID,
            food[i].foodName,
            &food[i].price)!=4)
        {
            break;
        }
        i++;  // Move to the next array position for the next food record.
    }
    fclose(fp);
      // Check whether at least one food record was successfully loaded.
    if(i>0)
    {
        // Update foodCount to reflect the actual number of records
        // loaded from the file.
        foodCount=i;
    }
    else
    {
        saveFoodData();
    }
}   
// MAIN FUNCTION
// Execution of the entire Restaurant Order And Billing System begins here.
int main(void)
{
    int choice; // Stores whether the user wants the Customer or Admin portal.
    int result; // Stores the result returned by the adminLogin() function.

    // Load saved food information before displaying the main menu.
    // This ensures that previously added, updated, or deleted food items
    // are reflected when the program starts.
    loadFoodData();

    printf("\n================================\n");
    printf("RESTAURANT MANAGEMENT SYSTEM\n");
    printf("================================\n");
    printf("\n1. Customer\n");
    printf("2. Admin\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);
    if(choice==1)
    {
        printf("\nWelcome to Customer Portal!\n");
        // Calls the customer dashboard to display customer operations.
        customerDashboard();
    }
    else if(choice==2)
    {
        result=adminLogin();
        if(result==1) // adminLogin() returns 1 if credentials are correct.
        {
            printf("\nWelcome to Admin Portal!\n");
            // Open the administrator dashboard after successful login.
            adminDashboard();
        }
    }
     // Execute this block if the user enters an option other than 1 or 2.
    else
    {
        printf("\nInvalid choice! Exiting the program.\n");
    }
    return 0;
}