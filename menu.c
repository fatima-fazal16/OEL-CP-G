#include<stdio.h>
#include "common.h" 
// Includes the shared structures, global variables,
// constants, and function declarations used in the project.


// FUNCTION: DISPLAY MENU
// Displays all available food items, arranged according to their categories.
void displayMenu() 
{
     // Print an empty line to create spacing before the menu.
    printf("\n");
    printf("============================================\n");
    printf("             RESTAURANT MENU\n");
    printf("============================================\n");

     // OUTER LOOP:
    // Traverses all 7 categories stored in the categories array.
    // i represents the index of the current category.
    for (int i=0;i<MAX_CATEGORIES;i++)
    {
         // Display the name of the current category.
        // categories[i].categoryName accesses the categoryName
        // member of the Category structure at index i.
        printf("\nCATEGORY: %s\n",categories[i].categoryName);
        printf("--------------------------------------------\n");
        printf("%-10s %-25s %s\n","Food ID","Food Name","Price");

        // INNER LOOP:
        // Traverses all currently available food records.
        // j represents the index of the current food item. 
        for (int j= 0;j<foodCount;j++)
        {
            // Check whether the current food item belongs
            // to the category currently selected by the outer loop.
            if (food[j].categoryID==categories[i].categoryID)
            {
                printf("%-10d %-25s Rs. %.2f\n",food[j].foodID,food[j].foodName,food[j].price);
            }
        }
         // Print a separator after displaying all items
        // belonging to the current category.
        printf("--------------------------------------------\n");
    }
}