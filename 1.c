/*
 * RVC Control Software - Version 1: Single FSM
 * Homework #7 - Structured Analysis and Design
 * 
 * Features:
 * - Single FSM with 5 states
 * - Handles obstacle avoidance (left priority)
 * - Dust detection and cleaning
 * - Backward movement for deadlock
 * - Pause state to prevent deadlock
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* ========== Type Definitions ========== */

// FSM States
typedef enum {
    STATE_MOVING,
    STATE_TURNING,
    STATE_BACKWARDING,
    STATE_DUST_CLEANING,
    STATE_PAUSE
} SystemState;

// Motor Commands
typedef enum {
    MOTOR_FORWARD,
    MOTOR_TURN_LEFT,
    MOTOR_TURN_RIGHT,
    MOTOR_BACKWARD,
    MOTOR_STOP
} MotorCommand;

// Cleaner Commands
typedef enum {
    CLEANER_OFF,
    CLEANER_ON,
    CLEANER_POWERUP
} CleanerCommand;

// Turn Direction
typedef enum {
    TURN_LEFT,
    TURN_RIGHT,
    TURN_NONE
} TurnDirection;

// Sensor Data Structure
typedef struct {
    bool front;
    bool left;
    bool right;
    bool dust;
} SensorData;

// System Context
typedef struct {
    SystemState state;
    SensorData sensors;
    MotorCommand motor_cmd;
    CleanerCommand cleaner_cmd;
    int tick_count;
    int state_duration;
    int dust_clean_timer;
    int backward_timer;
} RVCContext;

/* ========== Global Variables ========== */
RVCContext rvc;

/* ========== Sensor Interface Functions ========== */

void read_front_sensor(bool *value) {
    // Emulated: Random obstacle detection
    *value = (rand() % 10) < 2;  // 20% chance
}

void read_left_sensor(bool *value) {
    *value = (rand() % 10) < 2;
}

void read_right_sensor(bool *value) {
    *value = (rand() % 10) < 2;
}

void read_dust_sensor(bool *value) {
    *value = (rand() % 10) < 1;  // 10% chance
}

void sensor_interface(SensorData *sensors) {
    read_front_sensor(&sensors->front);
    read_left_sensor(&sensors->left);
    read_right_sensor(&sensors->right);
    read_dust_sensor(&sensors->dust);
}

/* ========== Control Logic Functions ========== */

bool all_blocked(SensorData *sensors) {
    return sensors->front && sensors->left && sensors->right;
}

TurnDirection decide_turn_priority(SensorData *sensors) {
    // Left priority policy
    if (!sensors->left) {
        return TURN_LEFT;
    } else if (!sensors->right) {
        return TURN_RIGHT;
    } else {
        return TURN_NONE;
    }
}

void fsm_executor(RVCContext *ctx) {
    ctx->state_duration++;
    
    switch (ctx->state) {
        case STATE_MOVING:
            ctx->motor_cmd = MOTOR_FORWARD;
            ctx->cleaner_cmd = CLEANER_ON;
            
            // Transition conditions
            if (ctx->sensors.dust) {
                ctx->state = STATE_DUST_CLEANING;
                ctx->dust_clean_timer = 5;  // 5 ticks
                ctx->state_duration = 0;
                printf("[FSM] MOVING -> DUST_CLEANING (dust detected)\n");
            } 
            else if (ctx->sensors.front) {
                ctx->state = STATE_TURNING;
                ctx->state_duration = 0;
                printf("[FSM] MOVING -> TURNING (front obstacle)\n");
            }
            break;
            
        case STATE_TURNING:
            ctx->cleaner_cmd = CLEANER_ON;
            
            if (all_blocked(&ctx->sensors)) {
                ctx->state = STATE_BACKWARDING;
                ctx->backward_timer = 3;  // 3 ticks
                ctx->state_duration = 0;
                printf("[FSM] TURNING -> BACKWARDING (all blocked)\n");
            } 
            else {
                TurnDirection turn = decide_turn_priority(&ctx->sensors);
                if (turn == TURN_LEFT) {
                    ctx->motor_cmd = MOTOR_TURN_LEFT;
                    printf("[FSM] Turning LEFT (priority)\n");
                } else if (turn == TURN_RIGHT) {
                    ctx->motor_cmd = MOTOR_TURN_RIGHT;
                    printf("[FSM] Turning RIGHT\n");
                } else {
                    ctx->state = STATE_PAUSE;
                    ctx->state_duration = 0;
                    printf("[FSM] TURNING -> PAUSE (no turn available)\n");
                }
                
                // Return to moving after turn
                if (ctx->state_duration >= 2 && turn != TURN_NONE) {
                    ctx->state = STATE_MOVING;
                    ctx->state_duration = 0;
                    printf("[FSM] TURNING -> MOVING (turn complete)\n");
                }
            }
            break;
            
        case STATE_BACKWARDING:
            ctx->motor_cmd = MOTOR_BACKWARD;
            ctx->cleaner_cmd = CLEANER_ON;
            ctx->backward_timer--;
            
            if (ctx->backward_timer <= 0) {
                ctx->state = STATE_TURNING;
                ctx->state_duration = 0;
                printf("[FSM] BACKWARDING -> TURNING (escape)\n");
            }
            break;
            
        case STATE_DUST_CLEANING:
            ctx->motor_cmd = MOTOR_STOP;
            ctx->cleaner_cmd = CLEANER_POWERUP;
            ctx->dust_clean_timer--;
            
            if (ctx->dust_clean_timer <= 0) {
                ctx->state = STATE_MOVING;
                ctx->state_duration = 0;
                printf("[FSM] DUST_CLEANING -> MOVING (clean complete)\n");
            }
            break;
            
        case STATE_PAUSE:
            ctx->motor_cmd = MOTOR_STOP;
            ctx->cleaner_cmd = CLEANER_ON;
            
            // Deadlock escape: try backward after pause
            if (ctx->state_duration >= 3) {
                ctx->state = STATE_BACKWARDING;
                ctx->backward_timer = 3;
                ctx->state_duration = 0;
                printf("[FSM] PAUSE -> BACKWARDING (deadlock escape)\n");
            }
            break;
    }
}

/* ========== Actuator Interface Functions ========== */

void motor_control(MotorCommand cmd) {
    const char *direction;
    switch (cmd) {
        case MOTOR_FORWARD:   direction = "MOVE_FORWARD"; break;
        case MOTOR_TURN_LEFT: direction = "TURN_LEFT_45"; break;
        case MOTOR_TURN_RIGHT: direction = "TURN_RIGHT_45"; break;
        case MOTOR_BACKWARD:  direction = "MOVE_BACKWARD"; break;
        case MOTOR_STOP:      direction = "STOP"; break;
        default:              direction = "UNKNOWN"; break;
    }
    printf("  [MOTOR] %s\n", direction);
}

void cleaner_control(CleanerCommand cmd) {
    const char *clean_cmd;
    switch (cmd) {
        case CLEANER_OFF:     clean_cmd = "VACUUM_OFF"; break;
        case CLEANER_ON:      clean_cmd = "VACUUM_NORMAL"; break;
        case CLEANER_POWERUP: clean_cmd = "VACUUM_TURBO"; break;
        default:              clean_cmd = "UNKNOWN"; break;
    }
    printf("  [CLEANER] %s\n", clean_cmd);
}

void actuator_interface(RVCContext *ctx) {
    motor_control(ctx->motor_cmd);
    cleaner_control(ctx->cleaner_cmd);
}

/* ========== Main Control Loop ========== */

void initialize_system() {
    srand(time(NULL));
    rvc.state = STATE_MOVING;
    rvc.tick_count = 0;
    rvc.state_duration = 0;
    rvc.dust_clean_timer = 0;
    rvc.backward_timer = 0;
    rvc.motor_cmd = MOTOR_FORWARD;
    rvc.cleaner_cmd = CLEANER_ON;
    printf("=== RVC Control System V1 (Single FSM) Started ===\n\n");
}

void print_status(RVCContext *ctx) {
    const char *state_names[] = {
        "MOVING", "TURNING", "BACKWARDING", "DUST_CLEANING", "PAUSE"
    };
    
    printf("\n--- Tick %d ---\n", ctx->tick_count);
    printf("State: %s (duration: %d)\n", 
           state_names[ctx->state], ctx->state_duration);
    printf("Sensors: F=%d L=%d R=%d D=%d\n",
           ctx->sensors.front, ctx->sensors.left, 
           ctx->sensors.right, ctx->sensors.dust);
}

int main(void) {
    initialize_system();
    
    // Simulation loop: 50 ticks
    for (int i = 0; i < 50; i++) {
        rvc.tick_count = i;
        
        // 1. Sensor Interface
        sensor_interface(&rvc.sensors);
        
        // 2. Control Logic (FSM)
        fsm_executor(&rvc);
        
        // 3. Actuator Interface
        actuator_interface(&rvc);
        
        // 4. Status Display
        print_status(&rvc);
        
        // Simulate tick delay
        #ifndef _WIN32
        usleep(200000);  // 200ms
        #endif
    }
    
    printf("\n=== Simulation Complete ===\n");
    return 0;
}
