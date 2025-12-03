============================================================
Project #3 과제 요구사항 확인 방법
============================================================

1. 자동 검증 스크립트 실행
   -------------------------
   
   Windows:
   > check_requirements.bat
   
   Python이 설치되어 있는 경우:
   > python check_requirements.py
   
   이 스크립트는 다음을 자동으로 확인합니다:
   - Unity 프레임워크 파일 존재 여부
   - Unit/Integration/System Test 파일 존재 여부
   - Test Harness, Stub, Driver 파일 존재 여부
   - 빌드 파일 존재 여부
   - Word 문서 존재 여부

2. 수동 확인 사항
   ----------------
   
   다음 항목들은 스크립트로 자동 확인이 어려우므로 수동으로 확인해야 합니다:
   
   A. 테스트 개수 확인
      - Unit Test: 정확히 10개인지 확인
      - Integration Test: 정확히 10개인지 확인
      - System Test: 정확히 10개인지 확인
      
      확인 방법:
      - 각 테스트 파일에서 "void test_" 로 시작하는 함수 개수 세기
      - 또는 검토_체크리스트.md의 항목과 대조
   
   B. Word 문서 확인
      - 15장 이상인지 확인
      - 폰트 크기가 10 미만인지 확인
      - 다음 섹션이 모두 포함되어 있는지:
        * Unity 프레임워크 선정 및 설명
        * Unit Test 10개 (내용 및 결과)
        * Integration Test 10개 (내용 및 결과)
        * System Test 10개 (내용 및 결과)
        * Driver, Stub, Test Harness 설명
   
   C. 컴파일 및 실행 확인
      - tests/build_tests.bat 실행하여 컴파일 성공 확인
      - test_runner.exe 실행하여 모든 테스트 통과 확인
   
   D. 검토_체크리스트.md 확인
      - submission/검토_체크리스트.md 파일의 모든 항목 확인

3. 체크리스트 기반 확인
   ---------------------
   
   submission/검토_체크리스트.md 파일을 열어서:
   - 각 담당자별 검토 항목 확인
   - 체크박스에 체크 표시하며 확인
   - 발견된 문제점 기록

4. 최종 확인 사항
   ---------------
   
   제출 전 반드시 확인:
   □ Word 문서가 15장 이상인가?
   □ 폰트 크기가 10 미만인가?
   □ PDF로 변환되었는가?
   □ ZIP 파일에 모든 파일이 포함되어 있는가?
   □ 테스트가 실제로 컴파일되고 실행되는가?
   □ 모든 테스트가 통과하는가?
   □ Project #2 프로그램(src/, 1.c, 2.c)이 제외되었는가?

============================================================
빠른 확인 명령어
============================================================

1. 테스트 함수 개수 확인:
   findstr /C:"void test_" tests\unit\test_unit.c
   findstr /C:"void test_" tests\integration\test_integration.c
   findstr /C:"void test_" tests\system\test_system.c

2. 컴파일 테스트:
   cd tests
   build_tests.bat

3. 테스트 실행:
   cd tests
   test_runner.exe

============================================================

