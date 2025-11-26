/* Test Driver - 테스트를 실행하는 드라이버 */

#ifndef TEST_DRIVER_H
#define TEST_DRIVER_H

#include "../unity/unity.h"
#include "../test_harness.h"
#include "../../src/types.h"

/* 테스트 케이스 실행 함수 */
void setUp(void);
void tearDown(void);

/* Unit Test 드라이버 */
void run_unit_tests(void);

/* Integration Test 드라이버 */
void run_integration_tests(void);

/* System Test 드라이버 */
void run_system_tests(void);

/* 모든 테스트 실행 */
void run_all_tests(void);

#endif

