local lua_python = require "lua_python"

-- 测试调用 Python 函数
local greet_result = lua_python.call_python_function("example", "greet")
print("greet() result:", greet_result)

-- 测试调用类方法
local class_obj = lua_python.call_new_class("example", "MyClass")
print("class_obj:", class_obj)
lua_python.call_class_methods(class_obj, "show_msg")


-- 测试调用异步方法
-- local async_sum = lua_python.call_async_method(3, 5)
-- print("async_add_numbers(3, 5) result:", async_sum)
