# RVC 제어 시스템 테스트 스위트

## 개요

이 디렉토리는 RVC(Robot Vacuum Cleaner) 제어 시스템에 대한 xUnit 테스트를 포함합니다.

## 디렉토리 구조

```
tests/
├── unity/                    # Unity 테스트 프레임워크
│   ├── unity.h
│   └── unity.c
├── test_harness.h/c          # 테스트 하네스
├── stubs/                    # 스텁 구현
│   ├── sensor_stub.c
│   └── actuator_stub.c
├── drivers/                  # 테스트 드라이버
│   ├── test_driver.h
│   └── test_driver.c
├── unit/                     # Unit Tests (10개)
│   └── test_unit.c
├── integration/              # Integration Tests (10개)
│   └── test_integration.c
├── system/                   # System Tests (10개)
│   └── test_system.c
├── test_runner.c             # 메인 테스트 러너
├── TEST_DOCUMENTATION.md     # 상세 테스트 문서
└── README.md                 # 이 파일
```

## 빌드 및 실행

```bash
cd tests

gcc -Wall -std=c99 -DUNIT_TEST -I. -I../src -Iunity unity/unity.c test_harness.c stubs/sensor_stub.c stubs/actuator_stub.c drivers/test_driver.c unit/test_unit.c integration/test_integration.c system/test_system.c test_runner.c ../src/sensors.c ../src/fsm.c ../src/actuators.c ../src/main.c -o test_runner.exe

./test_runner.exe
```

## 테스트 구성

- **Unit Tests**: 10개 - 개별 함수 단위 테스트
- **Integration Tests**: 10개 - 모듈 간 상호작용 테스트
- **System Tests**: 10개 - 전체 시스템 시나리오 테스트

총 30개의 테스트가 포함되어 있습니다.

## 상세 문서

자세한 테스트 내용과 결과는 `TEST_DOCUMENTATION.md`를 참조하세요.
