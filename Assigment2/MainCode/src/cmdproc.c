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
    unsigned char lastSamples[UART_RX_SIZE]; 
    int lastSamplesLen; 
	unsigned char sensorValue[UART_RX_SIZE];
	int sensorValueLen;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return -1; 
	
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

		if(i < rxBufLen) {
			switch(UARTRxBuffer[i+1]) { 
				
				case 'A':	// Reads the real-time values of the variables provided by the sensor

					// Emulate and send all sensor values
						//unsigned char sensorValues[UART_RX_SIZE];
						//int sensorValuesLen;
						emulateSensor('t', sensorValues, &sensorValuesLen);
						txChar('#');
						txChar('A');
						for(int k=0; k<sensorValuesLen; k++) {
							txChar(sensorValues[k]);
						}
						txChar('!');
						break;

				case 'P':	// Reads the real-time value of one of the sensors

					if (i + 2 >= rxBufLen) {
        				return CMD_NOT_FOUND; // Comando incompleto
    				}
					
					sid = UARTRxBuffer[i+2]; // Tipo de sensor
					if (sid != 't' && sid != 'h' && sid != 'c') {
						return CMD_NOT_FOUND; // Tipo de sensor inválido
					}

					/* Check checksum */
					if(!(calcChecksum(&(UARTRxBuffer[i+1]),2))) {
						return CS_ERROR;
					}

					/* Check EOF */
					if(UARTRxBuffer[i+6] != EOF_SYM) {
						return WRONG_STR_FORMAT;
					}

					//unsigned char sensorValue[UART_RX_SIZE];
					//int sensorValueLen;
					emulateSensor(sid, sensorValue, &sensorValueLen);
					txChar('#');
					txChar('p');
					txChar(sid);
					for(int k=0; k<sensorValueLen; k++) {
						txChar(sensorValue[k]);
					}
					txChar('!');
					break;


									
				case 'L': // Returns the last 20 samples of each variable
				
					// Código para  obter os últimos 20 samples de cada sensor.

					// Copiar as últimas 20 amostras para lastSamples
					for (int i = 0; i < 20; i++) {
						int index = (sensorData.count - i - 1 + 20) % 20; // Garante que o índice esteja dentro do intervalo
						if (i > 0) {
							lastSamples[lastSamplesLen++] = ','; // Separador entre amostras
						}
						sprintf((char*)&lastSamples[lastSamplesLen], "+%02d", sensorData.temperature[index]);
						lastSamplesLen += 3; // 3 caracteres - 1 de sinal e 2 numéricos

						sprintf((char*)&lastSamples[lastSamplesLen], ",%02d", sensorData.humidity[index]);
						lastSamplesLen += 3; // 3 caracteres para a humidade

						sprintf((char*)&lastSamples[lastSamplesLen], ",%04d", sensorData.co2[index]);
						lastSamplesLen += 5; // 5 caracteres para o CO2
    }

					txChar('#');
					txChar('L');
					for(int k=0; k<lastSamplesLen; k++) {
						txChar(lastSamples[k]);
					}
					txChar('!');
                    break;
                case 'R': // Resets the history

                    sensorCounter = 0;
					// Redefine os arrays de temperatura, umidade e CO2 para seus valores iniciais
					memset(sensorData.temperature, 0, sizeof(sensorData.temperature));
					memset(sensorData.humidity, 0, sizeof(sensorData.humidity));
					memset(sensorData.co2, 0, sizeof(sensorData.co2));
					sensorData.count = 0; // Redefine o contador de amostras para 0
					break;
                default:
                    return CMD_NOT_FOUND; // Unrecognized command
            }	
			
		}
	}
	
	/* Cmd string not null and SOF not found */
	return -4;
}

/**
 * @brief Calculo e validação do valor de checksum.
 * 
 * @param buf Buffer que contém os caracteres.
 * @param nbytes Número de caracteres a serem considerados para o cálculo do checksum.
 * @return 1 se o checksum calculado corresponder ao checksum recebido,
 *         0 caso contrário.
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
    int sum = 0;
    for (int i = 0; i < nbytes; i++) {
        sum += buf[i];
    }
    sum %= 256; // Garante que o valor 'sum' esteja entre 0-255

	// Conversão do para código ASCII e validação/comparação com o valor de checksum recebido
    char ascii_Sum[4];
    sprintf(ascii_Sum, "%03d", sum);
    if (buf[nbytes] == ascii_Sum[0] && buf[nbytes + 1] == ascii_Sum[1] && buf[nbytes + 2] == ascii_Sum[2]) {
        return 1; // Checksum calculado == checksum recebido
    }
    return 0;    // Caso contrário. 
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
    sensorCounter++;
    int index = sensorCounter % 20; // Índice para armazenar a amostra atual
	int temp, hum, co2;

    switch (sensorType) {
        case 't':
            temp = ((sensorCounter % 111) - 50); // Range do sensor de temperatura : [-50 ; 60]
            sensorData.temperature[index] = temp;
            sprintf((char*)buf, "+%02d", temp);
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




