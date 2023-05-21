# For debug mode (default)
#$ make

# For release mode
#$ make mode=release

# For cleaning
#$ make clean

# Define the compiler to be used (g++ in this case)
CXX := g++

# Define compiler flags: set the C++ standard, include the 'lib' directory for header files,
# and enable all warnings
CXXFLAGS := -I./lib -std=c++11 -Wall -v

# Define linker flags (none in this case)
LDFLAGS :=

# Define the source, object, and build directories, as well as the output binary's name
SRC_DIR := ./src
OBJ_DIR := ./obj
BUILD_DIR := ./build
TARGET := wet1

# Check if the mode is set to release; if so, add optimizations and remove debugging
# information; otherwise, enable debugging information
ifeq ($(mode), release)
  CXXFLAGS += -O3 -DNDEBUG
  BUILD_DIR := $(BUILD_DIR)/release
else
  CXXFLAGS += -g
  BUILD_DIR := $(BUILD_DIR)/debug
endif

# Find all source files in the source directory and create a list of corresponding object files
SRCS := $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Declare all and clean as phony targets to ensure they always run
.PHONY: all clean

# Define the 'all' target to build the main executable
all: $(BUILD_DIR)/$(TARGET)

# Define a rule for building the main executable; create the build directory if necessary,
# and link the object files together
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -o $@ $^

# Include the generated dependency files, which contain rules for rebuilding object files
# when their dependencies (header files) change
-include $(DEPS)

# Define a pattern rule for building object files from source files; create the object
# directory if necessary, and compile the source file with the specified flags
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

# Define the 'clean' target to remove the object and build directories
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
