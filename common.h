#ifndef COMMON_H
#define COMMON_H
#define MAX_FOOD 100
#define MAX_CATEGORIES 7
#define MAX_ORDER_ITEMS 20
#define MAX_ORDERS 100

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

// Structure for Individual items in an order
typedef struct{
    int foodID;
    int quantity;
    float price;
}OrderItem;

// Structure for a complete customer order
typedef struct{
    int orderID;
    char customerName[50];
    OrderItem items[MAX_ORDER_ITEMS];
    int itemCount;
    float totalAmount;
    float discountPercent;
    float discountAmount;
    float finalAmount;
    char orderStatus[20];
}Order;


// Global variable declarations
extern Category categories[MAX_CATEGORIES];
extern Food food[MAX_FOOD];
extern int foodCount;
extern Order orders[MAX_ORDERS];
extern int orderCount;

// Function declarations
int adminLogin(void);
void adminDashboard(void);
void displayMenu(void);
void addFoodItem(void);
void searchFoodItem(void);
void updateFoodItem(void);
void deleteFoodItem(void);
void customerDashboard(void);
#endif