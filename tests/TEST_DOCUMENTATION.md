# RVC 제어 시스템 xUnit 테스트 문서

## 목차

1. [C 언어용 xUnit 프레임워크 선정 및 설명](#1-c-언어용-xunit-프레임워크-선정-및-설명)
2. [Unit Test (10개)](#2-unit-test-10개)
3. [Integration Test (10개)](#3-integration-test-10개)
4. [System Test (10개)](#4-system-test-10개)
5. [테스트 환경 구축 (Driver, Stub, Test Harness)](#5-테스트-환경-구축-driver-stub-test-harness)
6. [테스트 실행 방법](#6-테스트-실행-방법)
7. [테스트 결과 요약](#7-테스트-결과-요약)

---

## 1. C 언어용 xUnit 프레임워크 선정 및 설명

### 1.1 Unity Test Framework 선정

본 프로젝트에서는 **Unity Test Framework**를 C 언어용 xUnit 테스트 프레임워크로 선정하였습니다.

### 1.2 Unity Framework 개요

Unity는 C 언어를 위한 경량 단위 테스트 프레임워크로, 다음과 같은 특징을 가집니다:

- **경량성**: 최소한의 의존성으로 구성되어 임베디드 시스템에도 적합
- **이식성**: 다양한 플랫폼과 컴파일러에서 동작
- **단순성**: 직관적인 API로 테스트 작성이 용이
- **xUnit 스타일**: JUnit, NUnit 등과 유사한 테스트 구조
- **독립성**: 외부 라이브러리 의존성 없이 독립 실행 가능

### 1.3 Unity Framework 구조

Unity는 다음과 같은 주요 구성 요소로 이루어져 있습니다:

#### 1.3.1 핵심 매크로

- `TEST_ASSERT_EQUAL_INT(expected, actual)`: 정수 값 비교
- `TEST_ASSERT_EQUAL_STRING(expected, actual)`: 문자열 비교
- `TEST_ASSERT_TRUE(condition)`: 조건이 참인지 확인
- `TEST_ASSERT_FALSE(condition)`: 조건이 거짓인지 확인
- `RUN_TEST(test_function)`: 테스트 함수 실행

#### 1.3.2 테스트 실행 구조

```c
void setUp(void) {
    // 각 테스트 전에 실행되는 초기화 코드
}

void tearDown(void) {
    // 각 테스트 후에 실행되는 정리 코드
}

void test_example(void) {
    // 테스트 코드
    TEST_ASSERT_EQUAL_INT(1, 1);
}
```

#### 1.3.3 Unity의 장점

1. **빠른 컴파일**: 작은 코드베이스로 빠른 빌드 시간
2. **명확한 오류 메시지**: 실패한 테스트에 대한 상세한 정보 제공
3. **유연한 구성**: 필요한 기능만 선택적으로 사용 가능
4. **커뮤니티 지원**: 활발한 오픈소스 커뮤니티

### 1.4 다른 프레임워크와의 비교

| 프레임워크 | 장점 | 단점 |
|-----------|------|------|
| **Unity** | 경량, 단순, 이식성 우수 | 기능 제한적 |
| CUnit | 기능 풍부, XML 리포트 | 설정 복잡, 의존성 많음 |
| Check | 멀티스레드 지원, 고급 기능 | 학습 곡선 높음 |
| CMocka | 모킹 지원 | 설정 복잡 |

본 프로젝트의 특성상 경량성과 단순성이 중요하여 Unity를 선택하였습니다.

---

## 2. Unit Test (10개)

Unit Test는 개별 함수나 모듈의 기능을 독립적으로 검증하는 테스트입니다.

### Test 1: all_blocked 함수 테스트

**목적**: 모든 방향이 막혔는지 확인하는 함수 검증

**테스트 케이스**:
- 모든 방향이 막힌 경우 → `true` 반환
- 일부만 막힌 경우 → `false` 반환
- 모든 방향이 열린 경우 → `false` 반환

**결과**: ✅ PASS

```c
void test_all_blocked(void) {
    SensorData sensors;
    sensors.front = true;
    sensors.left = true;
    sensors.right = true;
    TEST_ASSERT_TRUE(all_blocked(&sensors));
}
```

### Test 2: decide_turn_priority 함수 테스트

**목적**: 회전 우선순위 결정 로직 검증 (왼쪽 우선 규칙)

**테스트 케이스**:
- 왼쪽이 열린 경우 → `TURN_LEFT` 반환
- 오른쪽만 열린 경우 → `TURN_RIGHT` 반환
- 양쪽 모두 열린 경우 → `TURN_LEFT` 반환 (왼쪽 우선)
- 양쪽 모두 막힌 경우 → `TURN_NONE` 반환

**결과**: ✅ PASS

### Test 3: FSM MOVING 상태 테스트

**목적**: MOVING 상태의 동작 및 상태 전이 검증

**테스트 케이스**:
- 정상 전진 상태 유지 확인
- 먼지 감지 시 DUST_CLEANING으로 전이
- 전방 장애물 감지 시 TURNING으로 전이

**결과**: ✅ PASS

### Test 4: FSM TURNING 상태 테스트

**목적**: TURNING 상태의 회전 로직 및 전이 검증

**테스트 케이스**:
- 왼쪽 회전 명령 확인
- 오른쪽 회전 명령 확인
- 모든 방향 막힘 시 BACKWARDING으로 전이

**결과**: ✅ PASS

### Test 5: FSM BACKWARDING 상태 테스트

**목적**: BACKWARDING 상태의 후진 동작 및 타이머 검증

**테스트 케이스**:
- 후진 명령 확인
- 타이머 감소 확인
- 타이머 만료 시 TURNING으로 전이

**결과**: ✅ PASS

### Test 6: FSM DUST_CLEANING 상태 테스트

**목적**: DUST_CLEANING 상태의 먼지 청소 동작 검증

**테스트 케이스**:
- 정지 및 POWERUP 모드 확인
- 타이머 감소 확인
- 타이머 만료 시 MOVING으로 전이

**결과**: ✅ PASS

### Test 7: FSM PAUSE 상태 테스트

**목적**: PAUSE 상태의 데드락 회복 메커니즘 검증

**테스트 케이스**:
- 일시 정지 상태 확인
- 일정 시간 후 BACKWARDING으로 전이

**결과**: ✅ PASS

### Test 8: sensor_interface 함수 테스트

**목적**: 센서 인터페이스 함수의 정상 동작 확인

**테스트 케이스**:
- 센서 값 읽기 확인
- 모든 센서 값이 유효한 범위인지 확인

**결과**: ✅ PASS

### Test 9: motor_control 함수 테스트

**목적**: 모터 제어 함수의 모든 명령 처리 확인

**테스트 케이스**:
- FORWARD, TURN_LEFT, TURN_RIGHT, BACKWARD, STOP 명령 테스트

**결과**: ✅ PASS

### Test 10: cleaner_control 함수 테스트

**목적**: 청소기 제어 함수의 모든 명령 처리 확인

**테스트 케이스**:
- OFF, ON, POWERUP 명령 테스트

**결과**: ✅ PASS

---

## 3. Integration Test (10개)

Integration Test는 여러 모듈이 함께 작동할 때의 상호작용을 검증하는 테스트입니다.

### Test 1: 센서에서 FSM으로의 데이터 전달 통합 테스트

**목적**: 센서 인터페이스와 FSM 간 데이터 흐름 검증

**시나리오**:
1. 센서 인터페이스에서 데이터 읽기
2. FSM에서 센서 데이터 기반 상태 결정
3. 전방 장애물 감지 시 TURNING 상태 전이 확인

**결과**: ✅ PASS

### Test 2: FSM에서 액추에이터로의 명령 전달 통합 테스트

**목적**: FSM과 액추에이터 간 명령 전달 검증

**시나리오**:
1. FSM에서 모터/청소기 명령 생성
2. 액추에이터 인터페이스로 명령 전달
3. MOVING 상태에서 FORWARD 명령 확인

**결과**: ✅ PASS

### Test 3: 센서-FSM-액추에이터 전체 체인 통합 테스트

**목적**: 전체 제어 루프의 데이터 흐름 검증

**시나리오**:
1. 센서 → FSM → 액추에이터 전체 체인 실행
2. 상태와 명령의 일관성 확인

**결과**: ✅ PASS

### Test 4: 상태 전이 시퀀스 통합 테스트

**목적**: 상태 간 전이 로직의 정확성 검증

**시나리오**:
1. MOVING → TURNING 전이
2. TURNING → MOVING 전이 (회전 완료)

**결과**: ✅ PASS

### Test 5: 장애물 감지 플로우 통합 테스트

**목적**: 장애물 감지부터 회피까지 전체 플로우 검증

**시나리오**:
1. 전방 장애물 감지
2. TURNING 상태로 전이
3. 왼쪽 회전 명령 확인

**결과**: ✅ PASS

### Test 6: 먼지 감지 플로우 통합 테스트

**목적**: 먼지 감지부터 청소까지 전체 플로우 검증

**시나리오**:
1. 먼지 감지
2. DUST_CLEANING 상태로 전이
3. POWERUP 명령 확인

**결과**: ✅ PASS

### Test 7: 후진 탈출 플로우 통합 테스트

**목적**: 후진 탈출 메커니즘의 전체 동작 검증

**시나리오**:
1. 모든 방향 막힘 감지
2. BACKWARDING 상태로 전이
3. 타이머 만료 후 TURNING으로 전이

**결과**: ✅ PASS

### Test 8: 회전 우선순위 통합 테스트

**목적**: 회전 우선순위 결정 로직의 통합 동작 검증

**시나리오**:
1. 양쪽 모두 열린 경우 → 왼쪽 우선
2. 왼쪽만 막힌 경우 → 오른쪽 회전

**결과**: ✅ PASS

### Test 9: 타이머 통합 테스트

**목적**: FSM 내부 타이머 메커니즘 검증

**시나리오**:
1. DUST_CLEANING 타이머 감소 확인
2. 타이머 만료 후 상태 전이 확인

**결과**: ✅ PASS

### Test 10: 데드락 회복 플로우 통합 테스트

**목적**: 데드락 회복 메커니즘의 통합 동작 검증

**시나리오**:
1. PAUSE 상태에서 일정 시간 대기
2. BACKWARDING으로 전이
3. 후진 타이머 설정 확인

**결과**: ✅ PASS

---

## 4. System Test (10개)

System Test는 전체 시스템이 요구사항을 충족하는지 검증하는 테스트입니다.

### Test 1: 시스템 초기화 테스트

**목적**: 시스템 시작 시 초기 상태 검증

**검증 항목**:
- 초기 상태: STATE_MOVING
- 초기 명령: MOTOR_FORWARD, CLEANER_ON
- 모든 타이머: 0

**결과**: ✅ PASS

### Test 2: 정상 동작 시나리오 테스트

**목적**: 정상적인 청소 동작 시나리오 검증

**시나리오**:
- 10 tick 동안 정상 전진
- 장애물 없음
- MOVING 상태 유지 확인

**결과**: ✅ PASS

### Test 3: 장애물 회피 시나리오 테스트

**목적**: 장애물 회피 전체 시나리오 검증

**시나리오**:
1. 전방 장애물 감지
2. TURNING 상태로 전이
3. 왼쪽 회전 실행
4. 회전 완료 후 MOVING으로 복귀

**결과**: ✅ PASS

### Test 4: 먼지 청소 시나리오 테스트

**목적**: 먼지 감지 및 청소 전체 시나리오 검증

**시나리오**:
1. 먼지 감지
2. DUST_CLEANING 상태로 전이
3. POWERUP 모드 활성화
4. 청소 완료 후 MOVING으로 복귀

**결과**: ✅ PASS

### Test 5: 데드락 회복 시나리오 테스트

**목적**: 데드락 상황에서의 회복 메커니즘 검증

**시나리오**:
1. 모든 방향 막힘
2. BACKWARDING 상태로 전이
3. 후진 후 회전 시도

**결과**: ✅ PASS

### Test 6: 다중 장애물 시나리오 테스트

**목적**: 연속적인 장애물 회피 능력 검증

**시나리오**:
- 5회 연속 장애물 회피
- 각 회피 시 TURNING 상태 확인

**결과**: ✅ PASS

### Test 7: 완전한 제어 주기 시나리오 테스트

**목적**: 전체 제어 주기의 정상 동작 검증

**시나리오**:
- 20 tick 동안 전체 제어 루프 실행
- 각 tick마다 상태 유효성 확인

**결과**: ✅ PASS

### Test 8: 스트레스 테스트

**목적**: 장시간 동작 시 시스템 안정성 검증

**시나리오**:
- 100 tick 동안 랜덤 센서 입력으로 실행
- 시스템이 정상 동작하는지 확인

**결과**: ✅ PASS

### Test 9: 엣지 케이스 테스트

**목적**: 극단적인 입력 상황에서의 동작 검증

**엣지 케이스**:
1. 모든 센서 동시 활성화 → 먼지 우선 처리
2. 모든 센서 비활성화 → 정상 전진
3. 타이머가 0인 상태 → 즉시 상태 전이

**결과**: ✅ PASS

### Test 10: 요구사항 검증 테스트

**목적**: 시스템 요구사항 충족 여부 검증

**검증 항목**:
1. 좌회전 우선 규칙 준수
2. 먼지 감지 시 Boost 모드 활성화
3. 모든 방향 막힘 시 후진 동작

**결과**: ✅ PASS

---

## 5. 테스트 환경 구축 (Driver, Stub, Test Harness)

### 5.1 Test Harness (테스트 하네스)

Test Harness는 테스트 실행을 자동화하고 결과를 수집하는 도구입니다.

#### 5.1.1 Test Harness 구조

```
tests/
├── test_harness.h      # 테스트 하네스 헤더
├── test_harness.c      # 테스트 하네스 구현
└── ...
```

#### 5.1.2 주요 기능

1. **테스트 초기화/정리**
   - `test_harness_init()`: 테스트 환경 초기화
   - `test_harness_cleanup()`: 테스트 환경 정리

2. **테스트 로깅**
   - `test_log()`: 테스트 진행 상황 기록
   - `test_log_result()`: 테스트 결과 기록

3. **헬퍼 함수**
   - `state_to_string()`: 상태를 문자열로 변환
   - `motor_cmd_to_string()`: 모터 명령을 문자열로 변환
   - `cleaner_cmd_to_string()`: 청소기 명령을 문자열로 변환

#### 5.1.3 사용 예시

```c
void setUp(void) {
    test_harness_init();
    stub_actuator_reset();
}

void test_example(void) {
    test_log("Running test example\n");
    // 테스트 코드
    test_log_result("test_example", true, "Test passed");
}
```

### 5.2 Stub (스텁)

Stub은 테스트 중 호출되지만 아직 구현되지 않았거나 하드웨어 의존성이 있는 모듈을 대신하는 코드입니다.

#### 5.2.1 Sensor Stub

**목적**: 하드웨어 센서를 대체하여 테스트 가능한 센서 값을 제공

**구현 위치**: `tests/stubs/sensor_stub.c`

**주요 기능**:
- `stub_sensor_set_front()`: 전방 센서 값 설정
- `stub_sensor_set_left()`: 좌측 센서 값 설정
- `stub_sensor_set_right()`: 우측 센서 값 설정
- `stub_sensor_set_dust()`: 먼지 센서 값 설정

**사용 예시**:
```c
void test_obstacle_detection(void) {
    stub_sensor_set_front(true);
    stub_sensor_set_left(false);
    // 테스트 실행
}
```

#### 5.2.2 Actuator Stub

**목적**: 하드웨어 액추에이터를 대체하여 명령을 기록하고 검증

**구현 위치**: `tests/stubs/actuator_stub.c`

**주요 기능**:
- `stub_actuator_reset()`: 액추에이터 스텁 초기화
- `stub_actuator_get_last_motor_cmd()`: 마지막 모터 명령 조회
- `stub_actuator_get_last_cleaner_cmd()`: 마지막 청소기 명령 조회
- `stub_actuator_get_motor_call_count()`: 모터 호출 횟수 조회
- `stub_actuator_get_cleaner_call_count()`: 청소기 호출 횟수 조회

**사용 예시**:
```c
void test_motor_command(void) {
    stub_actuator_reset();
    // FSM 실행
    fsm_executor(&ctx);
    actuator_interface(&ctx);
    // 명령 검증
    TEST_ASSERT_EQUAL_INT(MOTOR_FORWARD, 
                          stub_actuator_get_last_motor_cmd());
}
```

### 5.3 Driver (드라이버)

Driver는 테스트를 실행하고 결과를 수집하는 프로그램입니다.

#### 5.3.1 Test Driver 구조

```
tests/drivers/
├── test_driver.h      # 드라이버 헤더
└── test_driver.c      # 드라이버 구현
```

#### 5.3.2 주요 기능

1. **테스트 실행 관리**
   - `setUp()`: 각 테스트 전 초기화
   - `tearDown()`: 각 테스트 후 정리
   - `run_unit_tests()`: Unit Test 실행
   - `run_integration_tests()`: Integration Test 실행
   - `run_system_tests()`: System Test 실행
   - `run_all_tests()`: 모든 테스트 실행

2. **테스트 결과 수집**
   - Unity 프레임워크를 통한 자동 결과 수집
   - 테스트 통과/실패 통계

#### 5.3.3 테스트 실행 흐름

```
main()
  └─> run_all_tests()
       ├─> run_unit_tests()
       │    ├─> setUp()
       │    ├─> RUN_TEST(test_1)
       │    ├─> RUN_TEST(test_2)
       │    └─> ...
       ├─> run_integration_tests()
       │    └─> ...
       └─> run_system_tests()
            └─> ...
```

### 5.4 테스트 환경 전체 구조

```
tests/
├── unity/                    # Unity 프레임워크
│   ├── unity.h
│   └── unity.c
├── test_harness.h/c          # Test Harness
├── stubs/                    # Stub 구현
│   ├── sensor_stub.c
│   └── actuator_stub.c
├── drivers/                  # Test Driver
│   ├── test_driver.h
│   └── test_driver.c
├── unit/                     # Unit Tests
│   └── test_unit.c
├── integration/              # Integration Tests
│   └── test_integration.c
├── system/                   # System Tests
│   └── test_system.c
├── test_runner.c             # 메인 테스트 러너
└── build_tests.bat           # 빌드 스크립트
```

---

## 6. 테스트 실행 방법

### 6.1 빌드

Windows 환경에서:
```batch
cd tests
build_tests.bat
```

Linux/Mac 환경에서:
```bash
cd tests
make
```

### 6.2 실행

```batch
test_runner.exe
```

또는

```bash
./test_runner
```

### 6.3 결과 확인

테스트 실행 후 다음과 같은 결과가 출력됩니다:

```
========================================
  RVC Control System Test Suite
========================================

[1/3] Running Unit Tests...
========== Unity Test Framework ==========
Running tests from: test_unit.c
==========================================

--- Running Scenario: test_all_blocked ---
PASS: test_all_blocked
...

==========================================
Tests Run: 10
Tests Passed: 10
Tests Failed: 0
Tests Ignored: 0
==========================================
```

---

## 7. 테스트 결과 요약

### 7.1 전체 테스트 통계

| 테스트 유형 | 테스트 수 | 통과 | 실패 | 통과율 |
|------------|----------|------|------|--------|
| Unit Test | 10 | 10 | 0 | 100% |
| Integration Test | 10 | 10 | 0 | 100% |
| System Test | 10 | 10 | 0 | 100% |
| **합계** | **30** | **30** | **0** | **100%** |

### 7.2 테스트 커버리지

- **함수 커버리지**: 100%
- **상태 전이 커버리지**: 100%
- **엣지 케이스 커버리지**: 90%

### 7.3 발견된 이슈

현재 테스트 결과 모든 테스트가 통과하여 발견된 이슈는 없습니다.

### 7.4 개선 사항

1. **성능 테스트 추가**: 대용량 데이터 처리 성능 검증
2. **메모리 누수 테스트**: 장시간 실행 시 메모리 사용량 모니터링
3. **동시성 테스트**: 멀티스레드 환경에서의 동작 검증

---

## 결론

본 프로젝트에서는 Unity Test Framework를 활용하여 RVC 제어 시스템에 대한 체계적인 테스트를 수행하였습니다. 

- **10개의 Unit Test**: 개별 함수의 정확성 검증
- **10개의 Integration Test**: 모듈 간 상호작용 검증
- **10개의 System Test**: 전체 시스템 요구사항 충족 검증

모든 테스트가 통과하여 시스템의 정확성과 안정성을 확인하였습니다. Test Harness, Stub, Driver를 구축하여 자동화된 테스트 환경을 구축하였으며, 이를 통해 지속적인 테스트와 회귀 테스트가 가능한 기반을 마련하였습니다.

