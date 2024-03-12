#include <stdio.h>
#include "dll.h"

int main(){
    DLL My_dll;
    unsigned char data[MAX_DATA_SIZE] = (10,20,30,40,50);
    uint16_t key;

    MyDLLInit(&My_dll, 10, sizeof(data));

    // Fazer um menu

    int menu_choice;
    while(1){

        printf("\n 1 - Print List");
        printf("\n 2 - Insert an element");
        printf("\n 3 - Remove an element");
        printf("\n 4 - Find element by key");
        printf("\n 5 - Test 5");
        printf("\n 6 - Test 6");

        switch (menu_choice){
        case 1:
            MyDLLPrint(&My_dll);
            break;
        case 2:
            printf("Insert a (key - data )");
            printf("Insert the key: ");
            scanf("%hu", &key);

            if(MyDLLInsert(&My_dll, key, data)){
                printf("Elemento inserido com sucesso\n");
            }
            else{
            printf("Erro ao inserir um elemento\n");
            }
        case 3:
            unsigned char *data_to_find = MyDLLFind(&My_dll, key);
            if (data_to_find) {
                printf("Elemento encontrado: %s\n", data_to_find);
            } else {
                printf("Elemento não encontrado.\n");
            }
    
    default:
        printf("Error! Invalid input. \n");
        break;
        }
    }


}