#include<stdio.h>
#include "common.h" 
void displayMenu()
{
    printf("\n");
    printf("============================================\n");
    printf("             RESTAURANT MENU\n");
    printf("============================================\n");
    for (int i=0;i<MAX_CATEGORIES;i++)
    {
        printf("\nCATEGORY: %s\n",categories[i].categoryName);
        printf("--------------------------------------------\n");
        printf("%-10s %-25s %s\n","Food ID","Food Name","Price");
        for (int j= 0;j<foodCount;j++)
        {
            if (food[j].categoryID == categories[i].categoryID)
            {
                printf("%-10d %-25s Rs. %.2f\n",food[j].foodID,food[j].foodName,food[j].price);
            }
        }
        printf("--------------------------------------------\n");
    }
}