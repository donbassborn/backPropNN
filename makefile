COMPILER=g++
PROJECT_DIR=$(shell echo %cd%)

BIN_SDIR=bin
OBJ_SDIR=obj
SRC_SDIR=src
EXEC=debug.exe

override CFLAGS+=
override CXXFLAGS+=-c -std=c++17 -Iexternal\lodepng\include
override LDFLAGS+=-Linclude -llodepng
override INCLIBS+=

SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
OBJ_ABS=$(addprefix $(PROJECT_DIR)\$(FORK_SDIR)\$(OBJ_SDIR)\,$(OBJ))

all: $(EXEC)

$(EXEC): $(SRC)
	@if not exist $(PROJECT_DIR)\$(FORK_SDIR)\$(BIN_SDIR) mkdir $(PROJECT_DIR)\$(FORK_SDIR)\$(BIN_SDIR)
	$(COMPILER) $(OBJ_ABS) $(INCLIBS) -o $(PROJECT_DIR)\$(FORK_SDIR)\$(BIN_SDIR)\$(EXEC) $(LDFLAGS)
$(SRC):
	@if not exist $(dir $(PROJECT_DIR)\$(FORK_SDIR)\$(OBJ_SDIR)\$(@)) mkdir $(dir $(PROJECT_DIR)\$(FORK_SDIR)\$(OBJ_SDIR)\$(@))
	$(COMPILER) $(CFLAGS) $(CXXFLAGS) $(PROJECT_DIR)\$(SRC_SDIR)\$(@) -o $(PROJECT_DIR)\$(FORK_SDIR)\$(OBJ_SDIR)\$(@:.cpp=.o)

clean:
	if exist $(PROJECT_DIR)\debug rmdir /s /q $(PROJECT_DIR)\debug
	if exist $(PROJECT_DIR)\release rmdir /s /q $(PROJECT_DIR)\release