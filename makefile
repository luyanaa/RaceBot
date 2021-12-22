CXX			= g++
CXX_FLAGS	= -O -std=c++11 -g

LIBRARIES	= -lsoil -lglad -lglfw3 -lopengl32 -lglu32 -lassimp -lwinmm -lgdi32 -lreactphysics3d
EXECUTABLE	= app.exe

SRC			= src
INCLUDE		= include
LIB			= lib
BUILD		= build
RESOURCES	= resources
PUBLISH		= publish

OBJS		= $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(wildcard $(SRC)/*.cpp $(SRC)/*/*/*.cpp $(SRC)/*/*/*/*.cpp  $(SRC)/*/*.cpp))

all:$(PUBLISH)/$(EXECUTABLE)
	@xcopy /e /y /q $(RESOURCES) $(PUBLISH)
	@echo Success.

run: all
	@cd $(PUBLISH)&&$(EXECUTABLE)

$(PUBLISH)/$(EXECUTABLE):$(OBJS)
	@cls
	@echo Linking $@

	@if not exist $(dir $@) mkdir "$(dir $@)"
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)


build/%.o: src/%.cpp 
	@echo Compiling...
	@echo $@

	@if not exist $(dir $@) mkdir "$(dir $@)"
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) -c $< -o $@

.PHONY:clean
clean:
	@rmdir /S /Q "$(BUILD)" 2>nul || @echo >nul
	@rmdir /S /Q "$(PUBLISH)" 2>nul || @echo >nul

.PHONY:test
test:


