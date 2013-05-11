TARGET=libeasysqlite.a

INCLUDES = \
	-I../easySQLite/

SOURCES = \
../easySQLite/SqlValue.cpp \
../easySQLite/SqlCommon.cpp \
../easySQLite/SHA1.cpp \
../easySQLite/SqlField.cpp \
../easySQLite/SqlFieldSet.cpp \
../easySQLite/SqlRecord.cpp \
../easySQLite/SqlTable.cpp \
../easySQLite/SqlRecordSet.cpp \
../easySQLite/SqlDatabase.cpp \
../easySQLite/sqlite3.c


all:

CC = gcc
CXX = g++
CCFLAGS += -MMD -Wall -Werror -fPIC -Wno-unknown-pragmas
CXXFLAGS += -MMD -Wall -Werror -fPIC -Wno-unknown-pragmas
ARFLAGS = cr

DEFINES += -DLINUX

THIS_MAKEFILE := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

OBJ_DIR ?= obj
LIB_DIR = ./lib/linux
BIN_DIR = bin

ifeq ($(DEBUG), 1)
CCFLAGS += -g3 -O0
CXXFLAGS += -g3 -O0
DEFINES += -D_DEBUG -DCOCOS2D_DEBUG=1
OBJ_DIR := $(OBJ_DIR)/debug
LIB_DIR := $(LIB_DIR)/debug
BIN_DIR := $(BIN_DIR)/debug
else
CCFLAGS += -O3
CXXFLAGS += -O3
DEFINES += -DNDEBUG
OBJ_DIR := $(OBJ_DIR)/release
LIB_DIR := $(LIB_DIR)/release
BIN_DIR := $(BIN_DIR)/release
endif

ifndef V
LOG_CC = @echo " CC $@";
LOG_CXX = @echo " CXX $@";
LOG_AR = @echo " AR $@";
LOG_LINK = @echo " LINK $@";
endif

OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(subst ../,,$(OBJECTS))
OBJECTS := $(subst $(COCOS_ROOT),,$(OBJECTS))
OBJECTS := $(addprefix $(OBJ_DIR)/, $(OBJECTS))
DEPS = $(OBJECTS:.o=.d)
CORE_MAKEFILE_LIST := $(MAKEFILE_LIST)
-include $(DEPS)


LIBS = -lrt -lz

.PHONY: all clean

# If the parent Makefile defines $(EXECUTABLE) then define this as the target
# and create a 'make run' rule to run the app.
ifdef EXECUTABLE
TARGET := $(BIN_DIR)/$(EXECUTABLE)

all: $(TARGET)

run: $(TARGET)
	cd $(dir $^) && ./$(notdir $^)

.PHONY: run
endif


TARGET := $(LIB_DIR)/$(TARGET)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_AR)$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.c $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CC)$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

clean:
	rm -R -f ./lib
	rm -R -f ./obj
