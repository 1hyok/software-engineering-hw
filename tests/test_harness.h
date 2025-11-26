/* Test Harness - 테스트 환경 설정 및 유틸리티 */

#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/types.h"

/* 테스트 하네스 전역 변수 */
extern bool test_mode;
extern FILE* test_output_file;

/* 테스트 초기화 */
void test_harness_init(void);
void test_harness_cleanup(void);

/* 테스트 결과 기록 */
void test_log(const char* format, ...);
void test_log_result(const char* test_name, bool passed, const char* message);

/* 센서 스텁 제어 */
void stub_sensor_set_front(bool value);
void stub_sensor_set_left(bool value);
void stub_sensor_set_right(bool value);
void stub_sensor_set_dust(bool value);

/* 액추에이터 스텁 제어 */
void stub_actuator_reset(void);
MotorCommand stub_actuator_get_last_motor_cmd(void);
CleanerCommand stub_actuator_get_last_cleaner_cmd(void);
int stub_actuator_get_motor_call_count(void);
int stub_actuator_get_cleaner_call_count(void);

/* FSM 상태 검증 헬퍼 */
const char* state_to_string(SystemState state);
const char* motor_cmd_to_string(MotorCommand cmd);
const char* cleaner_cmd_to_string(CleanerCommand cmd);

/* 테스트 시나리오 실행 */
void run_test_scenario(const char* name, void (*scenario)(void));

#endif

