#include<stdio.h>
#include "common.h" 
int compareStrings(char str1[],char str2[])
{
    int i=0;
    while (str1[i]!='\0'&&str2[i]!='\0')
    {
        if (str1[i]!=str2[i])
        {
            return 0;
        }
        i++;
    }
    if (str1[i]=='\0'&&str2[i]=='\0')
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
        scanf("%19s",adminID);
        printf("Enter Password: ");
        scanf("%19s",password);
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
                printf("\nAdd Food Item selected.\n");
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