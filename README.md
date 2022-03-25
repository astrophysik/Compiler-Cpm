# Compiler_Cpm

# Task
Let's introduce a new language called C+-. It consists of variables, constants, literals (string and numeric), print output function, input input function. 
The language uses implicit dynamic typing: variables can store values of different types. There are 3 main types:
  * Int (8 bytes)
  * String
  * Float (8 bytes)

The language also supports arithmetic operations "+" and "-" and assignment operator "=". All available types can stand on either side of arithmetic operations. In this case, the conversion rules are as follows (the order of the arguments is not important):
  * String + Int/Float --> number is converted to string
  * String - Int/Float --> The string is converted to an integer or fractional number, and if the string is not numeric, then its value is 0 
  * Float (+, -) Int   --> Integer converted to fractional

The input is one file in which all the code is written. Also compiler has several options:
  * --help        --> Call help manual
  * -c=<compiler> --> Specifies the compiler to process the translated code.
  * -o <file>     --> Place the output into <file>
 
Code example
```
 var str = "Hello"; // variable is set via var
 str = str + "!"; // variable can be changed
 print(str); // the print function takes one parameter
```
```
 str = 21 + 2 + 18; // can be written to the same variable number 
 print(str) // -> 42
``` 
```
 val worldString = "World"; // the constant is set via val 
 val answerNumber = 42;
``` 
```
 print(worldString + answerNumber); // -> World42
 print(worldString - answerNumber); // -> -42
 
 val pi = 3.14;
 print(pi + answerNumber); // 45.14
``` 
```
 val numberString = "134";
 print(numberString - answerNumber); // -> 92
 print(numberString + answerNumber); // -> 13492
```
```
 val name = input("Name: "); // outputs "Name: " and waits for input
 print("Your name is " + name);
``` 
 
 
