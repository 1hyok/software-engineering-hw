========================================
Project #3 - RVC 제어 시스템 xUnit 테스트
========================================

이 폴더는 Project #3의 제출물입니다.

Project #2는 이미 제출 완료되었으며,
이번 제출물에는 테스트 코드만 포함되어 있습니다.

구성:
- tests/   : xUnit 테스트 프로그램 전체
  - unity/        : Unity 테스트 프레임워크
  - unit/         : Unit Tests (10개)
  - integration/  : Integration Tests (10개)
  - system/       : System Tests (10개)
  - stubs/        : Stub 구현
  - drivers/      : Test Driver
  - TEST_DOCUMENTATION.md : 상세 테스트 문서

테스트 실행:
  Windows: cd tests && build_tests.bat && test_runner.exe
  Linux:   cd tests && make && ./test_runner

문서 생성:
  상위 폴더의 create_test_document.py 실행
  또는 tests/TEST_DOCUMENTATION.md 참고

========================================
주의: Project #2 프로그램(1.c, 2.c, src/)은 
      이미 제출했으므로 이 폴더에 포함하지 않았습니다.
========================================
