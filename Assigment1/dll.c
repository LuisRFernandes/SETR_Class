#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dll.h"

/**
* @brief Inicialização da DLL.
* 
* @param My_dll Ponteiro para a DLL.
* @param max_elements Numero máximo de elementos da DLL.
* @param data_size Tamanho dos dados de cada nó.
*/
void MyDLLInit(DLL *My_dll, uint8_t max_elements, uint8_t data_size){
    My_dll->head = NULL;        
    My_dll->tail = NULL;
    My_dll->nmr_elements = 0;           
    My_dll->max_elements = (max_elements > MAX_ELEMENTS) ? MAX_ELEMENTS : max_elements;

    for (int i = 0; i < My_dll->max_elements; i++) {
        My_dll->nodes[i].key = 0; 
        My_dll->nodes[i].previous = NULL;
        My_dll->nodes[i].next = NULL;
        My_dll->nodes[i].data_size = data_size; 
    }

}

/**
* @brief Inserção de um elemento na DLL. 
* 
* @param My_dll Ponteiro para a DLL.
* @param key Chave - Identificadora de um elemento.
* @param data Dados do elemento a inserir.
*
* @return Retorna true se o elemento for inserido com sucesso, e retorna false caso não seja.
*/
bool MyDLLInsert(DLL *My_dll, uint16_t key, unsigned char *data) {
    // Verifica se a DLL está cheia
    if(My_dll->nmr_elements >= My_dll->max_elements){
        return false;   // Caso esteja cheia, retorna false
    }

    // Verifica de o valor da key é válido
    if(key > UINT16_MAX){
        return false;  
    }
    
    // Verificar que a key é única
    DLL_Node *Node_key = My_dll->head;
    while (Node_key != NULL)
    {
        if (Node_key->key == key)
        {
            return false;       // Quer dizer que a key já existe e não é única.
        }
        Node_key = Node_key->next;
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

/**
* @brief Remoção de um elemento na DLL
* 
* @param My_dll Ponteiro para a DLL
* @param key Identifica o elemento a remover
*
* @return Retorna true se o elemento for removido com sucesso, e retorna false caso não seja.
*/
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

/**
* @brief Encontra um elemento na DLL
* @param My_dll Ponteiro para a DLL
* @param key Identifica o elemento que se pretende encontrar
*
* @return Retorna um ponteiro para os dados do elemento encontrado, e caso não seja encontrado, retorna NULL
*/
unsigned char *MyDLLFind(DLL *My_dll, uint16_t key){
    DLL_Node *current_Node = My_dll->head; //Aponta para o início da DLL
    unsigned char *result = NULL;              //Variável para guardar o resultado da pesquisa
    while(current_Node != NULL){ //Percorre todos os nós da lista
        if (current_Node->key == key){ //Caso a chave seja encontrada
            result = current_Node->data;
            break;
        }
        current_Node = current_Node->next; // Se a chave não corresponder à chave daquele nó, passado ao próximo nó
    }
    return result;
}

/**
* @brief Encontra o próximo elemento na DLL.
* 
* @param My_dll Ponteiro para a DLL.
* @param current_Node Ponteiro para o nó atual da DLL.
*
* @return Retorna um ponteiro para os dados do próximo elemento, e caso não exista próximo retorna NULL.
*/
unsigned char* MyDLLFind_Next(DLL *My_dll, DLL_Node *current_Node){
    if(current_Node == NULL || current_Node->next == NULL){
        return NULL;  
    }   
    return current_Node->next->data;
}

/**
* @brief Encontra o elemento anterior na DLL.
* 
* @param My_dll Ponteiro para a DLL.
* @param current_Node Ponteiro para o nó atual da DLL.
*
* @return Retorna um ponteiro para os dados do elemento anterior, e caso não exista retorna NULL.
*/
unsigned char* MyDLLFind_Previous(DLL *My_dll, DLL_Node *current_Node){
    if(current_Node == NULL || current_Node->previous == NULL){
        return NULL;  
    }   
    return current_Node->previous->data;
}

/**
* @brief Lista os elementos da DLL.
* 
* @param My_dll Ponteiro para a DLL.
*/
void MyDLLPrint (DLL *My_dll){
    DLL_Node *current_Node = My_dll->head;
    while (current_Node != NULL)
    {
        printf("Key: %u  : ", current_Node->key);
        printf("Data: %s\n",current_Node->data);
        current_Node = current_Node->next;
    }
}
