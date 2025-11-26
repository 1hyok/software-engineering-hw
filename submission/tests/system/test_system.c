/* System Tests - 전체 시스템 시나리오 테스트 */

#include "../unity/unity.h"
#include "../test_harness.h"
#include "../../src/types.h"

#include "../../src/types.h"

/* 함수 선언 */
void initialize_system(void);
void sensor_interface(SensorData *sensors);
void fsm_executor(RVCContext *ctx);
void actuator_interface(RVCContext *ctx);

/* 전역 변수 선언 */
extern RVCContext rvc;

/* Test 1: 시스템 초기화 테스트 */
void test_system_initialization(void) {
    initialize_system();
    
    /* 초기 상태 확인 */
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, rvc.state);
    TEST_ASSERT_EQUAL_INT(0, rvc.tick_count);
    TEST_ASSERT_EQUAL_INT(0, rvc.state_duration);
    TEST_ASSERT_EQUAL_INT(0, rvc.dust_clean_timer);
    TEST_ASSERT_EQUAL_INT(0, rvc.backward_timer);
    TEST_ASSERT_EQUAL_INT(MOTOR_FORWARD, rvc.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_ON, rvc.cleaner_cmd);
}

/* Test 2: 정상 동작 시나리오 테스트 */
void test_system_normal_operation(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 정상 전진 시나리오 */
    for (int i = 0; i < 10; i++) {
        ctx.sensors.front = false;
        ctx.sensors.dust = false;
        ctx.tick_count = i;
        
        sensor_interface(&ctx.sensors);
        fsm_executor(&ctx);
        actuator_interface(&ctx);
        
        /* 정상 전진 상태 유지 확인 */
        if (ctx.state == STATE_MOVING) {
            TEST_ASSERT_EQUAL_INT(MOTOR_FORWARD, ctx.motor_cmd);
            TEST_ASSERT_EQUAL_INT(CLEANER_ON, ctx.cleaner_cmd);
        }
    }
    
    TEST_ASSERT_TRUE(true); /* 시나리오 완료 */
}

/* Test 3: 장애물 회피 시나리오 테스트 */
void test_system_obstacle_avoidance(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 전방 장애물 감지 */
    ctx.sensors.front = true;
    ctx.sensors.left = false;
    ctx.sensors.right = true;
    
    sensor_interface(&ctx.sensors);
    fsm_executor(&ctx);
    actuator_interface(&ctx);
    
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_LEFT, ctx.motor_cmd);
    
    /* 회전 완료 후 전진 복귀 */
    ctx.sensors.front = false;
    ctx.state_duration = 2;
    fsm_executor(&ctx);
    
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
}

/* Test 4: 먼지 청소 시나리오 테스트 */
void test_system_dust_cleaning(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 먼지 감지 */
    ctx.sensors.dust = true;
    ctx.sensors.front = false;
    
    sensor_interface(&ctx.sensors);
    fsm_executor(&ctx);
    actuator_interface(&ctx);
    
    TEST_ASSERT_EQUAL_INT(STATE_DUST_CLEANING, ctx.state);
    TEST_ASSERT_EQUAL_INT(MOTOR_STOP, ctx.motor_cmd);
    TEST_ASSERT_EQUAL_INT(CLEANER_POWERUP, ctx.cleaner_cmd);
    
    /* 청소 완료 후 전진 복귀 */
    ctx.dust_clean_timer = 1;
    fsm_executor(&ctx);
    fsm_executor(&ctx);
    
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
}

/* Test 5: 데드락 회복 시나리오 테스트 */
void test_system_deadlock_recovery(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_TURNING;
    
    /* 모든 방향 막힘 */
    ctx.sensors.front = true;
    ctx.sensors.left = true;
    ctx.sensors.right = true;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_BACKWARDING, ctx.state);
    
    /* 후진 후 회전 시도 */
    ctx.backward_timer = 1;
    fsm_executor(&ctx);
    fsm_executor(&ctx);
    
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
}

/* Test 6: 다중 장애물 시나리오 테스트 */
void test_system_multiple_obstacles(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 연속적인 장애물 회피 */
    for (int i = 0; i < 5; i++) {
        ctx.sensors.front = (i % 2 == 0);
        ctx.sensors.left = false;
        ctx.sensors.right = true;
        
        sensor_interface(&ctx.sensors);
        fsm_executor(&ctx);
        actuator_interface(&ctx);
        
        if (ctx.sensors.front) {
            TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
        }
    }
    
    TEST_ASSERT_TRUE(true); /* 시나리오 완료 */
}

/* Test 7: 완전한 제어 주기 시나리오 테스트 */
void test_system_complete_cycle(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 전체 제어 주기 시뮬레이션 */
    for (int tick = 0; tick < 20; tick++) {
        ctx.tick_count = tick;
        
        /* 센서 읽기 */
        sensor_interface(&ctx.sensors);
        
        /* FSM 실행 */
        fsm_executor(&ctx);
        
        /* 액추에이터 제어 */
        actuator_interface(&ctx);
        
        /* 상태 유효성 확인 */
        TEST_ASSERT_TRUE(ctx.state >= STATE_MOVING && ctx.state <= STATE_PAUSE);
    }
    
    TEST_ASSERT_TRUE(true); /* 시나리오 완료 */
}

/* Test 8: 스트레스 테스트 */
void test_system_stress_test(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    
    /* 긴 시간 동안 실행 */
    for (int i = 0; i < 100; i++) {
        ctx.tick_count = i;
        ctx.sensors.front = (rand() % 10) < 2;
        ctx.sensors.dust = (rand() % 10) < 1;
        
        sensor_interface(&ctx.sensors);
        fsm_executor(&ctx);
        actuator_interface(&ctx);
        
        /* 시스템이 정상 동작하는지 확인 */
        TEST_ASSERT_TRUE(ctx.state >= STATE_MOVING && ctx.state <= STATE_PAUSE);
    }
    
    TEST_ASSERT_TRUE(true); /* 스트레스 테스트 완료 */
}

/* Test 9: 엣지 케이스 테스트 */
void test_system_edge_cases(void) {
    RVCContext ctx;
    
    /* 엣지 케이스 1: 모든 센서가 동시에 활성화 */
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    ctx.sensors.front = true;
    ctx.sensors.left = true;
    ctx.sensors.right = true;
    ctx.sensors.dust = true;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_DUST_CLEANING, ctx.state); /* 먼지 우선 */
    
    /* 엣지 케이스 2: 모든 센서가 비활성화 */
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_MOVING;
    ctx.sensors.front = false;
    ctx.sensors.left = false;
    ctx.sensors.right = false;
    ctx.sensors.dust = false;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_MOVING, ctx.state);
    
    /* 엣지 케이스 3: 타이머가 0인 상태 */
    memset(&ctx, 0, sizeof(RVCContext));
    ctx.state = STATE_BACKWARDING;
    ctx.backward_timer = 0;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_TURNING, ctx.state);
}

/* Test 10: 요구사항 검증 테스트 */
void test_system_requirement_validation(void) {
    RVCContext ctx;
    memset(&ctx, 0, sizeof(RVCContext));
    
    /* 요구사항 1: 좌회전 우선 규칙 */
    ctx.state = STATE_TURNING;
    ctx.sensors.left = false;
    ctx.sensors.right = false;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(MOTOR_TURN_LEFT, ctx.motor_cmd);
    
    /* 요구사항 2: 먼지 감지 시 Boost 모드 */
    ctx.state = STATE_MOVING;
    ctx.sensors.dust = true;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_DUST_CLEANING, ctx.state);
    TEST_ASSERT_EQUAL_INT(CLEANER_POWERUP, ctx.cleaner_cmd);
    
    /* 요구사항 3: 모든 방향 막힘 시 후진 */
    ctx.state = STATE_TURNING;
    ctx.sensors.front = true;
    ctx.sensors.left = true;
    ctx.sensors.right = true;
    
    fsm_executor(&ctx);
    TEST_ASSERT_EQUAL_INT(STATE_BACKWARDING, ctx.state);
    
    TEST_ASSERT_TRUE(true); /* 요구사항 검증 완료 */
}

