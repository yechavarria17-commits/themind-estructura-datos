CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/TheMind

SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/Game.cpp $(SRC_DIR)/RuleValidator.cpp
HEADERS = $(SRC_DIR)/Card.h $(SRC_DIR)/Hand.h $(SRC_DIR)/Deck.h $(SRC_DIR)/Player.h \
          $(SRC_DIR)/Level.h $(SRC_DIR)/Lives.h $(SRC_DIR)/Stars.h $(SRC_DIR)/Round.h \
          $(SRC_DIR)/Config.h $(SRC_DIR)/RuleValidator.h $(SRC_DIR)/Game.h \
          $(SRC_DIR)/EventLog.h $(SRC_DIR)/GameState.h

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(SOURCES) -o $(TARGET)
	@echo "Compilacion exitosa: $(TARGET)"

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)
