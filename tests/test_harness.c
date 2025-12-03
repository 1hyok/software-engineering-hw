/* Test Harness Implementation */

#include "test_harness.h"
#include <stdarg.h>
#include <string.h>

bool test_mode = false;
FILE* test_output_file = NULL;

bool stub_front_sensor = false;
bool stub_left_sensor = false;
bool stub_right_sensor = false;
bool stub_dust_sensor = false;

static MotorCommand last_motor_cmd = MOTOR_STOP;
static CleanerCommand last_cleaner_cmd = CLEANER_OFF;
static int motor_call_count = 0;
static int cleaner_call_count = 0;

void test_harness_init(void) {
    test_mode = true;
    test_output_file = stdout;
    stub_actuator_reset();
    printf("=== Test Harness Initialized ===\n");
}

void test_harness_cleanup(void) {
    if (test_output_file && test_output_file != stdout) {
        fclose(test_output_file);
    }
    printf("=== Test Harness Cleaned Up ===\n");
}

void test_log(const char* format, ...) {
    va_list args;
    va_start(args, format);
    if (test_output_file) {
        vfprintf(test_output_file, format, args);
    }
    va_end(args);
}

void test_log_result(const char* test_name, bool passed, const char* message) {
    test_log("[%s] %s: %s\n", passed ? "PASS" : "FAIL", test_name, message ? message : "");
}

void stub_sensor_set_front(bool value) {
    stub_front_sensor = value;
}

void stub_sensor_set_left(bool value) {
    stub_left_sensor = value;
}

void stub_sensor_set_right(bool value) {
    stub_right_sensor = value;
}

void stub_sensor_set_dust(bool value) {
    stub_dust_sensor = value;
}

void stub_actuator_reset(void) {
    last_motor_cmd = MOTOR_STOP;
    last_cleaner_cmd = CLEANER_OFF;
    motor_call_count = 0;
    cleaner_call_count = 0;
}

MotorCommand stub_actuator_get_last_motor_cmd(void) {
    return last_motor_cmd;
}

CleanerCommand stub_actuator_get_last_cleaner_cmd(void) {
    return last_cleaner_cmd;
}

int stub_actuator_get_motor_call_count(void) {
    return motor_call_count;
}

int stub_actuator_get_cleaner_call_count(void) {
    return cleaner_call_count;
}

const char* state_to_string(SystemState state) {
    switch (state) {
        case STATE_MOVING: return "MOVING";
        case STATE_TURNING: return "TURNING";
        case STATE_BACKWARDING: return "BACKWARDING";
        case STATE_DUST_CLEANING: return "DUST_CLEANING";
        case STATE_PAUSE: return "PAUSE";
        default: return "UNKNOWN";
    }
}

const char* motor_cmd_to_string(MotorCommand cmd) {
    switch (cmd) {
        case MOTOR_FORWARD: return "FORWARD";
        case MOTOR_TURN_LEFT: return "TURN_LEFT";
        case MOTOR_TURN_RIGHT: return "TURN_RIGHT";
        case MOTOR_BACKWARD: return "BACKWARD";
        case MOTOR_STOP: return "STOP";
        default: return "UNKNOWN";
    }
}

const char* cleaner_cmd_to_string(CleanerCommand cmd) {
    switch (cmd) {
        case CLEANER_OFF: return "OFF";
        case CLEANER_ON: return "ON";
        case CLEANER_POWERUP: return "POWERUP";
        default: return "UNKNOWN";
    }
}

void run_test_scenario(const char* name, void (*scenario)(void)) {
    printf("\n--- Running Scenario: %s ---\n", name);
    if (scenario) {
        scenario();
    }
    printf("--- Scenario Complete: %s ---\n\n", name);
}

