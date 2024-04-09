/* ******************************************************/
/*                                                      */
/*                                                      */
/*  Baseado no código fornecido pelo                    */
/* prof. Paulo Pedreiras.                               */
/* "Base code for Unit Testing                          */
/* Simple example of command processor                  */
/* for smart sensor node with 3 sensors"			    */
/*     													*/
/*														*/
/* ******************************************************/

#ifndef CMD_PROC_H_
#define CMD_PROC_H_

/* Constantes para o tamanho dos buffers UART */
#define UART_RX_SIZE 20 	/**< Tamanho máximo do buffer de receção da UART. */
#define UART_TX_SIZE 20 	/**< Tamanho máximo do buffer de transmissão da UART. */

/* Símbolos para marcar o início e o fim de um comando */
#define SOF_SYM '#'	        /**< Símbolo que representa o início do comando. */
#define EOF_SYM '!'          /**< Símbolo que representa o fim do comando. */

#define SUCCESS 0 
/* Códigos de erro */
#define EMPTY_STRING -1     /**< String vazia ou incompleta. */
#define CMD_NOT_FOUND -2     /**< Comando inválido. */
#define CS_ERROR -3          /**< Erro no checksum. */
#define WRONG_STR_FORMAT -4 /**< Formato de string errado. */
#define BUFFER_FULL -5       /**< Buffer cheio. */
#define SENSOR_ERROR -6      /**< Tipo de sensor errado*/

/* Function prototypes */

/* ************************************************************ */
/* Processes the chars in the RX buffer looking for commands 	*/
/* Returns:                                                     */
/*  	 0: if a valid command was found and executed           */
/* 		-1: if empty string or incomplete command found         */
/* 		-2: if an invalid command was found                     */
/* 		-3: if a CS error is detected (command not executed)    */
/* 		-4: if string format is wrong                           */
/* ************************************************************ */
int cmdProcessor(void);

/* ******************************** */
/* Adds a char to the RX buffer 	*/
/* I.e., the reception of commands 	*/
/* Returns: 				        */
/*  	 0: if success 		        */
/* 		-1: if buffer full	 	    */
/* ******************************** */
int rxChar(unsigned char car);

/* ************************************ */
/* Adds a char to the TX buffer 		*/
/* I.e., the tranmsisison of answers 	*/
/* Returns: 				        	*/
/*  	 0: if success 		        	*/
/* 		-1: if buffer full	 	    	*/
/* ************************************ */
int txChar(unsigned char car);

/* ************************* */
/* Resets the RX buffer		 */  
/* ************************* */
void resetRxBuffer(void);

/* ************************* */
/* Resets the TX buffer		 */  
/* ************************* */
void resetTxBuffer(void);

/* ************************************************ */
/* Return the data that would be sent by the sensor */  
/* ************************************************ */
void getTxBuffer(unsigned char * buf, int * len);

/* ************************************************ */
/* Computes the checksum of a given number of chars */
/* ************************************************ */ 
int calcChecksum(unsigned char * buf, int nbytes);

void emulateSensor(unsigned char sensorType, unsigned char *buf, int *len);


/**
 * @brief Estrutura para armazenar as amostras dos sensores emulados.
 * 
 * Esta estrutura armazena amostras de temperatura, humidade e CO2,
 * permitindo assim que os sensores possuam um histórico das últimas 20 amostras.
 */
typedef struct {
    int temperature[20];
    int humidity[20];
    int co2[20];
    int count;
} SensorSamples;

#endif
