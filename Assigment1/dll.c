#include <stdint.h>+
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dll.h"

void MyDLLInit(DLL *My_dll, uint8_t max_elements, uint8_t data_size)
{
    My_dll->head = NULL;        
    My_dll->tail = NULL;
    My_dll->nmr_elements = 0;           
    My_dll->max_elements = (max_elements > MAX_ELEMENTS) ? MAX_ELEMENTS : max_elements;

    for (int i = 0; i < My_dll->max_elements; i++) {
        My_dll->nodes[i].key = NULL;
        My_dll->nodes[i].previous = NULL;
        My_dll->nodes[i].next = NULL;
        My_dll->nodes[i].data_size = data_size; 
    }

}

bool MyDLLInsert(DLL *My_dll, uint16_t key, unsigned char *data) {
    // Verifica se a DLL está cheia
    if(My_dll->nmr_elements >= My_dll->max_elements){
        return false;   // Caso esteja cheia, retorna false
    }

    // Verifica de o valor da key é válido
    if(key > UINT16_MAX){
        return false;  
    }

    // Ultimo nó da lista
    DLL_Node *New_node = &My_dll->nodes[My_dll->nmr_elements];
    New_node->key = key;

    for(int i= 0; i < My_dll->nodes[My_dll->nmr_elements].data_size; i++){
        New_node->data[i]=data[i];
    }

    // Caso a DLL estiver vazia
    if(My_dll->head == NULL){               // Head == NULL, significa que a lista está vazia
        My_dll->head = New_node;            // A head da DLL, passa a ser o novo nó.
        My_dll->tail = New_node;            // A tail da DLL, passa a ser o novo nó.
        New_node->previous = NULL;          // Como é o unico nó, não existe nenhum previous
        New_node->next = NULL;              // Como é o unico nó, não existe nenhum next
    }
    else{
        New_node->previous = My_dll->tail;  // O campo previous do novo nó, vai ser 'antiga' tail da dll
        New_node->next = NULL;              // Como o nó é inserido no final da DLL, o campo next é nulo.
        My_dll->tail->next = New_node;      // A tail 'antiga' tem um campo next que vai apontar para o novo nó introduzido.
        My_dll->tail = New_node;            // Atualiza a tail da DLL para o novo nó.
    }

    My_dll->nmr_elements++;
    return true;
}


bool MyDLLRemove(DLL *My_dll, uint16_t key){
    DLL_Node *current_Node = My_dll->head;     // Aponta o "Nó atual" para o inicio da DLL

    while(current_Node != NULL){       // Enquanto o nó de teste "current_Node" apontar para algum nó da DLL
        if(current_Node->key == key){
            // Caso o elemento seja o primeiro da lista
            if(current_Node->previous == NULL)  {
                My_dll->head =current_Node->next;   // A head da DLL para a ser o segundo elemento, traduzindo current_Node.next.
                
                // Ao remover o primeiro da lista, e a lista nao ficar vazia
                if(My_dll->head != NULL){   
                    My_dll->head->previous = NULL; // Remove o campo previous da head da DLL.
                }
            }

            // Se removermos o ultimo elemento da lista.
            else if (current_Node->next == NULL )
            {
                My_dll->tail = current_Node->previous;      // Ao se remover o último nó, a tail fica a apontar para o anterior
                if(My_dll->tail !=NULL){
                    My_dll->tail->next = NULL;
                }
            }
            // Remove um nó do meio da lista
            else{
                // Atualização os ponteiros dos campos previous e next do nó ser removido.
                current_Node->previous->next = current_Node->next; 
                current_Node->next->previous = current_Node->previous;
            }

            My_dll->nmr_elements--;     
            return true;        // Nó removido com sucesso
        }
        current_Node = current_Node->next;  // Atualiza o nó de teste para apontar para a posição seguinte
    }
    return false;   // Key nao encontrada, return false.
}



