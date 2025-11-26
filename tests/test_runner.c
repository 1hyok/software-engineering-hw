/* Test Runner - 모든 테스트를 실행하는 메인 프로그램 */

#include <stdio.h>
#include <stdlib.h>
#include "drivers/test_driver.h"

int main(void) {
    printf("\n");
    printf("========================================\n");
    printf("  RVC Control System Test Suite\n");
    printf("  Using Unity Test Framework\n");
    printf("========================================\n");
    printf("\n");
    
    /* 모든 테스트 실행 */
    run_all_tests();
    
    printf("\n");
    printf("========================================\n");
    printf("  Test Execution Complete\n");
    printf("========================================\n");
    
    return 0;
}

