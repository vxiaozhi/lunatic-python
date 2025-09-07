python = require 'python'

globals = python.globals()
print("globals =", globals)

buildins = python.builtins()
print("buildins =", buildins)

example = python.import 'example'
example.greet()


c = example.add_numbers(2, 3)
print("2 + 3 =", c)

my_class = example.MyClass(10)
v = my_class.get_value()
print("my_class.get_value() =", v)