/*
 * RVC Control Software - Version 2: Dual FSM (CN1 + CN2)
 * Homework #7 - Structured Analysis and Design
 * 
 * Features:
 * - CN1 (Motor FSM): Idle, Moving, Turning, Backwarding, Paused
 * - CN2 (Cleaner FSM): Off, Normal_Cleaning, PowerUp_Cleaning
 * - Inter-FSM communication via Cleaner_Trigger and Motor_Status
 * - Better modularity and maintainability
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* ========== Type Definitions ========== */

// CN1: Motor FSM States
typedef enum {
    MOTOR_IDLE,
    MOTOR_MOVING,
    MOTOR_TURNING,
    MOTOR_BACKWARDING,
    MOTOR_PAUSED
} MotorState;

// CN2: Cleaner FSM States
typedef enum {
    CLEANER_OFF,
    CLEANER_NORMAL,
    CLEANER_POWERUP
} CleanerState;

// Motor Commands
typedef enum {
    CMD_FORWARD,
    CMD_TURN_LEFT,
    CMD_TURN_RIGHT,
    CMD_BACKWARD,
    CMD_STOP
} MotorCommand;

// Cleaner Commands
typedef enum {
    CMD_OFF,
    CMD_NORMAL,
    CMD_TURBO
} CleanerCommand;

// Turn Direction
typedef enum {
    TURN_LEFT,
    TURN_RIGHT,
    TURN_NONE
} TurnDirection;

// Sensor Data
typedef struct {
    bool front;
    bool left;
    bool right;
    bool dust;
} SensorData;

// CN1 Context
typedef struct {
    MotorState state;
    MotorCommand command;
    int state_duration;
    int backward_timer;
    bool cleaner_trigger_received;
} CN1_Context;

// CN2 Context
typedef struct {
    CleanerState state;
    CleanerCommand command;
    int powerup_timer;
    bool motor_is_moving;
} CN2_Context;

// System Context
typedef struct {
    CN1_Context cn1;
    CN2_Context cn2;
    SensorData sensors;
    int tick_count;
    bool cleaner_trigger;
    bool motor_status_moving;
} RVCSystem;

/* ========== Global Variables ========== */
RVCSystem rvc;

/* ========== Sensor Interface Functions ========== */

void read_front_sensor(bool *value) {
    *value = (rand() % 10) < 2;  // 20% obstacle
}

void read_left_sensor(bool *value) {
    *value = (rand() % 10) < 2;
}

void read_right_sensor(bool *value) {
    *value = (rand() % 10) < 2;
}

void read_dust_sensor(bool *value) {
    *value = (rand() % 10) < 1;  // 10% dust
}

void sensor_interface(SensorData *sensors) {
    read_front_sensor(&sensors->front);
    read_left_sensor(&sensors->left);
    read_right_sensor(&sensors->right);
    read_dust_sensor(&sensors->dust);
}

/* ========== CN1: Motor Control FSM ========== */

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

void cn1_motor_fsm(CN1_Context *cn1, SensorData *sensors, bool cleaner_trigger) {
    cn1->state_duration++;
    cn1->cleaner_trigger_received = cleaner_trigger;
    
    switch (cn1->state) {
        case MOTOR_IDLE:
            cn1->command = CMD_STOP;
            // Auto-start (simulated)
            if (cn1->state_duration >= 2) {
                cn1->state = MOTOR_MOVING;
                cn1->state_duration = 0;
                printf("[CN1] IDLE -> MOVING (start)\n");
            }
            break;
            
        case MOTOR_MOVING:
            cn1->command = CMD_FORWARD;
            
            // Priority 1: Cleaner trigger (pause for dust cleaning)
            if (cleaner_trigger) {
                cn1->state = MOTOR_PAUSED;
                cn1->state_duration = 0;
                printf("[CN1] MOVING -> PAUSED (cleaner trigger)\n");
            }
            // Priority 2: Obstacle avoidance
            else if (sensors->front) {
                cn1->state = MOTOR_TURNING;
                cn1->state_duration = 0;
                printf("[CN1] MOVING -> TURNING (front obstacle)\n");
            }
            break;
            
        case MOTOR_TURNING:
            if (all_blocked(sensors)) {
                cn1->state = MOTOR_BACKWARDING;
                cn1->backward_timer = 3;  // 3 ticks
                cn1->state_duration = 0;
                printf("[CN1] TURNING -> BACKWARDING (all blocked)\n");
            } 
            else {
                TurnDirection turn = decide_turn_priority(sensors);
                if (turn == TURN_LEFT) {
                    cn1->command = CMD_TURN_LEFT;
                    printf("[CN1] Executing TURN_LEFT\n");
                } else if (turn == TURN_RIGHT) {
                    cn1->command = CMD_TURN_RIGHT;
                    printf("[CN1] Executing TURN_RIGHT\n");
                } else {
                    cn1->state = MOTOR_PAUSED;
                    cn1->state_duration = 0;
                    printf("[CN1] TURNING -> PAUSED (no path)\n");
                }
                
                // Complete turn
                if (cn1->state_duration >= 2 && turn != TURN_NONE) {
                    cn1->state = MOTOR_MOVING;
                    cn1->state_duration = 0;
                    printf("[CN1] TURNING -> MOVING (turn complete)\n");
                }
            }
            break;
            
        case MOTOR_BACKWARDING:
            cn1->command = CMD_BACKWARD;
            cn1->backward_timer--;
            
            if (cn1->backward_timer <= 0) {
                cn1->state = MOTOR_TURNING;
                cn1->state_duration = 0;
                printf("[CN1] BACKWARDING -> TURNING (escape)\n");
            }
            break;
            
        case MOTOR_PAUSED:
            cn1->command = CMD_STOP;
            
            // Resume when cleaner done OR deadlock escape timeout
            if (!cleaner_trigger && cn1->state_duration >= 1) {
                cn1->state = MOTOR_MOVING;
                cn1->state_duration = 0;
                printf("[CN1] PAUSED -> MOVING (resume)\n");
            }
            // Deadlock escape after long pause
            else if (cn1->state_duration >= 5) {
                cn1->state = MOTOR_BACKWARDING;
                cn1->backward_timer = 3;
                cn1->state_duration = 0;
                printf("[CN1] PAUSED -> BACKWARDING (deadlock escape)\n");
            }
            break;
    }
}

/* ========== CN2: Cleaner Control FSM ========== */

void cn2_cleaner_fsm(CN2_Context *cn2, bool dust_detected, bool motor_moving) {
    cn2->motor_is_moving = motor_moving;
    
    switch (cn2->state) {
        case CLEANER_OFF:
            cn2->command = CMD_OFF;
            // Auto-start with system
            cn2->state = CLEANER_NORMAL;
            printf("[CN2] OFF -> NORMAL (start)\n");
            break;
            
        case CLEANER_NORMAL:
            cn2->command = CMD_NORMAL;
            
            // Detect dust while moving -> request pause and power up
            if (dust_detected && motor_moving) {
                cn2->state = CLEANER_POWERUP;
                cn2->powerup_timer = 5;  // 5 ticks intensive clean
                printf("[CN2] NORMAL -> POWERUP (dust detected)\n");
            }
            break;
            
        case CLEANER_POWERUP:
            cn2->command = CMD_TURBO;
            cn2->powerup_timer--;
            
            if (cn2->powerup_timer <= 0) {
                cn2->state = CLEANER_NORMAL;
                printf("[CN2] POWERUP -> NORMAL (clean complete)\n");
            }
            break;
    }
}

/* ========== Control Logic Coordination ========== */

void control_logic(RVCSystem *sys) {
    // Determine cleaner trigger signal
    sys->cleaner_trigger = (sys->cn2.state == CLEANER_POWERUP);
    
    // Determine motor status
    sys->motor_status_moving = (sys->cn1.state == MOTOR_MOVING);
    
    // Execute CN1 (Motor FSM)
    cn1_motor_fsm(&sys->cn1, &sys->sensors, sys->cleaner_trigger);
    
    // Execute CN2 (Cleaner FSM)
    cn2_cleaner_fsm(&sys->cn2, sys->sensors.dust, sys->motor_status_moving);
}

/* ========== Actuator Interface Functions ========== */

void motor_control(MotorCommand cmd) {
    const char *direction;
    switch (cmd) {
        case CMD_FORWARD:   direction = "MOVE_FORWARD"; break;
        case CMD_TURN_LEFT: direction = "TURN_LEFT_45"; break;
        case CMD_TURN_RIGHT: direction = "TURN_RIGHT_45"; break;
        case CMD_BACKWARD:  direction = "MOVE_BACKWARD"; break;
        case CMD_STOP:      direction = "STOP"; break;
        default:            direction = "UNKNOWN"; break;
    }
    printf("  [MOTOR] %s\n", direction);
}

void cleaner_control(CleanerCommand cmd) {
    const char *clean_cmd;
    switch (cmd) {
        case CMD_OFF:    clean_cmd = "VACUUM_OFF"; break;
        case CMD_NORMAL: clean_cmd = "VACUUM_NORMAL"; break;
        case CMD_TURBO:  clean_cmd = "VACUUM_TURBO"; break;
        default:         clean_cmd = "UNKNOWN"; break;
    }
    printf("  [CLEANER] %s\n", clean_cmd);
}

void actuator_interface(RVCSystem *sys) {
    motor_control(sys->cn1.command);
    cleaner_control(sys->cn2.command);
}

/* ========== Main Control Loop ========== */

void initialize_system() {
    srand(time(NULL));
    
    // Initialize CN1
    rvc.cn1.state = MOTOR_IDLE;
    rvc.cn1.command = CMD_STOP;
    rvc.cn1.state_duration = 0;
    rvc.cn1.backward_timer = 0;
    rvc.cn1.cleaner_trigger_received = false;
    
    // Initialize CN2
    rvc.cn2.state = CLEANER_OFF;
    rvc.cn2.command = CMD_OFF;
    rvc.cn2.powerup_timer = 0;
    rvc.cn2.motor_is_moving = false;
    
    // System
    rvc.tick_count = 0;
    rvc.cleaner_trigger = false;
    rvc.motor_status_moving = false;
    
    printf("=== RVC Control System V2 (Dual FSM: CN1+CN2) Started ===\n\n");
}

void print_status(RVCSystem *sys) {
    const char *motor_states[] = {
        "IDLE", "MOVING", "TURNING", "BACKWARDING", "PAUSED"
    };
    const char *cleaner_states[] = {
        "OFF", "NORMAL", "POWERUP"
    };
    
    printf("\n--- Tick %d ---\n", sys->tick_count);
    printf("CN1 State: %s (duration: %d)\n", 
           motor_states[sys->cn1.state], sys->cn1.state_duration);
    printf("CN2 State: %s\n", cleaner_states[sys->cn2.state]);
    printf("Sensors: F=%d L=%d R=%d D=%d\n",
           sys->sensors.front, sys->sensors.left, 
           sys->sensors.right, sys->sensors.dust);
    printf("Trigger: Cleaner->Motor=%d, Motor->Cleaner=%d\n",
           sys->cleaner_trigger, sys->motor_status_moving);
}

int main(void) {
    initialize_system();
    
    // Simulation loop: 50 ticks
    for (int i = 0; i < 50; i++) {
        rvc.tick_count = i;
        
        // 1. Sensor Interface
        sensor_interface(&rvc.sensors);
        
        // 2. Control Logic (CN1 + CN2)
        control_logic(&rvc);
        
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
    printf("\nVersion 2 Benefits:\n");
    printf("- Better separation of concerns (Motor vs Cleaner)\n");
    printf("- Easier to maintain and extend\n");
    printf("- Clear inter-module communication\n");
    printf("- No interface inconsistency\n");
    
    return 0;
}
