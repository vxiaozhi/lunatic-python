import asyncio

print("#example init!!!")

def greet():
    """无参数函数示例"""
    print("Hello from Python!")
    return 42

def add_numbers(a, b):
    """带参数的函数示例"""
    print(f"Adding {a} and {b}")
    return a + b

def get_message():
    """返回字符串的函数示例"""
    return "This is a message from Python"

class MyClass:
    """示例类"""
    def __init__(self, value=0):
        self.value = value
    
    def show_msg(self):
        """打印类属性的值"""
        print(f"show msg,Value: {self.value}")
        
    def get_value(self):
        """返回类属性的值"""
        return self.value
    
    def double_value(self):
        """将类属性的值翻倍并返回"""
        return self.value * 2
    
    async def async_add_numbers(self, a, b):
        """异步函数示例"""
        print(f"async Adding {a} and {b}")
        return a + b


def NewMyClass():
    """示例工厂函数"""
    return MyClass("created by NewMyClass")   

if __name__ == "__main__":
    greet()
    add_numbers(1, 2)
    get_message()
    my_class = MyClass(10)
    my_class.get_value()
    my_class.double_value()
    my_class.show_msg()
    asyncio.run(my_class.async_add_numbers(1, 2))
    
    class2 = NewMyClass()
    class2.show_msg()