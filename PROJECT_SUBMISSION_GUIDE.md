# 프로젝트 제출 가이드

## 제출물 요구사항

1. **Word 문서** (폰트 크기 10 미만, 15장 이상) → PDF로 변환
2. **C + Test Program 전체** → .zip 파일로 압축

## 단계별 가이드

### 1단계: Word 문서 생성

#### 방법 1: Python 스크립트 사용 (권장)

```bash
# python-docx 설치 (필요한 경우)
pip install python-docx

# Word 문서 생성
python create_test_document.py
```

이 스크립트는 `RVC_Test_Documentation.docx` 파일을 생성합니다.

#### 방법 2: 수동 생성

`tests/TEST_DOCUMENTATION.md` 파일의 내용을 Word 문서로 변환하세요.

### 2단계: PDF 변환

1. Word에서 `RVC_Test_Documentation.docx` 파일 열기
2. "파일" > "다른 이름으로 저장" 선택
3. 파일 형식을 "PDF"로 선택
4. 저장

또는 온라인 변환 도구 사용:
- https://www.ilovepdf.com/word-to-pdf
- https://smallpdf.com/word-to-pdf

### 3단계: 프로젝트 압축

```bash
# Python 스크립트 사용
python create_zip.py
```

또는 수동으로 다음 항목을 포함하여 압축:

```
프로젝트.zip
├── src/                    # 소스 코드
│   ├── types.h
│   ├── sensors.c
│   ├── fsm.c
│   ├── actuators.c
│   └── main.c
├── tests/                  # 테스트 코드
│   ├── unity/
│   ├── unit/
│   ├── integration/
│   ├── system/
│   ├── stubs/
│   ├── drivers/
│   └── ...
├── 1.c                      # 제출용 단일 파일
├── 2.c                      # 제출용 단일 파일
└── README.md
```

### 4단계: 최종 확인

제출 전 확인 사항:

- [ ] Word 문서가 15장 이상인가?
- [ ] 폰트 크기가 10 미만인가?
- [ ] PDF로 변환되었는가?
- [ ] .zip 파일에 모든 소스 코드와 테스트 코드가 포함되어 있는가?
- [ ] 테스트 문서에 다음 내용이 포함되어 있는가?
  - Unity 프레임워크 선정 및 설명
  - Unit Test 10개 (내용 및 결과)
  - Integration Test 10개 (내용 및 결과)
  - System Test 10개 (내용 및 결과)
  - Driver, Stub, Test Harness 설명

## 테스트 실행 방법

### Windows

```batch
cd tests
build_tests.bat
test_runner.exe
```

### Linux/Mac

```bash
cd tests
make
./test_runner
```

## 문제 해결

### 빌드 오류 발생 시

1. GCC 컴파일러가 설치되어 있는지 확인
2. 모든 소스 파일이 올바른 위치에 있는지 확인
3. include 경로가 올바른지 확인

### 테스트 실행 오류 시

1. `test_runner.exe` 파일이 생성되었는지 확인
2. 필요한 라이브러리가 링크되었는지 확인

## 문의

문제가 발생하면 다음을 확인하세요:
- `tests/README.md` - 테스트 디렉토리 설명
- `tests/TEST_DOCUMENTATION.md` - 상세 테스트 문서

