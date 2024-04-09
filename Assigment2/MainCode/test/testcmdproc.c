#include "unity.h"
#include "cmdproc.h"
#include "cmdproc.c"

void setUp(void) {}
void tearDown(void) {}

void test_EmptyString(void){
    
    //Chamar o cmd_processor
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento retornou o código de erro esperado para comando não encontrado
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
    unsigned char buf1[] = {'#','A','1','2','3','!'};
    unsigned char buf2[] = {'#','A','1','2','4','!'};
    
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

    printf("\n");
}

void test_emulateSensor(void){
    unsigned char buf[UART_RX_SIZE];
    int len;
    emulateSensor('t',buf,&len); // temperature sensor
    TEST_ASSERT_TRUE(len>0);

}

void test_processamento_comando_A(void) {
    //  É testado quando é recebido o comando A, com o objetivo de verificar se o EOF, cmd e EOF são enviados corretamente
    rxChar('#');
    rxChar('A');
    rxChar('!');
    
    // Imprimir o conteúdo do buffer UARTRxBuffer
    printf("Conteúdo do buffer UARTRxBuffer: ");
    for (int i = 0; i < rxBufLen; i++) {
        printf("%c", UARTRxBuffer[i]);
    }
    
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
    // Os bytes [2],[3] e [4] são para os dados emulados pelo sensor de temperatura.
    TEST_ASSERT_EQUAL_INT('!', resposta[5]); // Verificar fim do frame

}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_EmptyString);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");

    RUN_TEST(test_wrongCMD);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");

    RUN_TEST(test_sensor);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");

    RUN_TEST(test_string_format);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");

    RUN_TEST(test_calcChecksum);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");

    RUN_TEST(test_emulateSensor);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");

    RUN_TEST(test_processamento_comando_A);
    resetRxBuffer(); resetTxBuffer();
    printf("\n");
    
    return UNITY_END();
}