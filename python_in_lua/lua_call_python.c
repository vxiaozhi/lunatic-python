#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <Python.h>

// Lua 调用 Python 函数的封装
static int lua_call_python_function(lua_State *L) {
    const char *mod_name = luaL_checkstring(L, 1);
    const char *func_name = luaL_checkstring(L, 2);
    PyObject *pModule = PyImport_ImportModule(mod_name);
    if (!pModule) {
        luaL_error(L, "Failed to load Python module 'example'");
        return 0;
    }

    PyObject *pResult = PyObject_CallMethod(pModule, func_name, NULL);
    if (!pResult) {
        PyErr_Print();
        luaL_error(L, "Python function call failed");
        return 0;
    }

    if (PyLong_Check(pResult)) {
        lua_pushinteger(L, PyLong_AsLong(pResult));
    } else if (PyUnicode_Check(pResult)) {
        lua_pushstring(L, PyUnicode_AsUTF8(pResult));
    } else {
        lua_pushnil(L);
    }

    Py_DECREF(pResult);
    Py_DECREF(pModule);
    return 1;
}

static int lua_call_new_class(lua_State *L) {
    const char *mod_name = luaL_checkstring(L, 1);
    const char *class_name = luaL_checkstring(L, 2);
    PyObject *pModule = PyImport_ImportModule(mod_name);
    if (!pModule) {
        luaL_error(L, "Failed to load Python module 'example'");
        lua_pushnil(L);
        return 1;
    }


    PyObject *pClass = PyObject_GetAttrString(pModule, class_name);
    if (!pClass || !PyCallable_Check(pClass)) {
        PyErr_Print();
        luaL_error(L, "Failed to get Python class 'ExampleClass'");
        return 0;
    }

    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(10));
    PyObject *pInstance = PyObject_CallObject(pClass, pArgs);
    Py_DECREF(pArgs);

    if (!pInstance) {
        PyErr_Print();
        luaL_error(L, "Failed to create Python class instance");
        return 0;
    }

    lua_pushlightuserdata(L, pInstance);

    // 这里不能释放，否则lua_call_class_methods会出问题
    //Py_DECREF(pInstance);

    Py_DECREF(pClass);
    Py_DECREF(pModule);
    return 1;
}

// Lua 调用类方法的封装
static int lua_call_class_methods(lua_State *L) {
    // First param, class object
    PyObject *pInstance = lua_touserdata(L, 1);
    // Second param, method name
    const char *method_name = luaL_checkstring(L, 2);

    if (!pInstance) {
        luaL_error(L, "Invalid class object");
        return 0;
    }
    printf("pInstance: %p\n", pInstance);

    // 调用实例方法
    PyObject *pMethod = PyObject_GetAttrString(pInstance, method_name);
    if (!pMethod || !PyCallable_Check(pMethod)) {
        PyErr_Print();
        goto cleanup;
    }
    PyObject *pResult = PyObject_CallObject(pMethod, NULL);
    if (!pResult) {
        PyErr_Print();
        goto cleanup;
    }
    //printf("Initial value: %ld\n", PyLong_AsLong(pResult));
    Py_DECREF(pResult);


cleanup:
    if (pMethod)  Py_DECREF(pMethod);

    return 0;
}

// Lua 调用异步方法的封装
static int lua_call_async_method(lua_State *L) {
    int a = luaL_checkinteger(L, 1);
    int b = luaL_checkinteger(L, 2);
    PyObject *pModule = PyImport_ImportModule("example");
    if (!pModule) {
        luaL_error(L, "Failed to load Python module 'example'");
        return 0;
    }

    PyObject *pAsyncio = PyImport_ImportModule("asyncio");
    if (!pAsyncio) {
        luaL_error(L, "Failed to load Python module 'asyncio'");
        return 0;
    }

    PyObject *pCoroutine = PyObject_CallMethod(pModule, "async_add_numbers", "ii", a, b);
    PyObject *pResult = PyObject_CallMethod(pAsyncio, "run", "O", pCoroutine);
    lua_pushinteger(L, PyLong_AsLong(pResult));

    Py_DECREF(pResult);
    Py_DECREF(pCoroutine);
    Py_DECREF(pAsyncio);
    Py_DECREF(pModule);
    return 1;
}

static int lua_test_echo(lua_State *L) {
    const char *msg = luaL_checkstring(L, 1);
    lua_pushstring(L, msg);
    return 1;
}

// 模块函数注册表
static const luaL_Reg lua_python_lib[] = {
    {"call_python_function", lua_call_python_function},
    {"call_new_class", lua_call_new_class},
    {"call_class_methods", lua_call_class_methods},
    {"call_async_method", lua_call_async_method},
    {"test_echo", lua_test_echo},
    {NULL, NULL}
};

static void init_python() {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    printf("Using PythonVer: %s\n", Py_GetVersion());
}

// 模块入口函数
int luaopen_lua_call_python(lua_State *L) {
    init_python();
    luaL_newlib(L, lua_python_lib);
    return 1;
}