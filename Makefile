LIBFLAGS = -g -O0 -Wall -lGL -lGLU -lm -lglfw
CXX = clang++

SRC_DIR = ./src/
OBJETOS_DIR = ./objs/
BUILD_DIR = ./build/

SRCS = main.cpp \
       solido.cpp \
       render.cpp \
       pipes.cpp \



BIN_NOME = wow

OBJS = $(SRCS:.cpp=.o)

OBJS_FINAL = $(OBJS:%.o=$(OBJETOS_DIR)%.o)

BIN = $(BUILD_DIR)$(BIN_NOME)

all : $(BIN)

$(BIN) : $(OBJS_FINAL)
	$(CXX) $(OBJS_FINAL) ./objs/libSOIL.a -o $(BIN) $(LIBFLAGS)

$(OBJS_FINAL) : $(OBJETOS_DIR)%.o : $(SRC_DIR)%.cpp
	$(CXX) -c $< -o $@



run : all
	cd $(BUILD_DIR) && exec ./$(BIN_NOME)

clean :
	rm  $(BIN) $(OBJS_FINAL)
