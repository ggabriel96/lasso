.PHONY: all
all: setup build test

.PHONY: setup
setup:
	meson setup build/ -Dexamples=true

.PHONY: build
build:
	meson compile -C build/

.PHONY: test
test:
	meson test -C build/ --print-errorlogs

.PHONY: format
format:
	clang-format --verbose -i include/*.h examples/*.h examples/*.cpp

.PHONY: clean
clean:
	rm build/ -rf
