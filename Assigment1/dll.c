#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dll.h"

void MyDLLInit(DLL *My_dll, uint8_t max_size)
{
    My_dll->head = NULL;
    My_dll->tail = NULL;
    My_dll->max_size = max_size;
    My_dll->nmr_elements = 0;
}

bool MyDLLInsert(DLL *My_dll, uint16_t key, unsigned char *data, uint8_t data_size) {
    // Verifica se a lista já está cheia
    if (My_dll->nmr_elements >= My_dll -> max_size) {
        return false;
    }

    DLL_Node newNode;
    newNode->key  = key;
    newNode->data = data; 
    newNode->previous = NULL;
    newNode->next = NULL;

    // Se a lista estiver vazia, head==tail
    if (My_dll->head == NULL) {
        My_dll->head = newNode;
        My_dll->tail = newNode;
    } else {
        newNode->previous = My_dll->tail;
        My_dll->tail->next = newNode;
        My_dll->tail = newNode;
    }
   My_dll->nmr_elements++;
    return true; 
}
