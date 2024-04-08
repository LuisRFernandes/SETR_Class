#include <unity.h>
#include "cmdproc.h"

void setUp(void) {}
void tearDown(void) {}

void test_cmdproc_command_a_works(void) {
    // Test for command A
}

void test_cmdproc_command_p_works(void) {
    // Test for command P
}

void test_cmdproc_command_l_works(void) {
    // Test for command L
}

void test_cmdproc_command_r_works(void) {
    // Test for command R
}

void test_cmdproc_wrong_str_format(void) {
    // Test for wrong string format
}

void test_cmdproc_wrong_checksum(void) {
    // Test for wrong checksum
}

void test_cmdproc_missing_eof(void) {
    // Test for missing EOF
}

void test_cmdproc_empty_string(void) {
    // Test for empty string
}

void test_cmdproc_full_string(void) {
    // Test for full string
}

void test_cmdproc_more_than_one_sof(void) {
    // Test for more than one SOF
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_cmdproc_command_a_works);
    RUN_TEST(test_cmdproc_command_p_works);
    RUN_TEST(test_cmdproc_command_l_works);
    RUN_TEST(test_cmdproc_command_r_works);
    RUN_TEST(test_cmdproc_wrong_str_format);
    RUN_TEST(test_cmdproc_wrong_checksum);
    RUN_TEST(test_cmdproc_missing_eof);
    RUN_TEST(test_cmdproc_empty_string);
    RUN_TEST(test_cmdproc_full_string);
    RUN_TEST(test_cmdproc_more_than_one_sof);

    return UNITY_END();
}