# Makefile for RVC Test Suite

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I. -I../src -Iunity -Itest_harness.h
LDFLAGS = 

# Directories
SRC_DIR = ../src
UNITY_DIR = unity
STUBS_DIR = stubs
DRIVERS_DIR = drivers
UNIT_DIR = unit
INTEGRATION_DIR = integration
SYSTEM_DIR = system

# Source files
UNITY_SRC = $(UNITY_DIR)/unity.c
HARNESS_SRC = test_harness.c
STUB_SRC = $(STUBS_DIR)/sensor_stub.c $(STUBS_DIR)/actuator_stub.c
DRIVER_SRC = $(DRIVERS_DIR)/test_driver.c
UNIT_SRC = $(UNIT_DIR)/test_unit.c
INTEGRATION_SRC = $(INTEGRATION_DIR)/test_integration.c
SYSTEM_SRC = $(SYSTEM_DIR)/test_system.c
RUNNER_SRC = test_runner.c

# Source code files (testable versions)
RVC_SRC = $(SRC_DIR)/sensors.c $(SRC_DIR)/fsm.c $(SRC_DIR)/actuators.c $(SRC_DIR)/main.c

# Object files
UNITY_OBJ = $(UNITY_SRC:.c=.o)
HARNESS_OBJ = $(HARNESS_SRC:.c=.o)
STUB_OBJ = $(STUB_SRC:.c=.o)
DRIVER_OBJ = $(DRIVER_SRC:.c=.o)
UNIT_OBJ = $(UNIT_SRC:.c=.o)
INTEGRATION_OBJ = $(INTEGRATION_SRC:.c=.o)
SYSTEM_OBJ = $(SYSTEM_SRC:.c=.o)
RUNNER_OBJ = $(RUNNER_SRC:.c=.o)
RVC_OBJ = $(RVC_SRC:.c=.o)

# Test executable
TEST_EXE = test_runner.exe

# Default target
all: $(TEST_EXE)

# Build test executable
$(TEST_EXE): $(UNITY_OBJ) $(HARNESS_OBJ) $(STUB_OBJ) $(DRIVER_OBJ) $(UNIT_OBJ) $(INTEGRATION_OBJ) $(SYSTEM_OBJ) $(RUNNER_OBJ) $(RVC_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $(TEST_EXE)"

# Compile Unity framework
$(UNITY_DIR)/unity.o: $(UNITY_DIR)/unity.c $(UNITY_DIR)/unity.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test harness
test_harness.o: test_harness.c test_harness.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile stubs
$(STUBS_DIR)/%.o: $(STUBS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile drivers
$(DRIVERS_DIR)/%.o: $(DRIVERS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile unit tests
$(UNIT_DIR)/%.o: $(UNIT_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile integration tests
$(INTEGRATION_DIR)/%.o: $(INTEGRATION_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile system tests
$(SYSTEM_DIR)/%.o: $(SYSTEM_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test runner
test_runner.o: test_runner.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile RVC source files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
run: $(TEST_EXE)
	./$(TEST_EXE)

# Clean build artifacts
clean:
	rm -f $(UNITY_OBJ) $(HARNESS_OBJ) $(STUB_OBJ) $(DRIVER_OBJ) $(UNIT_OBJ) $(INTEGRATION_OBJ) $(SYSTEM_OBJ) $(RUNNER_OBJ) $(RVC_OBJ) $(TEST_EXE)
	@echo "Clean complete"

.PHONY: all run clean

