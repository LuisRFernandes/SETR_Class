#include "unity.h"
#include "cmdproc.h"
#include "cmdproc.c"

void setUp(void) {}
void tearDown(void) {}

void test_EmptyString(void){
    
    //Chamar o processador de comando
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento retornou o código esperado para comando não encontrado
    TEST_ASSERT_EQUAL_INT(EMPTY_STRING, resultado);
}

void test_wrongCMD(void){
    rxChar('#');
    rxChar('B');
    rxChar('1');
    rxChar('2');
    rxChar('3');
    rxChar('!');

    // Chamar o processador de comando
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento retornou o código esperado para comando não encontrado
    TEST_ASSERT_EQUAL_INT(CMD_NOT_FOUND, resultado);

}

void test_sensor(void){
    rxChar('#');
    rxChar('P');
    rxChar('x');
    rxChar('1');
    rxChar('2');
    rxChar('3');
    rxChar('!');

     // Chamar o processador de comando
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento retornou o código esperado para comando não encontrado
    TEST_ASSERT_EQUAL_INT(SENSOR_ERROR, resultado);


}

void test_calcChecksum(void){
    unsigned char buf1[] = {'#','A','1','2','3','4','!'};
    unsigned char buf2[] = {'#','A','1','2','3','5','!'};
    
    int x = calcChecksum(buf1, 7);
    int y = calcChecksum(buf2, 7);
    int result = 0;

    if(x!=y){
        result = CS_ERROR;
    }

    TEST_ASSERT_EQUAL_INT(CS_ERROR, result);


}

void test_string_format(void){
    rxChar('#');
    rxChar('P');
    rxChar('t');
    rxChar('1');
    rxChar('2');
    rxChar('3');
    rxChar('4');

     // Chamar o processador de comando
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento retornou o código esperado para comando não encontrado
    TEST_ASSERT_EQUAL_INT(WRONG_STR_FORMAT, resultado);
}

void test_emulateSensor(void){
    unsigned char buf[UART_RX_SIZE];
    int len;
    emulateSensor('t',buf,&len); // temperature sensor
    TEST_ASSERT_TRUE(len>0);
}

void test_processamento_comando_A(void) {
    // Simular o recebimento do comando "A"
    rxChar('#');
    rxChar('A');
    rxChar('!');
    
    // Imprimir o conteúdo do buffer UARTRxBuffer
    printf("Conteúdo do buffer UARTRxBuffer: ");
    for (int i = 0; i < rxBufLen; i++) {
        printf("%c", UARTRxBuffer[i]);
    }
    printf("\n");
    
    // Chamar o processador de comando
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento ocorreu sem erros
    TEST_ASSERT_EQUAL_INT(0, resultado);
    
    // Verificar se a resposta foi gerada corretamente
    unsigned char resposta[UART_TX_SIZE];
    int resposta_len;
    getTxBuffer(resposta, &resposta_len);
    printf("Resposta gerada: ");
    for (int i = 0; i < resposta_len; i++) {
        printf("%c", resposta[i]);
    }
    printf("\n");
    TEST_ASSERT_EQUAL_INT('#', resposta[0]); // Verificar início do frame
    TEST_ASSERT_EQUAL_INT('A', resposta[1]); // Verificar comando
    TEST_ASSERT_EQUAL_INT('!', resposta[5]); // Verificar fim do frame
}

void test_processamento_comando_P(void) {
    // Simula valores de sensores
    unsigned char simulated_data[UART_RX_SIZE];
    int len;
    char sensor = 'h';
    emulateSensor(sensor, simulated_data, &len); // Simula um valor para o sensor de temperatura
    rxChar('#'); 
    rxChar('P'); 
    rxChar('t'); 
    for (int i = 0; i < len; i++) {
        rxChar(simulated_data[i]); // Adiciona os dados simulados ao buffer de recebimento
    }
    rxChar('!'); // Adiciona o caractere de fim de mensagem ao buffer de recebimento
    
    // Chama o processador de comandos
    int resultado = cmdProcessor();
    
    // Verifica se o processamento ocorreu sem erros
    TEST_ASSERT_EQUAL_INT(0, resultado);
    
    // Verifica se a resposta gerada está correta
    unsigned char expected_response[] = {'p', sensor, simulated_data[0]}; // Define a resposta esperada
    unsigned char response[UART_TX_SIZE];
    int response_len;

    getTxBuffer(response, &response_len);
    TEST_ASSERT_EQUAL_INT_ARRAY(expected_response, response, response_len); // Verifica se a resposta gerada está correta
}



int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_EmptyString);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_wrongCMD);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_sensor);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_string_format);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_calcChecksum);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_emulateSensor);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_processamento_comando_A);
    resetRxBuffer(); resetTxBuffer();
    RUN_TEST(test_processamento_comando_P);
    resetRxBuffer(); resetTxBuffer();
    
    return UNITY_END();
}