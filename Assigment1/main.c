#include <stdio.h>
#include "dll.h"

int main(){
    DLL My_dll;
    unsigned char data[MAX_DATA_SIZE] = "A";
    uint16_t key;

    MyDLLInit(&My_dll, 10, sizeof(data));

    // Fazer um menu

    int menu_choice;
    while(1){

        printf("\n1 - Print List\n");
        printf("2 - Insert an element\n");
        printf("3 - Remove an element\n");
        printf("4 - Find element by key\n");
        printf("5 - Find next element\n");
        printf("6 - Find  previous element\n");
        printf("------------------\n");
        printf("Choose a number: ");
        scanf("%d", &menu_choice);

        switch (menu_choice){
        case 1:
            MyDLLPrint(&My_dll);
            break;
        case 2:
            printf("Insert a (key - data )");
            printf("Insert the key: ");
            scanf("%hu", &key);
            printf("Insert the data: ");
            scanf("%s", data);
            MyDLLInsert(&My_dll, key, data);
            break;
        case 3:
            printf("Insert a (key - data )");
            printf("Insert the key: ");
            scanf("%hu", &key);
            MyDLLRemove(&My_dll, key);
            break;

        case 4:
            printf("Find element by its key\n");
            printf("Insert the key: ");
            scanf("%hu", &key);
            unsigned char *element_data_find = MyDLLFind(&My_dll, key);
            if (element_data_find != NULL) {
                printf("Found data: %s\n", element_data_find);
            } else {
                printf("Element not found.\n");
            }
        break;

        case 5:
            printf("Find next element by its key\n");
            printf("Insert the key: ");
            scanf("%hu", &key);
            unsigned char *element_data_find_next = MyDLLFind_Next(&My_dll, key);
            if (element_data_find_next != NULL) {
                printf("Found data: %s\n", element_data_find_next);
            } else {
                printf("Element not found.\n");
            }

           
    break;
        
        case 6:
        printf("Find previous element by its key\n");
            printf("Insert the key: ");
            scanf("%hu", &key);
            unsigned char *element_data_find_previous = MyDLLFind_Previous(&My_dll, key);
            if (element_data_find_previous != NULL) {
                printf("Found data: %s\n", element_data_find_previous);
            } else {
                printf("Element not found.\n");
            }
        break;
    default:
        printf("Error! Invalid input. \n");
        break;
        }
    }


}