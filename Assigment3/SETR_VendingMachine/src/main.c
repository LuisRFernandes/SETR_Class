/**
 * @file main.c
 * @brief Simulação da máquina de estados gerada pelo itemis CREATE.
 */

#include <zephyr/kernel.h>          
#include <zephyr/device.h>          
#include <zephyr/devicetree.h>		
#include <zephyr/drivers/gpio.h>    
#include "../../SETR_VendingMachine/Vending_Machine_Final/src-gen/VendingMachine.h"
#include "../../SETR_VendingMachine/Vending_Machine_Final/src-gen/VendingMachine_required.h"

static struct gpio_callback button_cb_data;

static struct VendingMachine vm;

/* Get leds and buttons node IDs. Refer to the DTS file */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

#define BUT0_NODE DT_ALIAS(sw0)
#define BUT1_NODE DT_ALIAS(sw1)
#define BUT2_NODE DT_ALIAS(sw2)
#define BUT3_NODE DT_ALIAS(sw3)

/* Get the device pointer, pin number, and configuration flags for led0 and button 0. A build error on this line means your board is unsupported. */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

static const struct gpio_dt_spec but0 = GPIO_DT_SPEC_GET(BUT0_NODE, gpios);
static const struct gpio_dt_spec but1 = GPIO_DT_SPEC_GET(BUT1_NODE, gpios);
static const struct gpio_dt_spec but2 = GPIO_DT_SPEC_GET(BUT2_NODE, gpios);
static const struct gpio_dt_spec but3 = GPIO_DT_SPEC_GET(BUT3_NODE, gpios);


/**
 * @brief Associação das interrupções dos butões a funções da maquina de estados.
 *
 * @param dev   Dispositivo GPIO.
 * @param cb    Callback.
 * @param pins  Pinos de entrada da placa.
 */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    if (pins & BIT(but0.pin)) {
        vendingMachine_raise_botao1euro(&vm);
    }
    if (pins & BIT(but1.pin)) {
        vendingMachine_raise_botao2euros(&vm);
    }
    if (pins & BIT(but2.pin)) {
        vendingMachine_raise_botao3_browse(&vm);
    }
    if (pins & BIT(but3.pin)) {
        vendingMachine_raise_botao4_select(&vm);
    }
}

/**
 * @brief Inicializa os periféricos GPIO e callbacks.
 */
static void initialize_peripherals() {
    gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);

    gpio_pin_configure_dt(&but0, GPIO_INPUT);
    gpio_pin_configure_dt(&but1, GPIO_INPUT);
    gpio_pin_configure_dt(&but2, GPIO_INPUT);
    gpio_pin_configure_dt(&but3, GPIO_INPUT);

	gpio_pin_interrupt_configure_dt(&but0, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_pin_interrupt_configure_dt(&but1, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_pin_interrupt_configure_dt(&but2, GPIO_INT_EDGE_TO_ACTIVE);
	gpio_pin_interrupt_configure_dt(&but3, GPIO_INT_EDGE_TO_ACTIVE);
	
	gpio_init_callback(&button_cb_data, button_pressed, BIT(but0.pin) | BIT(but1.pin) | BIT(but2.pin) | BIT(but3.pin));
    gpio_add_callback(but0.port, &button_cb_data);
    gpio_add_callback(but1.port, &button_cb_data);
    gpio_add_callback(but2.port, &button_cb_data);
    gpio_add_callback(but3.port, &button_cb_data);
}

/**
 * @brief Controla os LEDs com base nos IDs fornecidos. 
 *
 * @param handle  Objeto do tipo VendingMachine para identificar a máquina de vendas associada a esta função
 * @param id      ID para controlar os LEDs.
 */
void vendingMachine_leds(VendingMachine* handle, const sc_integer id) {
    switch (id) {
        case 0: 
            gpio_pin_set(led0.port, led0.pin, 0); 
            gpio_pin_set(led1.port, led1.pin, 0); 
            gpio_pin_set(led2.port, led2.pin, 0);
            gpio_pin_set(led3.port, led3.pin, 0);
            break;
        case 1: 
            gpio_pin_set(led0.port, led0.pin, 0); 
            gpio_pin_set(led1.port, led1.pin, 1); 
            gpio_pin_set(led2.port, led2.pin, 0);
            gpio_pin_set(led3.port, led3.pin, 0);
            break;
        case 2: 
            gpio_pin_set(led0.port, led0.pin, 1); 
            gpio_pin_set(led1.port, led1.pin, 0); 
            gpio_pin_set(led2.port, led2.pin, 0);
            gpio_pin_set(led3.port, led3.pin, 0);
            break;
        case 3: 
            gpio_pin_set(led0.port, led0.pin, 1); 
            gpio_pin_set(led1.port, led1.pin, 1); 
            gpio_pin_set(led2.port, led2.pin, 0);
            gpio_pin_set(led3.port, led3.pin, 0);
            break;
        case 4:  
            gpio_pin_set(led2.port, led2.pin, 1);
            gpio_pin_set(led3.port, led3.pin, 0);
            break;
        case 5:  
            gpio_pin_set(led2.port, led2.pin, 0);
            gpio_pin_set(led3.port, led3.pin, 1);
            break;
        default:
            gpio_pin_set(led0.port, led0.pin, 1); 
            gpio_pin_set(led1.port, led1.pin, 1); 
            gpio_pin_set(led2.port, led2.pin, 1);
            gpio_pin_set(led3.port, led3.pin, 1);
            break;
    }
}

/**
 * @brief Função main.
 *
 * Inicia os periféricos e a máquina de venda em si.
 */
void main()
{

	initialize_peripherals();

	vendingMachine_init(&vm);
    vendingMachine_enter(&vm);

}