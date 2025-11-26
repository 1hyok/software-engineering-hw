/* Sensor Stub - 하드웨어 센서를 대체하는 스텁 */

#include <stdbool.h>
#include "../test_harness.h"

extern bool stub_front_sensor;
extern bool stub_left_sensor;
extern bool stub_right_sensor;
extern bool stub_dust_sensor;

/* 실제 센서 읽기 함수를 스텁으로 대체하는 것은 
   소스 코드 수정이 필요하므로 테스트 하네스에서 직접 제어 */

/* 실제 센서 읽기 함수를 스텁으로 대체 */
void read_front_sensor_stub(bool *value) {
    if (test_mode) {
        *value = stub_front_sensor;
    } else {
        *value = (rand() % 10) < 2;
    }
}

void read_left_sensor_stub(bool *value) {
    if (test_mode) {
        *value = stub_left_sensor;
    } else {
        *value = (rand() % 10) < 2;
    }
}

void read_right_sensor_stub(bool *value) {
    if (test_mode) {
        *value = stub_right_sensor;
    } else {
        *value = (rand() % 10) < 2;
    }
}

void read_dust_sensor_stub(bool *value) {
    if (test_mode) {
        *value = stub_dust_sensor;
    } else {
        *value = (rand() % 10) < 1;
    }
}

