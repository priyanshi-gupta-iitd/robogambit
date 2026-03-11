PYTHON ?= python
CXX ?= g++

MODULE := robogambit_cpp
SOURCES := bindings.cpp bitboard_eval.cpp move_application.cpp move_generation.cpp
HEADERS := bitboard_eval.h move_application.h move_generation.h files.h

PY_INCLUDES := $(shell $(PYTHON) -m pybind11 --includes)
EXT_SUFFIX := $(shell $(PYTHON) -c "import sysconfig; print(sysconfig.get_config_var('EXT_SUFFIX') or '.pyd')")
TARGET := $(MODULE)$(EXT_SUFFIX)

CXXFLAGS ?= -O3 -Wall -shared -std=c++17

# Required by most Unix-like toolchains when producing Python extensions.
ifeq ($(OS),Windows_NT)
    CXXFLAGS += -DMS_WIN64
else
    CXXFLAGS += -fPIC
endif

.PHONY: all clean run help

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(PY_INCLUDES) $(SOURCES) -o $@

clean:
	-$(RM) $(TARGET)
	-$(RM) *.o
	-$(RM) *.obj

run:
	$(PYTHON) visualiser.py

help:
	@echo "Targets:"
	@echo "  make all   - Build the robogambit_cpp Python extension"
	@echo "  make run   - Run the Python visualiser"
	@echo "  make clean - Remove build artifacts"
