# SPDX-License-Identifier: MIT

CXX := g++
CPPFLAGS := -MMD -MP
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2
LDLIBS := -lallegro -lallegro_image -lallegro_font -lallegro_primitives

TARGET := MineMan
SOURCES := src/main.cpp src/Game.cpp src/Mapa.cpp
OBJECTS := $(SOURCES:.cpp=.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDLIBS)

src/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJECTS) $(DEPENDENCIES) $(TARGET)

-include $(DEPENDENCIES)
