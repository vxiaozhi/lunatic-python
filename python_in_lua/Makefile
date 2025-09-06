PYTHON_FLAGS = $(shell python3.10-config --cflags --ldflags --embed)
PYTHON_LIB_PATH = $(shell python3.10-config --prefix)/lib
LUA_PATH = ../lua-5.4.6/src
LUA_FLAGS = -I../lua-5.4.6/src  -L../lua-5.4.6/src -llua

all: c_call_python lua_call_python python_inlua

c_call_python:
	gcc -o c_call_python c_call_python.c $(PYTHON_FLAGS) -L $(PYTHON_LIB_PATH)

lua_call_python:
	gcc -shared -fPIC -o lua_call_python.so lua_call_python.c $(PYTHON_FLAGS) $(LUA_FLAGS) -L $(PYTHON_LIB_PATH)

python_inlua:
	gcc -shared -fPIC -o python.so python_inlua.c $(PYTHON_FLAGS) $(LUA_FLAGS) -L $(PYTHON_LIB_PATH)

run:
	./c_call_python

run_lua:
	$(LUA_PATH)/lua lua_example.lua

run_python_inlua:
	$(LUA_PATH)/lua  python_inlua_example.lua
	
clean:
	rm c_call_python lua_call_python.so python.so

