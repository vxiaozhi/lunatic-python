#include <stdio.h>
#include <Python.h>

// 确保使用 Python 3.10+ 的 API
#if PY_VERSION_HEX < 0x030A0000
#error "This code requires Python 3.10 or later"
#endif

// 调用 Python 函数并打印结果
void call_python_function(PyObject* module, const char* func_name, PyObject* args) {
    PyObject *pFunc = NULL;
    PyObject *pResult = NULL;
    
    // 获取函数对象
    pFunc = PyObject_GetAttrString(module, func_name);
    if (!pFunc || !PyCallable_Check(pFunc)) {
        PyErr_Print();
        goto cleanup;
    }
    
    // 调用函数
    pResult = PyObject_CallObject(pFunc, args);
    if (!pResult) {
        PyErr_Print();
        goto cleanup;
    }
    
    // 处理返回值
    if (PyLong_Check(pResult)) {
        printf("Result (int): %ld\n", PyLong_AsLong(pResult));
    } else if (PyFloat_Check(pResult)) {
        printf("Result (float): %f\n", PyFloat_AsDouble(pResult));
    } else if (PyUnicode_Check(pResult)) {
        printf("Result (str): %s\n", PyUnicode_AsUTF8(pResult));
    } else {
        printf("Result (unknown type): ");
        PyObject_Print(pResult, stdout, 0);
        printf("\n");
    }
    
cleanup:
    Py_XDECREF(pFunc);
    Py_XDECREF(pResult);
}

// 封装示例4：调用类方法
void call_class_methods(PyObject *pModule) {
    PyObject *pClass = PyObject_GetAttrString(pModule, "MyClass");
    if (!pClass || !PyCallable_Check(pClass)) {
        PyErr_Print();
        return;
    }

    // 创建类实例
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(10));
    PyObject *pInstance = PyObject_CallObject(pClass, pArgs);
    Py_DECREF(pArgs);
    if (!pInstance) {
        PyErr_Print();
        return;
    }

    // 调用实例方法
    PyObject *pMethod = PyObject_GetAttrString(pInstance, "get_value");
    if (!pMethod || !PyCallable_Check(pMethod)) {
        PyErr_Print();
        goto cleanup;
    }
    PyObject *pResult = PyObject_CallObject(pMethod, NULL);
    if (!pResult) {
        PyErr_Print();
        goto cleanup;
    }
    printf("Initial value: %ld\n", PyLong_AsLong(pResult));
    Py_DECREF(pResult);

    pMethod = PyObject_GetAttrString(pInstance, "double_value");
    if (!pMethod || !PyCallable_Check(pMethod)) {
        PyErr_Print();
        goto cleanup;
    }
    pResult = PyObject_CallObject(pMethod, NULL);
    if (!pResult) {
        PyErr_Print();
        goto cleanup;
    }
    printf("Doubled value: %ld\n", PyLong_AsLong(pResult));
    Py_DECREF(pResult);

cleanup:
    Py_XDECREF(pInstance);
    Py_XDECREF(pClass);
}

// 封装示例5：调用异步方法
void call_async_method(PyObject *pModule) {
    PyObject *pClass = PyObject_GetAttrString(pModule, "MyClass");
    if (!pClass || !PyCallable_Check(pClass)) {
        PyErr_Print();
        return;
    }

    // 创建类实例
    PyObject *pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(10));
    PyObject *pInstance = PyObject_CallObject(pClass, pArgs);
    Py_DECREF(pArgs);
    if (!pInstance) {
        PyErr_Print();
        return;
    }

    // 调用异步方法
    PyObject *pMethod = PyObject_GetAttrString(pInstance, "async_add_numbers");
    if (!pMethod || !PyCallable_Check(pMethod)) {
        PyErr_Print();
        goto cleanup;
    }
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(3));
    PyTuple_SetItem(pArgs, 1, PyLong_FromLong(5));

    // 使用asyncio.run执行协程
    PyObject *pAsyncio = PyImport_ImportModule("asyncio");
    if (!pAsyncio) {
        PyErr_Print();
        goto cleanup;
    }
    PyObject *pRun = PyObject_GetAttrString(pAsyncio, "run");
    if (!pRun || !PyCallable_Check(pRun)) {
        PyErr_Print();
        goto cleanup;
    }
    PyObject *pCoroutine = PyObject_CallObject(pMethod, pArgs);
    if (!pCoroutine) {
        PyErr_Print();
        goto cleanup;
    }
    PyObject *pResult = PyObject_CallFunctionObjArgs(pRun, pCoroutine, NULL);
    if (!pResult) {
        PyErr_Print();
        goto cleanup;
    }
    printf("Async result: %ld\n", PyLong_AsLong(pResult));
    Py_DECREF(pResult);
    Py_DECREF(pCoroutine);
    Py_DECREF(pRun);
    Py_DECREF(pAsyncio);
    Py_DECREF(pArgs);
    Py_DECREF(pMethod);

cleanup:
    Py_XDECREF(pInstance);
    Py_XDECREF(pClass);
}

int main(int argc, char *argv[]) {
    // 初始化 Python 解释器
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    printf("Using Python %s\n", Py_GetVersion());

    // 加载 Python 模块
    PyObject *pModule = PyImport_ImportModule("example");
    if (!pModule) {
        PyErr_Print();
        Py_Finalize();
        return 1;
    }

    // 示例1：调用无参数函数
    printf("\nCalling greet():\n");
    call_python_function(pModule, "greet", NULL);

    // 示例2：调用带参数的函数
    printf("\nCalling add_numbers(3, 5):\n");
    PyObject *pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(3));
    PyTuple_SetItem(pArgs, 1, PyLong_FromLong(5));
    call_python_function(pModule, "add_numbers", pArgs);
    Py_DECREF(pArgs);

    // 示例3：调用返回字符串的函数
    printf("\nCalling get_message():\n");
    call_python_function(pModule, "get_message", NULL);

    // 示例4：调用类方法
    printf("\nCalling MyClass methods:\n");
    call_class_methods(pModule);

    // 示例5：调用异步方法
    printf("\nCalling async_add_numbers(3, 5):\n");
    call_async_method(pModule);

    // 清理
    Py_XDECREF(pModule);
    Py_Finalize();
    return 0;
}