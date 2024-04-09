/**
 * @file testcmdproc.c
 * @brief Testes usando Unit Testing para o cmd_processor
 *
 * Este ficheiro contém testes feitos ao cmd_processor para detetar possiveis erros de transmissão de dados,
 *  como por exemplo o envio de dados  vazios ou com comprimento errado.
 * Os testes são feitos com recurso à biblioteca Unity.
 */


#include "unity.h"
#include "cmdproc.h"
#include "cmdproc.c"

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Teste para verificar a omissão de dados recebidos. (String vazia)
 * 
 *  Caso não seja recebido nada, o sistema devolve uma mensagem de erro relativa à string vazia.
 */
void test_EmptyString(void){
    
    //Chamar o cmd_processor
    printf("cmdProcessor()...\n");
    int resultado = cmdProcessor();
    printf("Resultado do cmdProcessor(): %d\n", resultado);
    
    // Verificar se o processamento retornou o código de erro esperado para comando não encontrado
    TEST_ASSERT_EQUAL_INT(EMPTY_STRING, resultado);
}

/**
 * @brief Teste para verificar o processamento de um comando inválido.
 *
 * Caso o comando recebido não seja válido, o sistema devolve uma mensagem de erro que sinaliza esse acontecimento.
 * 
 * Neste teste em particular foi testado o envio do comando 'B' que é considerado inválido pelo sistema.
 */
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

/**
 * @brief Teste para verificar o processamento de um comando de sensor válido.
 *
 *Caso o comando recebido não seja válido, o sistema devolve uma mensagem de erro que sinaliza esse acontecimento.
 * 
 *Neste teste em particular foi testado o envio do comando 'x' que não corresponde a nenhum sensor válido.
 */
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

/**
 * @brief Teste para verificar o cálculo do checksum.
 *
 * Este teste verifica se o cálculo do checksum está funcionando corretamente.
 * São simuladas 2 tramas diferentes e se o valor produzido por elas for diferente, retorna uma mensagem de erro.
 */
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

/**
 * @brief Teste que faz a verificação do processamento de um comando com formato de string errado.
 *
 *Caso o formato da string não seja válida, o sistema devolve uma mensagem de erro que sinaliza esse acontecimento.
 * 
 *Neste teste em particular omitiu-se o envio do caracter que representa o EOF (!).
 * 
 */
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

/**
 * @brief Teste para verificar se a emulação do sensor produz algum valor.
 *
 * Neste teste apenas se verifica se o tamanho do valor produzido pelo sensor é maior que 0, o que implica que foi emulado um valor.
 */
void test_emulateSensor(void){
    unsigned char buf[UART_RX_SIZE];
    int len;
    emulateSensor('t',buf,&len); // temperature sensor
    TEST_ASSERT_TRUE(len>0);

}
/**
 * @brief Teste para verificar se o EOF, cmd e EOF são enviados corretamente.
 *
 */
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