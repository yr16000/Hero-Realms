#  Config 
CXX       := g++
CXXFLAGS  := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS   :=

# Dossiers 
SRC_DIR   := source
OBJ_DIR   := build
BIN       := main

# Sources (récursif) 
SRC       := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ       := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEP       := $(OBJ:.o=.d)

# Règles 
all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

# Utilitaires 
.PHONY: all clean run list

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

list:
	@echo "Sources:"; printf " - %s\n" $(SRC)

# Dépendances auto
-include $(DEP)
