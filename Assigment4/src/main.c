/**
 * @file main.c
 * @brief Hardware control module using Zephyr OS with RTDB.
 *
 */

/* Zephyr system libraries */
#include <zephyr/kernel.h>          
#include <zephyr/device.h>          
#include <zephyr/devicetree.h>		
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include <zephyr/timing/timing.h>
#include <zephyr/drivers/adc.h>

/* ADC channel specification obtained from devicetree */
static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));
static int16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		.buffer_size = sizeof(buf),
	};


/* Error codes */
#define ERR_OK  0       // No errors
#define ERR_RDY -1      // Device not ready error
#define ERR_CONF -2     // Configuration error 

/* Size of stack area used by each thread (can be thread specific, if necessary)*/
#define STACK_SIZE 1024

/* Thread scheduling priority (Natural numbers)*/
#define thread_BUT_prio 3
#define thread_LED_prio 3
#define thread_PRINT_MENU_prio 2
#define thread_TEMP_SENSOR_prio 3

/* Therad periodicity (in ms)*/
#define thread_BUT_period 200   
#define thread_LED_period 200   
#define thread_PRINT_MENU_period 200
#define thread_TEMP_SENSOR_period 200   


/* RX buffer size */
#define RXBUF_SIZE 10

/* Inactivity period after the instant when last char was received that triggers an rx event (in us) */
#define RX_TIMEOUT 1000


/* Get leds and buttons node IDs. Refer to the DTS file */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

#define BUT0_NODE DT_ALIAS(sw0)
#define BUT1_NODE DT_ALIAS(sw1)
#define BUT2_NODE DT_ALIAS(sw2)
#define BUT3_NODE DT_ALIAS(sw3)


/* UART configuration structure */
const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
};

/* UART device structure for interacting with the UART API */
const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

/* Define the transmission buffer to hold the data to be sent over UART */
static uint8_t tx_buf[] =   {""};

/* Define the receive buffer */
static uint8_t rx_buf[RXBUF_SIZE] = {0};


/* Create thread stack space */
K_THREAD_STACK_DEFINE(thread_BUT_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_LED_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_PRINT_MENU_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_TEMP_SENSOR_stack, STACK_SIZE);

/* Create variables for thread data */

struct k_thread thread_BUT_data;
struct k_thread thread_LED_data;
struct k_thread thread_PRINT_MENU_data;
struct k_thread thread_TEMP_SENSOR_data;

/* Create task IDs */
k_tid_t thread_BUT_tid;
k_tid_t thread_LED_tid;
k_tid_t thread_PRINT_MENU_tid;
k_tid_t thread_TEMP_SENSOR_tid;

/* Functions/threads prototypes */
void thread_BUT_code(void *argA, void *argB, void *argC);
void thread_LED_code(void *argA, void *argB, void *argC);
void thread_PRINT_MENU_code(void *argA, void *argB, void *argC);
void thread_TEMP_SENSOR_code(void *argA, void *argB, void *argC);

int HWInit(void);
void resetCmdString(void);
int cmdProcessor(void);
int newCmdChar(unsigned char newChar);
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data);

/* Define a mutex for thread synchronization - RTDB*/
K_MUTEX_DEFINE(mutex);


/* Get device pointers, pin numbers, and configuration flags for LEDs and buttons */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

static const struct gpio_dt_spec but0 = GPIO_DT_SPEC_GET(BUT0_NODE, gpios);
static const struct gpio_dt_spec but1 = GPIO_DT_SPEC_GET(BUT1_NODE, gpios);
static const struct gpio_dt_spec but2 = GPIO_DT_SPEC_GET(BUT2_NODE, gpios);
static const struct gpio_dt_spec but3 = GPIO_DT_SPEC_GET(BUT3_NODE, gpios);

static struct gpio_callback button_cb_data;


/* Callback function and variables for button presses, leds and ADC values*/
/*RTDB Variables*/
volatile int But0_cb_data = 0;      
volatile int But1_cb_data = 0;      
volatile int But2_cb_data = 0;      
volatile int But3_cb_data = 0;  

volatile int led0stat = 0; 
volatile int led1stat = 0; 
volatile int led2stat = 0; 
volatile int led3stat = 0; 

volatile int adc_val_RAW = 0;
volatile int adc_value = 0; 

/**
 * @brief Callback function for button presses
 * 
 * This function is called when a button is pressed and updates the status
 * of the LEDs based on the button pressed.
 * 
 * @param dev Pointer to the device structure
 * @param cb  Callback structure
 * @param pins Pins to trigger the callback
 */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    
    if(BIT(but0.pin) & pins) {
        led0stat = 1; 
        led1stat = 0; 
        led2stat = 0; 
        led3stat = 0;
    }

    if(BIT(but1.pin) & pins) {
        led0stat = 1; 
        led1stat = 1; 
        led2stat = 0; 
        led3stat = 0;
    }

    if(BIT(but2.pin) & pins) {
        led0stat = 1; 
        led1stat = 1; 
        led2stat = 1; 
        led3stat = 0;
    }

    if(BIT(but3.pin) & pins) {
        led0stat = 1; 
        led1stat = 1; 
        led2stat = 1; 
        led3stat = 1;
    }
}


/**
 * @brief Main function
 * 
 * Initializes the hardware and starts the threads.
 * 
 * @return int Status code
 */
int main(void){
    HWInit();

    /* Create the task */
    thread_BUT_tid = k_thread_create(&thread_BUT_data, thread_BUT_stack,
        K_THREAD_STACK_SIZEOF(thread_BUT_stack), thread_BUT_code,
        NULL, NULL, NULL, thread_BUT_prio, 0, K_NO_WAIT);

    thread_LED_tid = k_thread_create(&thread_LED_data, thread_LED_stack,
        K_THREAD_STACK_SIZEOF(thread_LED_stack), thread_LED_code,
        NULL, NULL, NULL, thread_LED_prio, 0, K_NO_WAIT);
	
	thread_PRINT_MENU_tid = k_thread_create(&thread_PRINT_MENU_data, thread_PRINT_MENU_stack,
        K_THREAD_STACK_SIZEOF(thread_PRINT_MENU_stack), thread_PRINT_MENU_code,
        NULL, NULL, NULL, thread_PRINT_MENU_prio, 0, K_NO_WAIT);

    thread_TEMP_SENSOR_tid = k_thread_create(&thread_TEMP_SENSOR_data, thread_TEMP_SENSOR_stack,
        K_THREAD_STACK_SIZEOF(thread_TEMP_SENSOR_stack), thread_TEMP_SENSOR_code,
        NULL, NULL, NULL, thread_TEMP_SENSOR_prio, 0, K_NO_WAIT);
 
}

/**
 * @brief Function to initialize the hardware
 * 
 * This function initializes the GPIO, UART, and ADC peripherals.
 * 
 * @return int Status code
 */
int HWInit(void) {

    int ret=0;   

    /* Check if Leds devices are ready */
	if (!device_is_ready(led0.port))  
	{
        printk("Fatal error: led0 device not ready!");
		return ERR_RDY;
	}
    if (!device_is_ready(led1.port))  
	{
        printk("Fatal error: led1 device not ready!");
		return ERR_RDY;
	}
    if (!device_is_ready(led2.port))  
	{
        printk("Fatal error: led2 device not ready!");
		return ERR_RDY;
	}
    if (!device_is_ready(led3.port))  
	{
        printk("Fatal error: led3 device not ready!");
		return ERR_RDY;
	}
 
    /* Check if buttons devices are ready */
    if (!device_is_ready(but0.port))  
	{
        printk("Fatal error: but0 device not ready!");
		return ERR_RDY;
	}
    if (!device_is_ready(but1.port))  
	{
        printk("Fatal error: but1 device not ready!");
		return ERR_RDY;
	}
    if (!device_is_ready(but2.port))  
	{
        printk("Fatal error: but2 device not ready!");
		return ERR_RDY;
	}
    if (!device_is_ready(but3.port))  
	{
        printk("Fatal error: but3 device not ready!");
		return ERR_RDY;
	}

    /* Configure leds IOs */
    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure led0 \n\r");
	    return ERR_CONF;
    }
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure led1 \n\r");
	    return ERR_CONF;
    }
    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure led2 \n\r");
	    return ERR_CONF;
    }
    ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure led3 \n\r");
	    return ERR_CONF;
    }

    /* Configure buttons IOs */
    ret = gpio_pin_configure_dt(&but0, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Failed to configure but0 \n\r");
	    return ERR_CONF;
    }
    ret = gpio_pin_configure_dt(&but1, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Failed to configure but1 \n\r");
	    return ERR_CONF;
    }
    ret = gpio_pin_configure_dt(&but2, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Failed to configure but2 \n\r");
	    return ERR_CONF;
    }
    ret = gpio_pin_configure_dt(&but3, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
        printk("Failed to configure but3 \n\r");
	    return ERR_CONF;
    }
    /* Check if UART device is ready */
    if (!device_is_ready(uart)) {
        printk("UART device not ready\r\n");
        return ERR_RDY;
    }

    /* Configure UART */
    ret = uart_configure(uart, &uart_cfg);
    if (ret == -ENOSYS) { /* If invalid configuration */
        printk("uart_configure() error. Invalid configuration\n\r");
        return ERR_CONF; 
    }

    /* Register callback */
    ret = uart_callback_set(uart, uart_cb, NULL);
    if (ret) {
        printk("uart_callback_set() error. Error code:%d\n\r",ret);
        return ERR_CONF;
    }

    /* Enable data reception */
    ret =  uart_rx_enable(uart ,rx_buf,sizeof(rx_buf),RX_TIMEOUT);
    if (ret) {
        printk("uart_rx_enable() error. Error code:%d\n\r",ret);
        return ERR_CONF;
    }
 
    /* Last arg is timeout. Only relevant if flow controll is used */
    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
    if (ret) {
        printk("uart_tx() error. Error code:%d\n\r",ret);
        return ERR_CONF;
    }

    // INTERRUPTS 
    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure_dt(&but0, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	    printk("Error %d: failed to configure interrupt on BUT0 pin \n\r", ret);
	    return ERR_CONF;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure_dt(&but1, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	    printk("Error %d: failed to configure interrupt on BUT1 pin \n\r", ret);
	    return ERR_CONF;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure_dt(&but2, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	    printk("Error %d: failed to configure interrupt on BUT2 pin \n\r", ret);
	    return ERR_CONF;
    }

    /* Set interrupt HW - which pin and event generate interrupt */
    ret = gpio_pin_interrupt_configure_dt(&but3, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret != 0) {
	    printk("Error %d: failed to configure interrupt on BUT3 pin \n\r", ret);
	    return ERR_CONF;
    }
    
    /* Set callback */
    gpio_init_callback(&button_cb_data, button_pressed, BIT(but0.pin)| BIT(but1.pin)| BIT(but2.pin) | BIT(but3.pin));
    
    gpio_add_callback(but0.port, &button_cb_data);
    gpio_add_callback(but1.port, &button_cb_data);
    gpio_add_callback(but2.port, &button_cb_data);
    gpio_add_callback(but3.port, &button_cb_data);


    if (!adc_is_ready_dt(&adc_channel)) {
	printk("ADC controller devivce %s not ready", adc_channel.dev->name);
	return ERR_RDY;

    ret = adc_channel_setup_dt(&adc_channel);
    if (ret < 0) {
        printk("Could not setup channel #%d (%d)", 0, ret);
        return ERR_CONF;
    }

    ret = adc_sequence_init_dt(&adc_channel, &sequence);
	if (ret < 0) {
		printk("Could not initalize sequence");
		return ERR_CONF;
	}
}

    /* HW successfully initialized */
    return ERR_OK;
}


/**
 * @brief UART callback function
 * 
 * 
 * @param dev Pointer to the device structure
 * @param evt Pointer to the "UART event" structure
 * @param user_data User data
 */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data) {
	
    switch (evt->type) {

	case UART_RX_RDY:   // Not implemented
        break;

	case UART_RX_DISABLED:
		uart_rx_enable(dev ,rx_buf,sizeof rx_buf,RX_TIMEOUT);
		break;
		
	default:
		break;
    }
}
/**
 * @brief Thread function for button handling
 * 
 * This thread periodically reads the state of the buttons.
 * 
 * @param argA Not used
 * @param argB Not used
 * @param argC Not used
 */
void thread_BUT_code(void *argA , void *argB, void *argC){
/* Local vars */
    int64_t fin_time=0, release_time=0;     /* Timing variables to control task periodicity */  
        
    /* Task init code */
    printk("Thread BUT init (periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_BUT_period;

    /* Thread loop */
    while(1) {  

        // RTDB Sync - locking the mutex
        k_mutex_lock(&mutex, K_FOREVER);

        But0_cb_data = gpio_pin_get_dt(&but0);
        But1_cb_data = gpio_pin_get_dt(&but1);
        But2_cb_data = gpio_pin_get_dt(&but2);
        But3_cb_data = gpio_pin_get_dt(&but3);

        // RTDB Sync - unlocking the mutex
        k_mutex_unlock(&mutex);

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_BUT_period;
        }
    }
    /* Stop timing functions */
    timing_stop();
}
/**
 * @brief Thread function for LED control
 * 
 * This thread periodically updates the state of the LEDs.
 * 
 * @param argA Not used
 * @param argB Not used
 * @param argC Not used
 */
void thread_LED_code(void *argA, void *argB, void *argC){
/* Local vars */
    int64_t fin_time=0, release_time=0;     /* Timing variables to control task periodicity */  
        
    /* Task init code */
    printk("Thread LED init (periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_BUT_period;

    /* Thread loop */
    while(1) {  
        // RTDB Sync - locking the mutex
        k_mutex_lock(&mutex, K_FOREVER);

        gpio_pin_set_dt(&led0,led0stat);
        gpio_pin_set_dt(&led1,led1stat);
        gpio_pin_set_dt(&led2,led2stat);
        gpio_pin_set_dt(&led3,led3stat);

        // RTDB Sync - unlocking the mutex
        k_mutex_unlock(&mutex);

        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_BUT_period;
        }
    }
    /* Stop timing functions */
    timing_stop();
}

/**
 * @brief Thread function to print the menu
 * 
 * This thread periodically prints the menu with the current state of the buttons, LEDs and the ADC value
 * for the temperature sensor
 * 
 * @param argA Not used
 * @param argB Not used
 * @param argC Not used
 */
void thread_PRINT_MENU_code(void *argA , void *argB, void *argC)
{
    /* Local vars */
    int64_t fin_time=0, release_time=0;     /* Timing variables to control task periodicity */    
        
    /* Task init code */
    printk("Thread PRINT_MENU (periodic)\n");

    /* Compute next release instant */
    release_time = k_uptime_get() + thread_PRINT_MENU_period;

    /* Thread loop */
    while(1) {   

        printk("\033[2J\033[H");  
       /* Print menu */       
	    printk("   Button state || Button 1 : %d |  Button 2 : %d |  Button 3 : %d |  Button 4 : %d ||\n\n\r",But0_cb_data,But1_cb_data,But2_cb_data,But3_cb_data);
	    printk("   Led state    || Led 1 :    %d |  Led 2 :    %d |  Led 3 :    %d |  Led 4    : %d ||\n\n\r",led0stat,led1stat,led2stat,led3stat);
		printk("   TemperatureSensor : %d C\n\r", adc_value);
		printk("   Temperature RAW : %d C\n\r", adc_val_RAW);
  
        /* Wait for next release instant */ 
        fin_time = k_uptime_get();
        if( fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_PRINT_MENU_period;

        }
    }

    /* Stop timing functions */
    timing_stop();
}

/**
 * @brief Thread function for temperature sensor reading
 * 
 * This thread periodically reads the ADC value, simulation a temperature sensor
 * 
 * @param argA Not used
 * @param argB Not used
 * @param argC Not used
 */
void thread_TEMP_SENSOR_code(void *argA, void *argB, void *argC) {
    int64_t fin_time = 0, release_time = 0;

    printk("Thread temperature Sensor read (periodic)\n");

    release_time = k_uptime_get() + thread_TEMP_SENSOR_period;

    while (1) {
        // RTDB Sync
         k_mutex_lock(&mutex, K_FOREVER);
         
        adc_val_RAW = adc_read(adc_channel.dev, &sequence);
        if (adc_val_RAW < 0) {
            printk("Could not read ADC (%d)\n", adc_val_RAW);
        } else {
            adc_value = (int)buf * 3000 / 1023; // Conversão para mV
        }
        
        // RTDB Sync
        k_mutex_unlock(&mutex);

        fin_time = k_uptime_get();
        if (fin_time < release_time) {
            k_msleep(release_time - fin_time);
            release_time += thread_TEMP_SENSOR_period;
        }
    }
    timing_stop();
}
