/* Unity Test Framework - Implementation */
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

static jmp_buf TestFailures;
static int TestsRun = 0;
static int TestsFailed = 0;
static int TestsIgnored = 0;
static const char* CurrentTestName = "";
static int CurrentTestLineNumber = 0;
static const char* CurrentTestFile = "";

jmp_buf Unity_TestFailures;
const char* Unity_CurrentTestName;
int Unity_CurrentTestLineNumber;

void UnityBegin(const char* filename) {
    CurrentTestFile = filename;
    TestsRun = 0;
    TestsFailed = 0;
    TestsIgnored = 0;
    printf("\n========== Unity Test Framework ==========\n");
    printf("Running tests from: %s\n", filename);
    printf("==========================================\n\n");
}

void UnityEnd(void) {
    printf("\n==========================================\n");
    printf("Tests Run: %d\n", TestsRun);
    printf("Tests Passed: %d\n", TestsRun - TestsFailed - TestsIgnored);
    printf("Tests Failed: %d\n", TestsFailed);
    printf("Tests Ignored: %d\n", TestsIgnored);
    printf("==========================================\n");
}

void UnityConcludeTest(void) {
    TestsRun++;
}

void UnityFail(const char* msg, const UNITY_LINE_TYPE line) {
    TestsFailed++;
    printf("FAIL: %s (line %d)\n", CurrentTestName, line);
    if (msg) {
        printf("  Message: %s\n", msg);
    }
    longjmp(TestFailures, 1);
}

void UnityIgnore(const char* msg, const UNITY_LINE_TYPE line) {
    TestsIgnored++;
    printf("IGNORE: %s (line %d)\n", CurrentTestName, line);
    if (msg) {
        printf("  Message: %s\n", msg);
    }
}

void UnityAssertEqualNumber(const UNITY_INT expected,
                            const UNITY_INT actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber,
                            const UNITY_DISPLAY_STYLE_T style) {
    if (expected != actual) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Expected %d but was %d", expected, actual);
        if (msg) {
            snprintf(buffer, sizeof(buffer), "%s: Expected %d but was %d", msg, expected, actual);
        }
        UnityFail(buffer, lineNumber);
    }
}

void UnityAssertEqualInt(const UNITY_INT expected,
                         const UNITY_INT actual,
                         const char* msg,
                         const UNITY_LINE_TYPE lineNumber) {
    UnityAssertEqualNumber(expected, actual, msg, lineNumber, UNITY_DISPLAY_STYLE_INT);
}

void UnityAssertEqualIntArray(const UNITY_INT* expected,
                               const UNITY_INT* actual,
                               const UNITY_UINT32 num_elements,
                               const char* msg,
                               const UNITY_LINE_TYPE lineNumber,
                               const UNITY_DISPLAY_STYLE_T style) {
    UNITY_UINT32 i;
    for (i = 0; i < num_elements; i++) {
        if (expected[i] != actual[i]) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Array element %d differs. Expected %d but was %d", i, expected[i], actual[i]);
            UnityFail(buffer, lineNumber);
        }
    }
}

void UnityAssertEqualMemory(const void* expected,
                            const void* actual,
                            const UNITY_UINT32 length,
                            const UNITY_UINT32 num_elements,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber) {
    const UNITY_UINT8* exp = (const UNITY_UINT8*)expected;
    const UNITY_UINT8* act = (const UNITY_UINT8*)actual;
    UNITY_UINT32 i;
    for (i = 0; i < length * num_elements; i++) {
        if (exp[i] != act[i]) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Memory differs at byte %d. Expected 0x%02X but was 0x%02X", i, exp[i], act[i]);
            UnityFail(buffer, lineNumber);
        }
    }
}

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber) {
    if (expected == NULL && actual == NULL) {
        return;
    }
    if (expected == NULL || actual == NULL) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Expected %s but was %s", 
                 expected ? expected : "NULL", actual ? actual : "NULL");
        UnityFail(buffer, lineNumber);
        return;
    }
    if (strcmp(expected, actual) != 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Expected \"%s\" but was \"%s\"", expected, actual);
        if (msg) {
            snprintf(buffer, sizeof(buffer), "%s: Expected \"%s\" but was \"%s\"", msg, expected, actual);
        }
        UnityFail(buffer, lineNumber);
    }
}

void UnityAssertEqualStringArray(const char** expected,
                                  const char** actual,
                                  const UNITY_UINT32 num_elements,
                                  const char* msg,
                                  const UNITY_LINE_TYPE lineNumber) {
    UNITY_UINT32 i;
    for (i = 0; i < num_elements; i++) {
        UnityAssertEqualString(expected[i], actual[i], msg, lineNumber);
    }
}

void UnityAssertEqualFloat(const UNITY_FLOAT expected,
                           const UNITY_FLOAT actual,
                           const char* msg,
                           const UNITY_LINE_TYPE lineNumber) {
    UNITY_FLOAT diff = expected - actual;
    if (diff < 0) diff = -diff;
    if (diff > 0.00001f) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Expected %f but was %f", expected, actual);
        if (msg) {
            snprintf(buffer, sizeof(buffer), "%s: Expected %f but was %f", msg, expected, actual);
        }
        UnityFail(buffer, lineNumber);
    }
}

void UnityTestFile(const char* filename) {
    UnityBegin(filename);
}

void UnityDefaultTestRun(void) {
    UnityEnd();
}

#define UNITY_FLOAT_TYPE float
#define UNITY_FLOAT_ABSOLUTE_TYPE float
#define UNITY_DISPLAY_STYLE_POINTER UNITY_DISPLAY_STYLE_HEX32

