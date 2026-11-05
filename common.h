#ifndef COMMON_H
#define COMMON_H
#define MAX_FOOD 100
#define MAX_CATEGORIES 7
// Category structure
typedef struct
{
    int categoryID;
    char categoryName[50];
}Category;
// Food structure
typedef struct
{
    int foodID;
    int categoryID;
    char foodName[50];
    float price;
}Food;
// Global variable declarations
extern Category categories[MAX_CATEGORIES];
extern Food food[MAX_FOOD];
extern int foodCount;
// Function declarations
int adminLogin(void);
void adminDashboard(void);
void displayMenu(void);
void addFoodItem(void);
void searchFoodItem(void);
void updateFoodItem(void);
#endif