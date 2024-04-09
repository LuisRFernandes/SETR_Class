/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include <stdio.h>
#include <string.h>

#include "cmdproc.h"



/**
 * @brief Inicialização da estrutura de dados 
 */
SensorSamples sensorData = {
    .temperature = {0},
    .humidity = {0},
    .co2 = {0},
    .count = 0
};

static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static unsigned char txBufLen = 0; 

static int sensorCounter = 0;


/* Function implementation */

/**
 * @brief Processa os caracteres recebidos através do buffer de receção da UART 
 * 
 * @return  0 se um comando válido foi encontrado e executado,
 *         -1 (EMPTY_STRING) se a string de comando estiver vazia ou incompleta,
 *         -2 (CMD_NOT_FOUND) se um comando inválido for encontrado,
 *         -3 (CS_ERROR) se um erro de checksum for detectado,
 *         -4 (WRONG_STR_FORMAT) se o formato da string estiver errado.
 */ 
int cmdProcessor(void)
{
	int i;
	int j;
	unsigned char sid;
	unsigned char cmdData[UART_RX_SIZE];
	int cmdDataLen = 0;
	unsigned char sensorValues[UART_RX_SIZE]; 
    int sensorValuesLen; 
	unsigned char sensorValue[UART_RX_SIZE];
	int sensorValueLen;
		
	// DEBBUG: Verificar Rx Buffer
	printf("Rx Buffer:");
	for(int i = 0;i<rxBufLen;i++){
		printf("%c",UARTRxBuffer[i]);
	}
	printf("\n");

	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return EMPTY_STRING; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
        for(j=i+1; j < rxBufLen; j++) {
            if(UARTRxBuffer[j] == EOF_SYM) {
                break;
            }
            cmdData[cmdDataLen++] = UARTRxBuffer[j];
        }

		// DEBBUG : Buffer de transmissão
        printf("Tx Buffer:");
        for (int k = 0; k < cmdDataLen; k++) {
            printf("%c", cmdData[k]);
        }
        printf("\n");

		if(i < rxBufLen) {
			switch(UARTRxBuffer[i+1]) { 
				
				case 'A': 
					printf("Comando A \n");
					emulateSensor('t', sensorValues, &sensorValuesLen);
					resetTxBuffer(); // Limpa o buffer de transmissão antes de adicionar a nova resposta
					txChar('#');
					txChar('A');
					for (int k = 0; k < sensorValuesLen; k++) {
						txChar(sensorValues[k]);
					}
					txChar('!');
					txBufLen = sensorValuesLen + 3; // Atualiza o comprimento do buffer de transmissão
					break;



				case 'P':	// Reads the real-time value of one of the sensors

					if (i + 2 >= rxBufLen) {
						printf("CmdProcessor: Incomplete command\n");
						return CMD_NOT_FOUND; // Comando incompleto
					}
                    
					sid = UARTRxBuffer[i+2]; // Tipo de sensor
					printf("CmdProcessor: Sensor type: %c\n", sid);
					if (sid != 't' && sid != 'h' && sid != 'c') {
						printf("CmdProcessor: Invalid sensor type\n");
						return SENSOR_ERROR; // Tipo de sensor inválido
					}

					/* Check EOF */
					if(UARTRxBuffer[j-1] != EOF_SYM) {
    				printf("CmdProcessor: Wrong string format\n");
    				return WRONG_STR_FORMAT;
					}

					emulateSensor(sid, sensorValue, &sensorValueLen);
					txChar('#');
					txChar('p');
					txChar(sid);
					for(int k=0; k<sensorValueLen; k++) {
						txChar(sensorValue[k]);
					}
					txChar('!');
					
					printf("CmdProcessor: Command P\n");
					break;


									
				case 'L': // Não implementado
				
                case 'R': // Não implementado

                default:
                    return CMD_NOT_FOUND; // Unrecognized command
            }	
			
		}
		return 0;
	}
	
	/* Cmd string not null and SOF not found */
	return WRONG_STR_FORMAT;
}

/**
 * @brief Calculo e validação do valor de checksum.
 * 
 * @param buf Buffer que contém os caracteres.
 * @param nbytes Número de caracteres a serem considerados para o cálculo do checksum.
 * @return Devolve o valor do checksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
    int sum = 0;
    for (int i = 0; i < nbytes-1 ; i++) {
        sum += buf[i];
		//printf("Valor de buf[%d] : %d\n",i, buf[i]);
        //printf("Valor de buf[%d]: %c, Soma parcial: %d\n", i, buf[i], sum);
    }
    sum %= 256; // Garante que o valor 'sum' esteja entre 0-255
    printf("Soma final antes da conversão ASCII: %03d\n", sum);

    // Conversão para código ASCII e validação/comparação com o valor de checksum recebido
    char ascii_Sum[4];
    sprintf(ascii_Sum, "%03d", sum);
    printf("Checksum calculado em ASCII: %s\n", ascii_Sum);
    
	return sum;
}



/**
 * @brief Adiciona um caractere ao buffer de receção da UART.
 * 
 * @param car Caractere a ser adicionado ao buffer de receção.
 * @return  0 se o caractere for adicionado com sucesso,
 *         -5 (BUFFER_FULL) se o buffer estiver cheio.
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) {
		UARTRxBuffer[rxBufLen] = car;
		rxBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return BUFFER_FULL;
}

/**
 * @brief Adiciona um caractere ao buffer de transmissão da UART.
 * 
 * @param car Caractere a ser adicionado ao buffer de transmissão.
 * @return  0 se o caractere for adicionado com sucesso,
 *         -5 (BUFFER_FULL) se o buffer estiver cheio.
 */
int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufLen < UART_TX_SIZE) {
		UARTTxBuffer[txBufLen] = car;
		txBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/**
 * @brief Reset do buffer de receção da UART
 */
void resetRxBuffer(void)
{
	rxBufLen = 0;		
	return;
}

/**
 * @brief Reset do buffer de transmissão da UART
 */
void resetTxBuffer(void)
{
	txBufLen = 0;		
	return;
}

/**
 * @brief Retorna os dados que vão ser enviados pelo sensor.
 * 
 * @param buf Buffer onde os dados serão armazenados.
 * @param len Ponteiro para uma variável onde é armazenado o tamanho dos dados.
 */
void getTxBuffer(unsigned char * buf, int * len)
{
	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return;
}

/**
 * @brief Simula a leitura de um sensor e armazena o valor em 'buf'.
 * 
 * @param sensorType Tipo de sensor a ser emulado(t,h,c).
 * @param buf Buffer onde o valor do sensor será armazenado.
 * @param len Ponteiro para uma variável onde é armazenado o tamanho dos dados.
 */
void emulateSensor(unsigned char sensorType, unsigned char *buf, int *len) {
    sensorCounter++;  //Variavel usada para obter diferentes valores a chamada da função.
    int index = sensorCounter % 20; // Indice da amostra num buffer "circular" de 20 amostras para atender à especificação de ter um log de 20 amostras
	int temp, hum, co2;

    switch (sensorType) {
        case 't':
			temp = ((sensorCounter % 111) - 50); // Range do sensor de temperatura : [-50 ; 60]
			sensorData.temperature[index] = temp;
			if (temp >= 0) {
				sprintf((char*)buf, "+%02d", temp);
			} else {
				sprintf((char*)buf, "-%02d", -temp);
			}
			*len = strlen((char*)buf);
			break;
        case 'h':
            hum = (sensorCounter % 101); // Range do sensor de humidade : [0 ; 100]
            sensorData.humidity[index] = hum;
            sprintf((char*)buf, "%02d", hum);
            *len = strlen((char*)buf);
            break;
        case 'c':
            co2 = ((sensorCounter % 19601) + 400); // Range do sensor de C02 : [400 ; 20000]
            sensorData.co2[index] = co2;
            sprintf((char*)buf, "%04d", co2);
            *len = strlen((char*)buf);
            break;
        default:
            *len = 0;
            break;
    }
    sensorData.count = (sensorData.count + 1) % 20; // Atualiza o contador de amostras
}




