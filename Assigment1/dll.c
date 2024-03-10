#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void MyDLLInit(DLL *dll, uint8_t max_size, uint8_t nmr_elements)
{
    dll->head = NULL;
    dll->tail = NULL;
    dll->max_size = max_size;
    dll->nmr_elements = nmr_elements;
}