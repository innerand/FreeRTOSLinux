TARGET=exec
BUILD_DIR ?= ./build


# -------------------------
FREERTOS_VERSION="V10.2.1"
FREERTOS_INCLUDE_DIRS := \
FreeRTOS/Source/include
FREERTOS_SOURCES := \
FreeRTOS/Source/croutine.c \
FreeRTOS/Source/event_groups.c\
FreeRTOS/Source/list.c\
FreeRTOS/Source/queue.c \
FreeRTOS/Source/stream_buffer.c \
FreeRTOS/Source/tasks.c \
FreeRTOS/Source/timers.c
# -------------------------
SRCS += $(FREERTOS_SOURCES)
INCLUDE_DIRS += $(FREERTOS_INCLUDE_DIRS)

# -------------------------
EXAMPLE_INCLUDE_DIRS := \
example
EXAMPLE_SOURCES := \
example/main.c
# -------------------------
SRCS += $(EXAMPLE_SOURCES)
INCLUDE_DIRS += $(EXAMPLE_INCLUDE_DIRS)

# -------------------------
PORT_INCLUDE_DIRS := \
portable/GCC/Linux/
PORT_SOURCES := \
portable/GCC/Linux/port.c\
FreeRTOS/Source/portable/MemMang/heap_4.c

# -------------------------
SRCS += $(PORT_SOURCES)
INCLUDE_DIRS += $(PORT_INCLUDE_DIRS)

LDFLAGS += -lrt -lpthread
CPPFLAGS += -g

# Build files
OBJS   := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS   := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))
$(info srcs [${INC_FLAGS}])

# Link
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build C sources
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Build C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

.PHONY: get_freertos clean

# Checkout FreeRTOS from SVN Repository
get_freertos:
	svn checkout https://svn.code.sf.net/p/freertos/code/tags/$(FREERTOS_VERSION)/FreeRTOS/Source FreeRTOS/Source

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

