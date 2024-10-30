SHELL := $(shell which bash)
MICROMAMBA := $(CURDIR)/.micromamba
MAMBA := $(MICROMAMBA)/micromamba
VENV := $(PWD)/.venv
DEPS := $(VENV)/.deps
BIN := $(VENV)/bin
BUILD_TYPE?=debug
BUILD_TYPE_LOWER:=$(shell echo $(BUILD_TYPE) | tr '[:upper:]' '[:lower:]')
BUILD_ROOT:=$(CURDIR)/out/$(BUILD_TYPE_LOWER)
COMPILER := $(BIN)/x86_64-conda-linux-gnu-g++
CMAKE := $(BIN)/cmake
NINJA := $(BIN)/ninja
OUT := $(CURDIR)/out

.PHONY: test help

ifndef VERBOSE
.SILENT:
endif

help:
	grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

$(MAMBA):
	echo "Installing Mamba..."
	$(SHELL) ./install-micromamba.sh "$(MICROMAMBA)"

$(DEPS): environment.yml $(MAMBA)
	echo "Installing dependencies..."
	$(MAMBA) create --quiet --yes -p $(VENV)
	$(MAMBA) install --quiet --yes -p $(VENV) -f environment.yml
	cp environment.yml $(DEPS)

.PHONY: deps
deps: $(DEPS)

clean:
	rm -rf $(VENV)
	rm -rf $(MICROMAMBA)
	rm -rf $(OUT)

.PHONY: config
config: $(BUILD_ROOT)/.configured

$(BUILD_ROOT)/.configured:
	mkdir -p $(BUILD_ROOT)
	$(CMAKE) -S $(CURDIR) -B $(BUILD_ROOT) \
		-GNinja \
		-DCMAKE_MAKE_PROGRAM=$(NINJA) \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_CXX_COMPILER=$(COMPILER)

.PHONY: build
build: deps config
	$(CMAKE) --build $(BUILD_ROOT)

.PHONY: test
test: build
	$(BUILD_ROOT)/test

.PHONY: format
format: deps
	find src -iname '*.hpp' -o -iname '*.cpp' | xargs $(BIN)/clang-format -i
	$(BIN)/clang-format -i test.cpp

patch:
	git add .
	git diff --binary main > aqtc_cpp.patch