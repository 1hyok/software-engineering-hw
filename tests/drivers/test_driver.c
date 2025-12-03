/* Test Driver Implementation */

#include "test_driver.h"
#include <stdlib.h>
#include <string.h>

RVCContext test_context;

void setUp(void) {
    memset(&test_context, 0, sizeof(RVCContext));
    stub_actuator_reset();
    test_harness_init();
}

void tearDown(void) {
    stub_actuator_reset();
}

void run_unit_tests(void) {
    UNITY_BEGIN();
    extern void test_all_blocked(void);
    extern void test_decide_turn_priority(void);
    extern void test_fsm_moving_state(void);
    extern void test_fsm_turning_state(void);
    extern void test_fsm_backwarding_state(void);
    extern void test_fsm_dust_cleaning_state(void);
    extern void test_fsm_pause_state(void);
    extern void test_sensor_interface(void);
    extern void test_motor_control(void);
    extern void test_cleaner_control(void);
    
    RUN_TEST(test_all_blocked);
    RUN_TEST(test_decide_turn_priority);
    RUN_TEST(test_fsm_moving_state);
    RUN_TEST(test_fsm_turning_state);
    RUN_TEST(test_fsm_backwarding_state);
    RUN_TEST(test_fsm_dust_cleaning_state);
    RUN_TEST(test_fsm_pause_state);
    RUN_TEST(test_sensor_interface);
    RUN_TEST(test_motor_control);
    RUN_TEST(test_cleaner_control);
    
    UNITY_END();
}

void run_integration_tests(void) {
    UNITY_BEGIN();
    extern void test_sensor_to_fsm_integration(void);
    extern void test_fsm_to_actuator_integration(void);
    extern void test_sensor_fsm_actuator_chain(void);
    extern void test_state_transition_sequence(void);
    extern void test_obstacle_detection_flow(void);
    extern void test_dust_detection_flow(void);
    extern void test_backward_escape_flow(void);
    extern void test_turn_priority_integration(void);
    extern void test_timer_integration(void);
    extern void test_deadlock_recovery_flow(void);
    
    RUN_TEST(test_sensor_to_fsm_integration);
    RUN_TEST(test_fsm_to_actuator_integration);
    RUN_TEST(test_sensor_fsm_actuator_chain);
    RUN_TEST(test_state_transition_sequence);
    RUN_TEST(test_obstacle_detection_flow);
    RUN_TEST(test_dust_detection_flow);
    RUN_TEST(test_backward_escape_flow);
    RUN_TEST(test_turn_priority_integration);
    RUN_TEST(test_timer_integration);
    RUN_TEST(test_deadlock_recovery_flow);
    
    UNITY_END();
}

void run_system_tests(void) {
    UNITY_BEGIN();
    extern void test_system_initialization(void);
    extern void test_system_normal_operation(void);
    extern void test_system_obstacle_avoidance(void);
    extern void test_system_dust_cleaning(void);
    extern void test_system_deadlock_recovery(void);
    extern void test_system_multiple_obstacles(void);
    extern void test_system_complete_cycle(void);
    extern void test_system_stress_test(void);
    extern void test_system_edge_cases(void);
    extern void test_system_requirement_validation(void);
    
    RUN_TEST(test_system_initialization);
    RUN_TEST(test_system_normal_operation);
    RUN_TEST(test_system_obstacle_avoidance);
    RUN_TEST(test_system_dust_cleaning);
    RUN_TEST(test_system_deadlock_recovery);
    RUN_TEST(test_system_multiple_obstacles);
    RUN_TEST(test_system_complete_cycle);
    RUN_TEST(test_system_stress_test);
    RUN_TEST(test_system_edge_cases);
    RUN_TEST(test_system_requirement_validation);
    
    UNITY_END();
}

void run_all_tests(void) {
    printf("\n");
    printf("========================================\n");
    printf("  RVC Control System Test Suite\n");
    printf("========================================\n");
    
    printf("\n[1/3] Running Unit Tests...\n");
    run_unit_tests();
    
    printf("\n[2/3] Running Integration Tests...\n");
    run_integration_tests();
    
    printf("\n[3/3] Running System Tests...\n");
    run_system_tests();
    
    printf("\n========================================\n");
    printf("  All Tests Complete\n");
    printf("========================================\n");
}

