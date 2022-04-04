define say
$(info [Project 1] $1)
endef

CC := g++
CC_FLAGS := -std=c++17 -Wall -g

# Determine some more paths
HOSTNAME := $(shell hostname)
SUBMISSION_DIR := .
RESULTS_JSON := ./results.json
RESULTS_JSON_FOR_GRADESCOPE := /autograder/results/results.json

$(call say,Hostname: $(HOSTNAME))
$(call say,Submission dir: $(SUBMISSION_DIR))
$(call say,Results path: $(RESULTS_JSON))
$(call say,Results path: $(RESULTS_JSON_FOR_GRADESCOPE))
#######


# Binary names
BIN_NAME := main
BIN := ./$(BIN_NAME)
#
TESTS_BIN_NAME := main-tests
TESTS_BIN := ./$(TESTS_BIN_NAME)


$(info )


#
default: help
.PHONY: default


#
help:
	@echo "Available targets: run, build, test, clean"
.PHONY: help


#
run:	$(BIN)
	$(BIN)
.PHONY: run


#
build:	$(BIN) $(TESTS_BIN)
.PHONY: build


# Run tests
gradescope: test
tests: test
test: $(TESTS_BIN)
	$(TESTS_BIN)
.PHONY: tests test gradescope


# Build the main binary
$(BIN):	$(SUBMISSION_DIR)/main.cpp
	$(CC) $(CC_FLAGS) $^ -o "$@"


# Build the test binary
$(TESTS_BIN):	$(SUBMISSION_DIR)/CPP_Tests.cpp
	$(CC) $(CC_FLAGS) $^ -o "$@"
	

#
clean:
	-rm -f $(BIN)
	-rm -f $(TESTS_BIN)
	-rm -f $(RESULTS_JSON)
.PHONY: clean



