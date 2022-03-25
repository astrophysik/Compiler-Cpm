# Compiler_Cpm

# Task
Let's introduce a new language called C+-. It consists of variables, constants, literals (string and numeric), print output function, input input function. 
The language uses implicit dynamic typing: variables can store values ​​of different types. There are 3 main types:
  * Int (8 bytes)
  * String
  * Float (8 bytes)

The language also supports arithmetic operations "+" and "-" and assignment operator "=". All available types can stand on either side of arithmetic operations. In this case, the conversion rules are as follows (the order of the arguments is not important):
  * String + Int/Float --> number is converted to string
  * String - Int/Float --> The string is converted to an integer or fractional number, and if the string is not numeric, then its value is 0 
  * Float (+, -) Int   --> Integer converted to fractional

The output is one file in which all the code is written.
