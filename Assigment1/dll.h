#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>


// Node - estrutura
typedef struct DLL_Node {
    uint16_t key;           // Chave - uint16
    unsigned char *data;    // Dados - unsigned char
    struct DLL_Node *previous;
    struct DLL_Node *next;

}DLL_Node;

//DLL - estrutura
typedef struct DLL{


}DLL;

// Headers das funções
void MyDLLInit(DLL *My_dll, uint8_t max_elements, );

bool MyDLLInsert();

bool MyDLLRemove();

unsigned char *MyDLLFind();

unsigned char *MyDLLFind_Next();

unsigned char *MyDLLFind_Previous();


