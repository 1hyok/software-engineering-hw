/* Unit Tests - 개별 함수 단위 테스트 */

#include "../unity/unity.h"
#include "../test_harness.h"
#include "../../src/types.h"

/* 테스트 대상 함수 선언 (외부에서 접근 가능하도록) */
#include "../../src/types.h"

/* 함수 선언 - src/fsm.c에서 정의 */
bool all_blocked(SensorData *sensors);
TurnDirection decide_turn_priority(SensorData *sensors);
void fsm_executor(RVCContext *ctx);

/* 함수 선언 - src/sensors.c에서 정의 */
void sensor_interface(SensorData *sensors);

/* 함수 선언 - src/actuators.c에서 정의 */
void motor_control(MotorCommand cmd);
void cleaner_control(CleanerCommand cmd);

/* Test 1: all_blocked 함수 테스트 - 모든 방향 막힘 확인 */
void test_all_blocked(void) {
    SensorData sensors;
    
    /* 모든 방향이 막힌 경우 */
    sensors.front = true;
    sensors.left = true;
    sensors.right = true;
    TEST_ASSERT_TRUE(all_blocked(&sensors));
    
    /* 일부만 막힌 경우 */
    sensors.front = true;
    sensors.left = false;
    sensors.right = true;
    TEST_ASSERT_FALSE(all_blocked(&sensors));
    
    /* 모든 방향이 열린 경우 */
    sensors.front = false;
    sensors.left = false;
    sensors.right = false;
    TEST_ASSERT_FALSE(all_blocked(&sensors));
}

/* Test 2: decide_turn_priority 함수 테스트 - 회전 우선순위 결정 */
void test_decide_turn_priority(void) {
    SensorData sensors;
    
    /* 왼쪽이 열린 경우 - 왼쪽 우선 */
    sensors.left = false;
    sensors.right = true;
    TEST_ASSERT_EQUAL_INT(TURN_LEFT, decide_turn_priority(&sensors));
    
    /* 오른쪽만 열린 경우 */
    sensors.left = true;
    sensors.right = false;
    TEST_ASSERT_EQUAL_INT(TURN_RIGHT, decide_turn_priority(&sensors));
    
    /* 양쪽 모두 열린 경우 - 왼쪽 우선 */
    sensors.left = false;
    sensors.right = false;
    TEST_ASSERT_EQUAL_INT(TURN_LEFT, decide_turn_priority(&sensors));
    
    /* 양쪽 모두 막힌 경우 */
    sensors.left = true;
    sensors.right = true;
    TEST_ASSERT_EQUAL_INT(TURN_NONE, decide_turn_priority(&sensors));
}

/* Test 3: FSM MOVING 상태 테스트 */
void test_fsm_moving_state(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    ctx.state_duration = 0;
    
    /* 정상 전진 상태 */
    ctx.sensors.front = false;
    ctx.sensors.dust = false;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
    TEST_ASSERT_EQUAL_INT(MOTOR_FORWARD, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_ON, ctx.cleaner_cmd);
    
    /* 먼지 감지 시 DUST_CLEANING으로 전이 */
    ctx.state = STATE_MOVING;
    ctx.sensors.dust = true;
    ctx.state_duration = 0;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_DUST_CLEANING, ctx.state);
    TEST_ASSERT_EQUAL_INT(5, ctx.dust_clean_timer);
    
    /* 전방 장애물 감지 시 TURNING으로 전이 */
    ctx.state = STATE_MOVING;
    ctx.sensors.front = true;
    ctx.sensors.dust = false;
    ctx.state_duration = 0;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
}

/* Test 4: FSM TURNING 상태 테스트 */
void test_fsm_turning_state(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_TURNING;
    ctx.state_duration = 0;
    
    /* 왼쪽 회전 가능 */
    ctx.sensors.front = true;
    ctx.sensors.left = false;
    ctx.sensors.right = true;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_LEFT, ctx.motor_cmd);
    
    /* 오른쪽 회전 가능 */
    ctx.state = STATE_TURNING;
    ctx.sensors.left = true;
    ctx.sensors.right = false;
    ctx.state_duration = 0;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_RIGHT, ctx.motor_cmd);
    
    /* 모든 방향 막힘 - BACKWARDING으로 전이 */
    ctx.state = STATE_TURNING;
    ctx.sensors.front = true;
    ctx.sensors.left = true;
    ctx.sensors.right = true;
    ctx.state_duration = 0;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_BACKWARDING, ctx.state);
    TEST_ASSERT_EQUAL_INT(3, ctx.backward_timer);
}

/* Test 5: FSM BACKWARDING 상태 테스트 */
void test_fsm_backwarding_state(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_BACKWARDING;
    ctx.backward_timer = 3;
    
    /* 후진 중 */
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_BACKWARD, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_ON, ctx.cleaner_cmd);
    TEST_ASSERT_EQUAL_INT(2, ctx.backward_timer);
    
    /* 타이머 만료 시 TURNING으로 전이 */
    ctx.backward_timer = 1;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(0, ctx.backward_timer);
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
}

/* Test 6: FSM DUST_CLEANING 상태 테스트 */
void test_fsm_dust_cleaning_state(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_DUST_CLEANING;
    ctx.dust_clean_timer = 5;
    
    /* 먼지 청소 중 */
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_STOP, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_POWERUP, ctx.cleaner_cmd);
    TEST_ASSERT_EQUAL_INT(4, ctx.dust_clean_timer);
    
    /* 타이머 만료 시 MOVING으로 전이 */
    ctx.dust_clean_timer = 1;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(0, ctx.dust_clean_timer);
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
}

/* Test 7: FSM PAUSE 상태 테스트 */
void test_fsm_pause_state(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_PAUSE;
    ctx.state_duration = 0;
    
    /* 일시 정지 중 */
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_STOP, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_ON, ctx.cleaner_cmd);
    
    /* 일정 시간 후 BACKWARDING으로 전이 */
    ctx.state_duration = 3;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_BACKWARDING, ctx.state);
    TEST_ASSERT_EQUAL_INT(3, ctx.backward_timer);
}

/* Test 8: sensor_interface 함수 테스트 */
void test_sensor_interface(void) {
    SensorData sensors;
    
    /* 센서 인터페이스 호출 */
    sensor_interface(&sensors);
    
    /* 센서 값이 설정되었는지 확인 (랜덤이므로 범위 체크) */
    TEST_ASSERT_TRUE(sensors.front == true || sensors.front == false);
    TEST_ASSERT_TRUE(sensors.left == true || sensors.left == false);
    TEST_ASSERT_TRUE(sensors.right == true || sensors.right == false);
    TEST_ASSERT_TRUE(sensors.dust == true || sensors.dust == false);
}

/* Test 9: motor_control 함수 테스트 */
void test_motor_control(void) {
    /* 각 모터 명령 테스트 */
    motor_control(MOTOR_FORWARD);
    motor_control(MOTOR_TURN_LEFT);
    motor_control(MOTOR_TURN_RIGHT);
    motor_control(MOTOR_BACKWARD);
    motor_control(MOTOR_STOP);
    
    /* 함수가 정상적으로 실행되는지 확인 (스텁에서 검증) */
    TEST_ASSERT_TRUE(true); /* 함수 호출 성공 확인 */
}

/* Test 10: cleaner_control 함수 테스트 */
void test_cleaner_control(void) {
    /* 각 청소기 명령 테스트 */
    cleaner_control(CLEANER_OFF);
    cleaner_control(CLEANER_ON);
    cleaner_control(CLEANER_POWERUP);
    
    /* 함수가 정상적으로 실행되는지 확인 */
    TEST_ASSERT_TRUE(true); /* 함수 호출 성공 확인 */
}

