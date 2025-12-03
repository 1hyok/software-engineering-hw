@echo off
echo Building RVC Test Suite...

cd /d "%~dp0"

gcc -Wall -Wextra -std=c99 -g -DUNIT_TEST -I. -I../src -Iunity unity/unity.c test_harness.c stubs/sensor_stub.c stubs/actuator_stub.c drivers/test_driver.c unit/test_unit.c integration/test_integration.c system/test_system.c test_runner.c ../src/sensors.c ../src/fsm.c ../src/actuators.c ../src/main.c -o test_runner.exe

if %ERRORLEVEL% EQU 0 (
    echo Build successful!
    echo Run: .\test_runner.exe
) else (
    echo Build failed!
)
