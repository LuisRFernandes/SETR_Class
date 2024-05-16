/**
 * @file VendingMmachine_required.c
 * @brief Implementação das funções relacionadas ás "operations" do itemis CREATE.
 */

#include "../src/sc_types.h"
#include "VendingMachine.h"
#include "VendingMachine_required.h"
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>



/**
 * @brief Exibe o estado atual da máquina de venda e um menu para user interface
 *
 * Esta função imprime informações sobre o estado atual da máquina de vendas, mais concretamente
 * o estado atual, o crédito e o produto.
 *
 * @param handle Objeto do tipo VendingMachine para identificar a máquina de vendas associada a esta função
 * @param state Estado atual da máquina de vendas.
 */
void vendingMachine_currentState(VendingMachine* handle, const sc_integer state) {
    const char* stateName;
    switch (state) {
        case 0:
            stateName = "State 0 - Operation";
            break;
        case 1:
            stateName = "State 1 - Refund";
            break;
        case 2:
            stateName = "State 2 -Buying";
            break;
        case 3:
            stateName = "State3 - Buy Rejected";
            break;
        default:
            stateName = "Unknown";
            break;
    }

    printk("\033[2J\033[H");
    printk("Estado atual: %s\n", stateName);
    printk("Credito: %d\n", vendingMachine_get_credito(handle));
    printk("Produto: %d\n", vendingMachine_get_produto(handle));
}
