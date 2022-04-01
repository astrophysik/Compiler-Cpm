# Compiler Cpm

  description
  -----------

Let me introduce you new language called C+-. It consists of variables, constants, literals (string and numeric), print output function and input function. 
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
 print(str); // -> 41
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
The work of the compiler is divided into 3 stages:
  * *lexer* is responsible for splitting the text into correct language tokens. 
  * *Parser* builds AST based on generated tokens from lexer and is responsible for its correctness.
  * *Translator* is responsible for converting AST to C++ code.
  * *Compiler* runs lexer, parser and translator.
  
*Lexer* uses class ***CharSource*** as a wrapper over the input file. It works in ***parallel*** with *Parser*. *Lexer* put the received tokens of one command into a ***pipe***. *Parser* takes ready-made tokens from there and adds in *AST*. The finished *AST* is converted into c++ code using a *Translator*. In order for the translated code to compile correctly, the ***mixed*** class is added to it. The description of which in the string type lies in the ***LIB*** constant. In case of an error in the code, the ***CompileException*** error is thrown.
  
Approximate scheme of class *Compiler*
  ```mermaid
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
    ParserWorker-->Translator
    Translator-->TranslatedCppCode
    CppCompiler-->OutputBinary
    TranslatedCppCode-->OutputBinary
```
  
  launch
  ------
Installation instructions:
```
  git clone https://github.com/Ramzeeset/Compiler-Cpm
  cd Compiler-Cpm/
```
If you are on the Windows platform. You should use the ***initializer.ps1*** script. If you are using linux, then ***initializer.sh***

After that in the folder *cpm/* there will be an executable. The initializer script will try to find popular compilers on your device and ask for confirmation of use or ask you to specify the path to the compiler
  
  Usage
  -----
cpm used similarly to gcc and others: *cpm.exe file [options]...*
```
  help manual
  Usage: cpm.exe file [options]...
  Options:
          --help          Display this information.
          -cmp=<src>      Use your <src>. Compiler must support version from c++17. Default value <src> is stored in details/cpp_compiler_path
          -o <file>       Place the output into <file>
``` 
 
