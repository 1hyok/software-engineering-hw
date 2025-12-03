/* Actuator Stub - 하드웨어 액추에이터를 대체하는 스텁 */

#include <stdio.h>
#include "../test_harness.h"
#include "../../src/types.h"

/* 함수 선언 (src/actuators.c에 정의됨) */
void motor_control(MotorCommand cmd);
void cleaner_control(CleanerCommand cmd);

static MotorCommand last_motor_cmd = MOTOR_STOP;
static CleanerCommand last_cleaner_cmd = CLEANER_OFF;
static int motor_call_count = 0;
static int cleaner_call_count = 0;

/* 실제 모터 제어 함수를 스텁으로 대체 */
void motor_control_stub(MotorCommand cmd) {
    last_motor_cmd = cmd;
    motor_call_count++;
    
    if (test_mode) {
        const char *direction;
        switch (cmd) {
            case MOTOR_FORWARD:   direction = "MOVE_FORWARD"; break;
            case MOTOR_TURN_LEFT: direction = "TURN_LEFT_45"; break;
            case MOTOR_TURN_RIGHT: direction = "TURN_RIGHT_45"; break;
            case MOTOR_BACKWARD:  direction = "MOVE_BACKWARD"; break;
            case MOTOR_STOP:      direction = "STOP"; break;
            default:              direction = "UNKNOWN"; break;
        }
        test_log("  [MOTOR_STUB] %s\n", direction);
    } else {
        motor_control(cmd);
    }
}

/* 실제 청소기 제어 함수를 스텁으로 대체 */
void cleaner_control_stub(CleanerCommand cmd) {
    last_cleaner_cmd = cmd;
    cleaner_call_count++;
    
    if (test_mode) {
        const char *clean_cmd;
        switch (cmd) {
            case CLEANER_OFF:     clean_cmd = "VACUUM_OFF"; break;
            case CLEANER_ON:      clean_cmd = "VACUUM_NORMAL"; break;
            case CLEANER_POWERUP: clean_cmd = "VACUUM_TURBO"; break;
            default:              clean_cmd = "UNKNOWN"; break;
        }
        test_log("  [CLEANER_STUB] %s\n", clean_cmd);
    } else {
        cleaner_control(cmd);
    }
}

MotorCommand get_last_motor_cmd(void) {
    return last_motor_cmd;
}

CleanerCommand get_last_cleaner_cmd(void) {
    return last_cleaner_cmd;
}

int get_motor_call_count(void) {
    return motor_call_count;
}

int get_cleaner_call_count(void) {
    return cleaner_call_count;
}

void reset_actuator_stub(void) {
    last_motor_cmd = MOTOR_STOP;
    last_cleaner_cmd = CLEANER_OFF;
    motor_call_count = 0;
    cleaner_call_count = 0;
}

