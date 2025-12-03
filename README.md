# Software Engineering Homework

이 저장소는 소프트웨어 공학 과제들을 관리하는 저장소입니다.

## 프로젝트 구조

```
software-engineering-hw/
├── src/                 # Project #2 소스 코드 (절대 수정 금지!)
├── tests/               # Project #3 테스트 코드
├── docs/                # 참고 문서
└── README.md            # 이 파일
```

## 프로젝트별 설명

### Project #2 (완료)
- **과제명**: RVC 구조화 분석 및 설계
- **위치**: `project2_archive/`
- **상태**: ✅ 제출 완료

### Project #3 (진행 중)
- **과제명**: Project #2 프로그램에 대한 xUnit 테스트
- **위치**: `tests/`
- **상태**: 작업 중

## ⚠️ 중요 사항

### Project #3 작업 규칙
- **Project #2 코드(`src/`) 절대 수정 금지!**
- 테스트 코드(`tests/`)만 수정 가능
- FSM 원본 동작: 상태 전이 후 **다음 tick**에서 명령 설정됨

## 빠른 시작

### Project #3 제출물 확인
```bash
# 소스 코드: src/
# 테스트 코드: tests/
# 문서: docs/
```

### 테스트 실행
```bash
cd tests

gcc -Wall -std=c99 -DUNIT_TEST -I. -I../src -Iunity unity/unity.c test_harness.c stubs/sensor_stub.c stubs/actuator_stub.c drivers/test_driver.c unit/test_unit.c integration/test_integration.c system/test_system.c test_runner.c ../src/sensors.c ../src/fsm.c ../src/actuators.c ../src/main.c -o test_runner.exe

./test_runner.exe
```

## 참고 문서
- 참고 문서는 `docs/` 폴더에 있습니다.
