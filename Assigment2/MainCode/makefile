MODULE_NAME = cmdproc

# Paths
UNITY_ROOT = ..
SRC_FOLDER = src
TEST_FOLDER = test

CLEANUP = rm -f
MKDIR = mkdir -p

C_COMPILER = gcc
CFLAGS += -Wall

TARGET = test$(MODULE_NAME)
SRC_FILES = $(UNITY_ROOT)/src/unity.c $(TEST_FOLDER)/test$(MODULE_NAME).c
INC_DIRS = -I$(SRC_FOLDER) -I$(UNITY_ROOT)/src
SYMBOLS =

all: clean default

.PHONY: clean

default: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $^ -o testcmdproc

clean:
	$(CLEANUP) $(TARGET)
