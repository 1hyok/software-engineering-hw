@echo off
REM Build script for RVC Test Suite on Windows

echo ========================================
echo Building RVC Test Suite
echo ========================================

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c99 -g -I. -I../src -Iunity
set LDFLAGS=

REM Create object files
echo Compiling Unity framework...
%CC% %CFLAGS% -c unity/unity.c -o unity/unity.o

echo Compiling test harness...
%CC% %CFLAGS% -c test_harness.c -o test_harness.o

echo Compiling stubs...
%CC% %CFLAGS% -c stubs/sensor_stub.c -o stubs/sensor_stub.o
%CC% %CFLAGS% -c stubs/actuator_stub.c -o stubs/actuator_stub.o

echo Compiling drivers...
%CC% %CFLAGS% -c drivers/test_driver.c -o drivers/test_driver.o

echo Compiling unit tests...
%CC% %CFLAGS% -c unit/test_unit.c -o unit/test_unit.o

echo Compiling integration tests...
%CC% %CFLAGS% -c integration/test_integration.c -o integration/test_integration.o

echo Compiling system tests...
%CC% %CFLAGS% -c system/test_system.c -o system/test_system.o

echo Compiling test runner...
%CC% %CFLAGS% -c test_runner.c -o test_runner.o

echo Compiling RVC source files...
%CC% %CFLAGS% -c ../src/sensors.c -o ../src/sensors.o
%CC% %CFLAGS% -c ../src/fsm.c -o ../src/fsm.o
%CC% %CFLAGS% -c ../src/actuators.c -o ../src/actuators.o
%CC% %CFLAGS% -c ../src/main.c -o ../src/main.o

echo Linking test executable...
%CC% %CFLAGS% -o test_runner.exe ^
    unity/unity.o ^
    test_harness.o ^
    stubs/sensor_stub.o ^
    stubs/actuator_stub.o ^
    drivers/test_driver.o ^
    unit/test_unit.o ^
    integration/test_integration.o ^
    system/test_system.o ^
    test_runner.o ^
    ../src/sensors.o ^
    ../src/fsm.o ^
    ../src/actuators.o ^
    ../src/main.o ^
    %LDFLAGS%

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo.
    echo Run tests with: test_runner.exe
) else (
    echo.
    echo ========================================
    echo Build failed!
    echo ========================================
    exit /b 1
)

