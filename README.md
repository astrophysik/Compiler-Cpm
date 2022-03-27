# Compiler Cpm

  description
  -----------

Let's introduce a new language called C+-. It consists of variables, constants, literals (string and numeric), print output function and input function. 
The language uses implicit dynamic typing: variables can store values of different types. There are 3 main types:
  * Int (8 bytes)
  * String
  * Float (8 bytes)

The language also supports arithmetic operations "+" and "-" and assignment operator "=". All available types can stand on either side of arithmetic operations. In this case, the conversion rules are as follows (the order of the arguments is not important):
  * String + Int/Float --> number is converted to string
  * String - Int/Float --> The string is converted to an integer or fractional number, and if the string is not numeric, then its value is 0 
  * Float (+, -) Int   --> Integer converted to fractional

Also the language has support for comments. A comment is a line starting with "//" and ending with a newline
 
Code example
```
 var str = "Hello"; // variable is set via var
 str = str + "!"; // variable can be changed
 print(str); // the print function takes one parameter
```
```
 str = 21 + 2 + 18; // can be written to the same variable number 
 print(str) // -> 42;
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
  Implementation
  --------------
In general, the work of the compiler is divided into 3 stages:
  * *lexer*
  This class is responsible for splitting the text into correct language tokens. 
  * *Parser*
  Parser builds AST based on generated tokens from lexer and is responsible for its correctness
  * *Compiler*
  Compiler runs lexer and parser. Getting from the last AST and generates c++ code from it, which will be executed
  
Approximate scheme of work
  ```mermaid
graph cmp
flowchart LR
    direction TB
    subgraph LexerWorker
        direction LR
        CharSource-->Lexer
    end
    subgraph ParserWorker
        Parser
    end
    InputCode-->LexerWorker
    LexerWorker-->Pipe-->ParserWorker
    ParserWorker-->Pipe
    Pipe-->LexerWorker
    ParserWorker-->Compiler
    Compiler-->TranslatedCppCode
    CppCompiler-->OutputBinary
    TranslatedCppCode-->OutputBinary
```
  
  launch
  ------
To start using the compiler, you need to clone repository and run script ***init_compiler.sh***. This generates a folder ***"/cpm"*** with a file ***"cpm.exe"*** inside. ***g++*** is used by default to compile source. You can change the compiler, but note that it must support versions from ***c++17***
  
  Usage
  -----
cpm used similarly to gcc and others: *cpm.exe file [options]...*
```
  help manual
  Usage: cpm.exe file [options]...
  Options:
          --help          Display this information.
          -cmp=<compiler> Use your <compiler>. Compiler must support version from c++17. Default value <compiler> is g++
          -o <file>       Place the output into <file>  
``` 
 
