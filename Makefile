# SPDX-License-Identifier: MIT

CXX := g++
CPPFLAGS := -MMD -MP
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2
LDLIBS := -lallegro -lallegro_image -lallegro_font -lallegro_primitives

empty :=
space := $(empty) $(empty)
comma := ,

SOURCES := src/main.cpp src/Game.cpp src/Mapa.cpp
CPP_OBJECTS := $(SOURCES:.cpp=.o)
RESOURCE_OBJECT :=

ifeq ($(OS),Windows_NT)
TARGET := MineMan.exe
RESOURCE_OBJECT := src/MineMan.res.o
CLEAN = powershell.exe -NoProfile -Command "Remove-Item -Force -ErrorAction SilentlyContinue $(subst $(space),$(comma),$(subst /,\,$(1))); exit 0"
else
TARGET := MineMan
CLEAN = $(RM) $(1)
endif

OBJECTS := $(CPP_OBJECTS) $(RESOURCE_OBJECT)
DEPENDENCIES := $(CPP_OBJECTS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

ifeq ($(OS),Windows_NT)
src/MineMan.res.o: resources/MineMan.rc Icons/MineMan.ico
	windres $< -O coff -o $@
endif

run: $(TARGET)
	./$(TARGET)

clean:
	-$(call CLEAN,$(OBJECTS) $(DEPENDENCIES) MineMan MineMan.exe)

-include $(DEPENDENCIES)
