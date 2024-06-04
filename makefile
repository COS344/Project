SRC_DIR = ./src
TARGET = main
files = shader.cpp main.cpp shapes.cpp glad.c

all: clean compile run

clean:
	@cd $(SRC_DIR) && rm -f *.o $(TARGET)

compile:
	@cd $(SRC_DIR) && g++ -g $(files) -lglfw -pthread -lGLEW -ldl -lGL -o $(TARGET)

run:
	@cd $(SRC_DIR) && ./$(TARGET)

.PHONY: all clean compile run