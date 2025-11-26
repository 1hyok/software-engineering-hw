/* Unity Test Framework
 * http://www.throwtheswitch.org/unity
 * Copyright (c) 2007-2016 Mike Karlesky, Mark VanderVoord, Greg Williams
 */

#ifndef UNITY_H
#define UNITY_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void UnityBegin(const char* filename);
void UnityEnd(void);
void UnityConcludeTest(void);

void UnityTestFile(const char* filename);
void UnityDefaultTestRun(void);

void UnityAssertEqualNumber(const UNITY_INT expected,
                            const UNITY_INT actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber,
                            const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualInt(const UNITY_INT expected,
                         const UNITY_INT actual,
                         const char* msg,
                         const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualIntArray(const UNITY_INT* expected,
                               const UNITY_INT* actual,
                               const UNITY_UINT32 num_elements,
                               const char* msg,
                               const UNITY_LINE_TYPE lineNumber,
                               const UNITY_DISPLAY_STYLE_T style);

void UnityAssertEqualMemory(const void* expected,
                            const void* actual,
                            const UNITY_UINT32 length,
                            const UNITY_UINT32 num_elements,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualString(const char* expected,
                            const char* actual,
                            const char* msg,
                            const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualStringArray(const char** expected,
                                  const char** actual,
                                  const UNITY_UINT32 num_elements,
                                  const char* msg,
                                  const UNITY_LINE_TYPE lineNumber);

void UnityAssertEqualFloat(const UNITY_FLOAT expected,
                           const UNITY_FLOAT actual,
                           const char* msg,
                           const UNITY_LINE_TYPE lineNumber);

void UnityFail(const char* msg, const UNITY_LINE_TYPE line);

void UnityIgnore(const char* msg, const UNITY_LINE_TYPE line);

#ifndef UNITY_EXCLUDE_SETJMP_H
#include <setjmp.h>
#endif

#ifndef UNITY_EXCLUDE_MATH_H
#include <math.h>
#endif

#ifndef UNITY_EXCLUDE_STDDEF_H
#include <stddef.h>
#endif

#ifndef UNITY_EXCLUDE_STDINT_H
#include <stdint.h>
#endif

#ifndef UNITY_EXCLUDE_STDLIB_H
#include <stdlib.h>
#endif

#ifndef UNITY_EXCLUDE_STRING_H
#include <string.h>
#endif

#ifndef UNITY_EXCLUDE_FLOAT
#define UNITY_FLOAT_TYPE float
#define UNITY_FLOAT_ABSOLUTE_TYPE float
#endif

#ifndef UNITY_DOUBLE_TYPE
#define UNITY_DOUBLE_TYPE double
#endif

#ifndef UNITY_INTERNAL_PTR
#define UNITY_INTERNAL_PTR UNITY_PTR_ATTRIBUTE
#endif

#ifndef UNITY_PTR_ATTRIBUTE
#define UNITY_PTR_ATTRIBUTE
#endif

#ifndef UNITY_LINE_TYPE
#define UNITY_LINE_TYPE UNITY_UINT
#endif

#ifndef UNITY_COUNTER_TYPE
#define UNITY_COUNTER_TYPE UNITY_UINT
#endif

#ifndef UNITY_UINT
#define UNITY_UINT unsigned int
#endif

#ifndef UNITY_INT
#define UNITY_INT int
#endif

#ifndef UNITY_UINT8
#define UNITY_UINT8 unsigned char
#endif

#ifndef UNITY_INT8
#define UNITY_INT8 signed char
#endif

#ifndef UNITY_UINT16
#define UNITY_UINT16 unsigned short
#endif

#ifndef UNITY_INT16
#define UNITY_INT16 signed short
#endif

#ifndef UNITY_UINT32
#define UNITY_UINT32 unsigned long
#endif

#ifndef UNITY_INT32
#define UNITY_INT32 signed long
#endif

#ifndef UNITY_POINTER_WIDTH
#define UNITY_POINTER_WIDTH (sizeof(void*))
#endif

#ifndef UNITY_DISPLAY_STYLE_INT
#define UNITY_DISPLAY_STYLE_INT UNITY_DISPLAY_STYLE_HEX32
#endif

#ifndef UNITY_DISPLAY_STYLE_UINT
#define UNITY_DISPLAY_STYLE_UINT UNITY_DISPLAY_STYLE_HEX32
#endif

#ifndef UNITY_DISPLAY_STYLE_HEX8
#define UNITY_DISPLAY_STYLE_HEX8  (UNITY_UINT8)
#endif

#ifndef UNITY_DISPLAY_STYLE_HEX16
#define UNITY_DISPLAY_STYLE_HEX16 (UNITY_UINT16)
#endif

#ifndef UNITY_DISPLAY_STYLE_HEX32
#define UNITY_DISPLAY_STYLE_HEX32 (UNITY_UINT32)
#endif

#ifndef UNITY_DISPLAY_STYLE_HEX64
#define UNITY_DISPLAY_STYLE_HEX64 (UNITY_UINT64)
#endif

typedef enum
{
    UNITY_DISPLAY_STYLE_INT = UNITY_DISPLAY_STYLE_INT,
    UNITY_DISPLAY_STYLE_INT8 = UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_INT16 = UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_INT32 = UNITY_DISPLAY_STYLE_HEX32,
    UNITY_DISPLAY_STYLE_UINT = UNITY_DISPLAY_STYLE_UINT,
    UNITY_DISPLAY_STYLE_UINT8 = UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_UINT16 = UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_UINT32 = UNITY_DISPLAY_STYLE_HEX32,
    UNITY_DISPLAY_STYLE_HEX8 = UNITY_DISPLAY_STYLE_HEX8,
    UNITY_DISPLAY_STYLE_HEX16 = UNITY_DISPLAY_STYLE_HEX16,
    UNITY_DISPLAY_STYLE_HEX32 = UNITY_DISPLAY_STYLE_HEX32,
    UNITY_DISPLAY_STYLE_HEX64 = UNITY_DISPLAY_STYLE_HEX64,
    UNITY_DISPLAY_STYLE_CHAR = UNITY_DISPLAY_STYLE_HEX8
} UNITY_DISPLAY_STYLE_T;

typedef void (*UnityTestFunction)(void);

#define UNITY_TEST_ASSERT_EQUAL_INT(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(expected), (UNITY_INT)(actual), message, line, UNITY_DISPLAY_STYLE_INT)

#define UNITY_TEST_ASSERT_EQUAL_INT8(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT8)(expected), (UNITY_INT)(UNITY_INT8)(actual), message, line, UNITY_DISPLAY_STYLE_INT8)

#define UNITY_TEST_ASSERT_EQUAL_INT16(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)(expected), (UNITY_INT)(UNITY_INT16)(actual), message, line, UNITY_DISPLAY_STYLE_INT16)

#define UNITY_TEST_ASSERT_EQUAL_INT32(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)(expected), (UNITY_INT)(UNITY_INT32)(actual), message, line, UNITY_DISPLAY_STYLE_INT32)

#define UNITY_TEST_ASSERT_EQUAL_UINT(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(expected), (UNITY_INT)(actual), message, line, UNITY_DISPLAY_STYLE_UINT)

#define UNITY_TEST_ASSERT_EQUAL_UINT8(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT8)(expected), (UNITY_INT)(UNITY_INT8)(actual), message, line, UNITY_DISPLAY_STYLE_UINT8)

#define UNITY_TEST_ASSERT_EQUAL_UINT16(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)(expected), (UNITY_INT)(UNITY_INT16)(actual), message, line, UNITY_DISPLAY_STYLE_UINT16)

#define UNITY_TEST_ASSERT_EQUAL_UINT32(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)(expected), (UNITY_INT)(UNITY_INT32)(actual), message, line, UNITY_DISPLAY_STYLE_UINT32)

#define UNITY_TEST_ASSERT_EQUAL_HEX8(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT8)(expected), (UNITY_INT)(UNITY_INT8)(actual), message, line, UNITY_DISPLAY_STYLE_HEX8)

#define UNITY_TEST_ASSERT_EQUAL_HEX16(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT16)(expected), (UNITY_INT)(UNITY_INT16)(actual), message, line, UNITY_DISPLAY_STYLE_HEX16)

#define UNITY_TEST_ASSERT_EQUAL_HEX32(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT32)(expected), (UNITY_INT)(UNITY_INT32)(actual), message, line, UNITY_DISPLAY_STYLE_HEX32)

#define UNITY_TEST_ASSERT_EQUAL_CHAR(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_INT)(UNITY_INT8)(expected), (UNITY_INT)(UNITY_INT8)(actual), message, line, UNITY_DISPLAY_STYLE_CHAR)

#define UNITY_TEST_ASSERT_EQUAL_PTR(expected, actual, line, message) \
    UnityAssertEqualNumber((UNITY_PTR_ATTRIBUTE const void*)(expected), (UNITY_PTR_ATTRIBUTE const void*)(actual), message, line, UNITY_DISPLAY_STYLE_POINTER)

#define UNITY_TEST_ASSERT_EQUAL_STRING(expected, actual, line, message) \
    UnityAssertEqualString((const char*)(expected), (const char*)(actual), message, line)

#define UNITY_TEST_ASSERT_EQUAL_MEMORY(expected, actual, len, line, message) \
    UnityAssertEqualMemory((const void*)(expected), (const void*)(actual), (UNITY_UINT32)(len), 1, message, line)

#define UNITY_TEST_ASSERT_EQUAL_FLOAT(expected, actual, delta, line, message) \
    UnityAssertEqualFloat((UNITY_FLOAT)(expected), (UNITY_FLOAT)(actual), message, line)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_INT((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_INT8(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_INT8((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_INT16(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_INT16((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_INT32(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_INT32((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_UINT(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_UINT((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_UINT8(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_UINT8((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_UINT16(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_UINT16((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_UINT32(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_UINT32((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_HEX8(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_HEX8((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_HEX16(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_HEX16((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_HEX32(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_HEX32((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_CHAR(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_CHAR((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_PTR(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_PTR((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    UNITY_TEST_ASSERT_EQUAL_STRING((expected), (actual), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len) \
    UNITY_TEST_ASSERT_EQUAL_MEMORY((expected), (actual), (len), __LINE__, NULL)

#define TEST_ASSERT_EQUAL_FLOAT(expected, actual, delta) \
    UNITY_TEST_ASSERT_EQUAL_FLOAT((expected), (actual), (delta), __LINE__, NULL)

#define TEST_ASSERT_TRUE(condition) \
    UNITY_TEST_ASSERT((condition), __LINE__, "Expected TRUE was FALSE")

#define TEST_ASSERT_FALSE(condition) \
    UNITY_TEST_ASSERT(!(condition), __LINE__, "Expected FALSE was TRUE")

#define TEST_ASSERT_NULL(pointer) \
    UNITY_TEST_ASSERT_NULL((pointer), __LINE__)

#define TEST_ASSERT_NOT_NULL(pointer) \
    UNITY_TEST_ASSERT_NOT_NULL((pointer), __LINE__)

#define TEST_FAIL(message) \
    UnityFail((message), __LINE__)

#define TEST_IGNORE(message) \
    UnityIgnore((message), __LINE__)

#define UNITY_TEST_ASSERT(condition, line, message) \
    do { \
        if (!(condition)) { \
            UnityFail((message), (line)); \
        } \
    } while(0)

#define UNITY_TEST_ASSERT_NULL(pointer, line) \
    UNITY_TEST_ASSERT(((pointer) == NULL), (line), "Expected NULL was Not NULL")

#define UNITY_TEST_ASSERT_NOT_NULL(pointer, line) \
    UNITY_TEST_ASSERT(((pointer) != NULL), (line), "Expected Not NULL was NULL")

#define RUN_TEST(func) \
    do { \
        Unity.CurrentTestName = #func; \
        Unity.CurrentTestLineNumber = __LINE__; \
        if (TEST_PROTECT()) { \
            setUp(); \
            func(); \
        } \
        if (TEST_PROTECT()) { \
            tearDown(); \
        } \
        UnityConcludeTest(); \
    } while(0)

#define TEST_PROTECT() (setjmp(Unity.TestFailures) == 0)

#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END() UnityEnd()

#ifdef __cplusplus
}
#endif

#endif

