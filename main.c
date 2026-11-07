#include <stdio.h>
#include "common.h"
// Initialize all 7 categories
Category categories[MAX_CATEGORIES]={
    {1,"Starters"},
    {2,"Soups & Salads"},
    {3,"Fast Food"},
    {4,"Chinese Cuisine"},
    {5,"Pakistani Cuisine & BBQ"},
    {6,"Pizza & Pasta"},
    {7,"Desserts & Beverages"}
}; 
// Initializing all 35 food items
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
    int foodCount=35;
    //Global order storage
    Order orders[MAX_ORDERS];
    int orderCount=0;
void saveFoodData(void)
{
    FILE *fp;
    int i;
    fp=fopen("food.txt","w");
    if(fp==NULL)
    {
        printf("\nUnable to save food data!\n");
        return;
    }
    for (i=0;i<foodCount;i++)
    {
        fprintf(fp,"%d|%d|%s|%.2f\n",food[i].foodID,food[i].categoryID,food[i].foodName,food[i].price);
    }
    fclose(fp);
}
void loadFoodData(void)
{
    FILE *fp;
    int i=0;
    fp=fopen("food.txt","r");
    if(fp==NULL)
    {
        saveFoodData();
        return;
    }
    while (i<MAX_FOOD)
    {
        if (fscanf(fp,"%d|%d|%49[^|]|%f",&food[i].foodID,&food[i].categoryID,food[i].foodName,&food[i].price)!=4)
        {
            break;
        }
        i++;
    }
    fclose(fp);
    if (i>0)
    {
        foodCount = i;
    }
    else
    {
        saveFoodData();
    }
}   
int main(void)
{
    int choice;
    int result;
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
        customerDashboard();
    }
    else if(choice==2)
    {
        result=adminLogin();
        if(result==1)
        {
            printf("\nWelcome to Admin Portal!\n");
            adminDashboard();
        }
    }
    else
    {
        printf("\nInvalid choice! Exiting the program.\n");
    }
    return 0;
}