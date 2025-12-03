/* Unity Test Framework - Implementation */
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

struct UNITY_STORAGE_T Unity;

void UnityBegin(const char* filename) {
    Unity.TestFile = filename;
    Unity.CurrentTestName = "";
    Unity.CurrentTestLineNumber = 0;
    Unity.NumberOfTests = 0;
    Unity.TestFailures = 0;
    Unity.TestIgnores = 0;
    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;

    printf("\n========== Unity Test Framework ==========\n");
    printf("Running tests from: %s\n", filename);
    printf("==========================================\n\n");
}

void UnityEnd(void) {
    printf("\n==========================================\n");
    printf("Tests Run: %d\n", Unity.NumberOfTests);
    printf("Tests Passed: %d\n", Unity.NumberOfTests - Unity.TestFailures - Unity.TestIgnores);
    printf("Tests Failed: %d\n", Unity.TestFailures);
    printf("Tests Ignored: %d\n", Unity.TestIgnores);
    printf("==========================================\n");
}

void UnityConcludeTest(void) {
    if (Unity.CurrentTestIgnored) {
        Unity.TestIgnores++;
    } else if (!Unity.CurrentTestFailed) {
        // Passed
    } else {
        Unity.TestFailures++;
    }
    
    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;
    Unity.NumberOfTests++;
}

void UnityFail(const char* msg, const UNITY_LINE_TYPE line) {
    Unity.CurrentTestFailed = 1;
    printf("FAIL: %s (line %d)\n", Unity.CurrentTestName, line);
    if (msg) {
        printf("  Message: %s\n", msg);
    }
    longjmp(Unity.AbortFrame, 1);
}

void UnityIgnore(const char* msg, const UNITY_LINE_TYPE line) {
    Unity.CurrentTestIgnored = 1;
    printf("IGNORE: %s (line %d)\n", Unity.CurrentTestName, line);
    if (msg) {
        printf("  Message: %s\n", msg);
    }
    longjmp(Unity.AbortFrame, 1);
}

void UnityAssertEqualNumber(const UNITY_INT expected,
                            const UNITY_INT actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber,
                            const UNITY_DISPLAY_STYLE_T style) {
    (void)style; // Unused parameter
    if (expected != actual) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "Expected %d but was %d", expected, actual);
        if (msg) {
            char msgBuffer[256];
            snprintf(msgBuffer, sizeof(msgBuffer), "%s: %s", msg, buffer);
            UnityFail(msgBuffer, lineNumber);
        } else {
            UnityFail(buffer, lineNumber);
        }
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
    (void)style; // Unused parameter
    for (i = 0; i < num_elements; i++) {
        if (expected[i] != actual[i]) {
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Array element %u differs. Expected %d but was %d", i, expected[i], actual[i]);
            if (msg) {
                char msgBuffer[256];
                snprintf(msgBuffer, sizeof(msgBuffer), "%s: %s", msg, buffer);
                UnityFail(msgBuffer, lineNumber);
            } else {
                UnityFail(buffer, lineNumber);
            }
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
            snprintf(buffer, sizeof(buffer), "Memory differs at byte %u. Expected 0x%02X but was 0x%02X", i, exp[i], act[i]);
            if (msg) {
                char msgBuffer[256];
                snprintf(msgBuffer, sizeof(msgBuffer), "%s: %s", msg, buffer);
                UnityFail(msgBuffer, lineNumber);
            } else {
                UnityFail(buffer, lineNumber);
            }
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
            char msgBuffer[256];
            snprintf(msgBuffer, sizeof(msgBuffer), "%s: %s", msg, buffer);
            UnityFail(msgBuffer, lineNumber);
        } else {
            UnityFail(buffer, lineNumber);
        }
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
            char msgBuffer[256];
            snprintf(msgBuffer, sizeof(msgBuffer), "%s: %s", msg, buffer);
            UnityFail(msgBuffer, lineNumber);
        } else {
            UnityFail(buffer, lineNumber);
        }
    }
}

void UnityTestFile(const char* filename) {
    UnityBegin(filename);
}

void UnityDefaultTestRun(void) {
    UnityEnd();
}


