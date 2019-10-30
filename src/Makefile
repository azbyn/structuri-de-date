FILE := lab4_04_*.cpp
CXX := clang++
#CXX_FLAGS := -std=c++17 -Wall -Wextra -g
CXX_FLAGS := -std=c++17 -Wall -Wextra -O3
OUT = test

default: build

build:
	${CXX} ${CXX_FLAGS} ${FILE} -o ${OUT}

run:
	./${OUT}
