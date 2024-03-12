/**
* @file dll.h
* @brief Doubly Linked List (DLL)
*/

#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>

#define MAX_ELEMENTS 30            // Numero maximo de elementos da dll
#define MAX_DATA_SIZE 100           // Tamanho maximo de dados de cada nó          

// Definir variaveis de erro : recomendação do professor
#define OK 0            // Variavel de sucesso
#define EMPTY_LIST -1   // 
#define LIST_FULL        

/**
* @brief Estrutura de um nó da DLL
*/
typedef struct DLL_Node {
    uint16_t key;  
    uint16_t data_size;               // tamanho do array de dados       
    unsigned char data[MAX_DATA_SIZE];               
    struct DLL_Node *previous;
    struct DLL_Node *next;
    
}DLL_Node;

/**
* @brief Estrutura da DLL
*/
typedef struct DLL{
    DLL_Node nodes[MAX_ELEMENTS];
    DLL_Node *head;         // Representa o primeiro nó da lista (cabeça)
    DLL_Node *tail;         // Representa o último nó da lista (cauda)
    uint16_t nmr_elements;  // Numero atual de elementos
    uint16_t max_elements;  // Numero máximo de elementos da lista
}DLL;

/**
* @brief Headers das funções
*/
void MyDLLInit(DLL *My_dll, uint8_t max_elements, uint8_t data_size ) ;

bool MyDLLInsert(DLL *My_dll, uint16_t key, unsigned char *data);

bool MyDLLRemove(DLL *My_dll, uint16_t key);

unsigned char *MyDLLFind(DLL *My_dll, uint16_t key);

unsigned char *MyDLLFind_Next(DLL *My_dll, uint16_t key);

unsigned char *MyDLLFind_Previous(DLL *My_dll, uint16_t key);

bool MyDLLPrint (DLL *My_dll);

