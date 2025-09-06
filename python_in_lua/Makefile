PYTHON_FLAGS = $(shell python3.10-config --cflags --ldflags --embed)
PYTHON_LIB_PATH = $(shell python3.10-config --prefix)/lib
LUA_PATH = ../lua-5.4.6/src
LUA_FLAGS = -I../lua-5.4.6/src  -L../lua-5.4.6/src -llua

all: call_python lua_mod

call_python:
	gcc -o call_python call_python.c $(PYTHON_FLAGS) -L $(PYTHON_LIB_PATH)

lua_mod:
	gcc -shared -fPIC -o lua_python.so lua_python.c $(PYTHON_FLAGS) $(LUA_FLAGS) -L $(PYTHON_LIB_PATH)

run:
	./call_python

run_lua:
	$(LUA_PATH)/lua lua_example.lua
	
clean:
	rm call_python lua_python.so

