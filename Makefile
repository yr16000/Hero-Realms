SHELL := /bin/bash

SRCDIR := source
INCDIR := include
BINDIR := bin
BUILDDIR := build
TARGET := $(BINDIR)/hero_realms

CXX := g++
CPPFLAGS := -I$(INCDIR)
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2 -Iinclude -Wno-unused-parameter
LDFLAGS := 

# optimisation / debug flags
RELEASE_FLAGS := -O2
DEBUG_FLAGS := -g -O0 -DDEBUG

# collect sources recursively
SRCS := $(shell find $(SRCDIR) -name '*.cpp')
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

.PHONY: all release debug clean run help

all: release

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	@echo "Linking $@"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# compile objects keeping directory structure in build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# ensure output dirs exist
$(BINDIR) $(BUILDDIR):
	@mkdir -p $@

run: all
	@echo "Running $(TARGET)"
	@$(TARGET)

clean:
	@echo "Cleaning build and bin directories"
	@rm -rf $(BUILDDIR) $(BINDIR)

help:
	@printf "Usage:\n  make            (alias for make release)\n  make release    (optimised build)\n  make debug      (debug build)\n  make run        (build then run)\n  make clean\n"