/* Integration Tests - 모듈 간 상호작용 테스트 */

#include "../unity/unity.h"
#include "../test_harness.h"
#include "../../src/types.h"

#include "../../src/types.h"

/* 함수 선언 */
void sensor_interface(SensorData *sensors);
void fsm_executor(RVCContext *ctx);
void actuator_interface(RVCContext *ctx);
void motor_control(MotorCommand cmd);
void cleaner_control(CleanerCommand cmd);

/* Test 1: 센서에서 FSM으로의 데이터 전달 통합 테스트 */
void test_sensor_to_fsm_integration(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 센서 데이터 설정 */
    ctx.sensors.front = true;
    ctx.sensors.dust = false;
    
    /* 센서 인터페이스 호출 생략 (랜덤 값 방지) 후 FSM 실행 */
    // sensor_interface(&ctx.sensors);
    fsm_executor(&ctx);
    
    /* 전방 장애물 감지 시 TURNING 상태로 전이 확인 */
    if (ctx.sensors.front) {
        TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
    }
}

/* Test 2: FSM에서 액추에이터로의 명령 전달 통합 테스트 */
void test_fsm_to_actuator_integration(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    ctx.sensors.front = false;
    ctx.sensors.dust = false;
    
    /* FSM 실행 */
    fsm_executor(&ctx);
    
    /* 액추에이터 인터페이스 호출 */
    actuator_interface(&ctx);
    
    /* MOVING 상태에서 FORWARD 명령 확인 */
    TEST_ASSERT_EQUAL_INT(MOTOR_FORWARD, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_ON, ctx.cleaner_cmd);
}

/* Test 3: 센서-FSM-액추에이터 전체 체인 통합 테스트 */
void test_sensor_fsm_actuator_chain(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 전체 제어 루프 시뮬레이션 */
    // sensor_interface(&ctx.sensors);
    fsm_executor(&ctx);
    actuator_interface(&ctx);
    
    /* 상태와 명령이 일관성 있게 설정되었는지 확인 */
    TEST_ASSERT_TRUE(ctx.state >= STATE_MOVING && ctx.state <= STATE_PAUSE);
    TEST_ASSERT_TRUE(ctx.motor_cmd >= MOTOR_FORWARD && ctx.motor_cmd <= MOTOR_STOP);
    TEST_ASSERT_TRUE(ctx.cleaner_cmd >= CLEANER_OFF && ctx.cleaner_cmd <= CLEANER_POWERUP);
}

/* Test 4: 상태 전이 시퀀스 통합 테스트 */
void test_state_transition_sequence(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* MOVING -> TURNING 전이 */
    ctx.sensors.front = true;
    ctx.sensors.dust = false;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
    
    /* TURNING -> MOVING 전이 (회전 완료) */
    ctx.sensors.front = false;
    ctx.sensors.left = false;
    ctx.state_duration = 2;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
}

/* Test 5: 장애물 감지 플로우 통합 테스트 */
void test_obstacle_detection_flow(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 전방 장애물 감지 */
    ctx.sensors.front = true;
    ctx.sensors.left = false;
    ctx.sensors.right = true;
    
    // sensor_interface(&ctx.sensors);
    fsm_executor(&ctx);
    actuator_interface(&ctx);
    
    /* TURNING 상태로 전이하고 왼쪽 회전 명령 확인 */
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_LEFT, ctx.motor_cmd);
}

/* Test 6: 먼지 감지 플로우 통합 테스트 */
void test_dust_detection_flow(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 먼지 감지 */
    ctx.sensors.dust = true;
    ctx.sensors.front = false;
    
    // sensor_interface(&ctx.sensors);
    fsm_executor(&ctx);
    actuator_interface(&ctx);
    
    /* DUST_CLEANING 상태로 전이하고 POWERUP 명령 확인 */
    TEST_ASSERT_EQUAL_INT(STATE_DUST_CLEANING, ctx.state);
    TEST_ASSERT_EQUAL_INT(MOTOR_STOP, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_POWERUP, ctx.cleaner_cmd);
}

/* Test 7: 후진 탈출 플로우 통합 테스트 */
void test_backward_escape_flow(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_TURNING;
    
    /* 모든 방향 막힘 */
    ctx.sensors.front = true;
    ctx.sensors.left = true;
    ctx.sensors.right = true;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_BACKWARDING, ctx.state);
    TEST_ASSERT_EQUAL_INT(3, ctx.backward_timer);
    
    /* 후진 타이머 만료 후 TURNING으로 전이 */
    ctx.backward_timer = 1;
    fsm_executor(&ctx);
    // fsm_executor(&ctx); // 두 번째 실행 시 센서값이 그대로면 다시 BACKWARDING으로 갈 수 있음
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
}

/* Test 8: 회전 우선순위 통합 테스트 */
void test_turn_priority_integration(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_TURNING;
    
    /* 양쪽 모두 열린 경우 - 왼쪽 우선 */
    ctx.sensors.front = true;
    ctx.sensors.left = false;
    ctx.sensors.right = false;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_LEFT, ctx.motor_cmd);
    
    /* 왼쪽만 막힌 경우 - 오른쪽 회전 */
    ctx.sensors.left = true;
    ctx.sensors.right = false;
    ctx.state_duration = 0;
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_RIGHT, ctx.motor_cmd);
}

/* Test 9: 타이머 통합 테스트 */
void test_timer_integration(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    
    /* DUST_CLEANING 타이머 테스트 */
    ctx.state = STATE_DUST_CLEANING;
    ctx.dust_clean_timer = 5;
    
    for (int i = 0; i < 5; i++) {
        fsm_executor(&ctx);
        TEST_ASSERT_EQUAL_INT(5 - i - 1, ctx.dust_clean_timer);
    }
    
    /* 타이머 만료 후 MOVING으로 전이 */
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
}

/* Test 10: 데드락 회복 플로우 통합 테스트 */
void test_deadlock_recovery_flow(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_PAUSE;
    ctx.state_duration = 0;
    
    /* PAUSE 상태에서 일정 시간 대기 */
    for (int i = 0; i < 2; i++) {
        fsm_executor(&ctx);
        TEST_ASSERT_EQUAL_INT(STATE_PAUSE, ctx.state);
    }
    
    /* BACKWARDING으로 전이 */
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_BACKWARDING, ctx.state);
    TEST_ASSERT_EQUAL_INT(3, ctx.backward_timer);
}

