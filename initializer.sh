#!/bin/bash

cmp_path=""

flag=$(type -p "gcc")

if [ "$flag" != "" ];
then
        cmp_path=$flag;
fi

flag=$(type -p "g++")

if [ "$flag" != "" ];
then
        cmp_path=$flag;
fi

flag=$(type -p "clang")

if [ "$flag" != "" ];
then
        cmp_path=$flag;
fi

cmp=""

if [ "$cmp_path" == "" ];
then
        read -p "Cannot find cpp compiler. Please specify the path to the executable compiler: " cmp
else
	confirm=""
	cmp=$cmp_path
	read -p "Compiler that was found '${cmp}' Use it? [y/n]: " confirm && [[ $confirm == [yY] || $confirm == [yY][eE][sS] ]]
	if [[ $confirm != [yY] && $confirm != [yY][eE][sS] ]];
	then
		read -p "Specify the compiler's path: " cmp
		
	fi
fi


if [ -d ./cpm ];

then
	echo './cpm already exist. Compilation...'
else
	mkdir cpm
fi

main_comp="${cmp} ./Compiler/src/main.cpp ./Compiler/AST/ast_nodes.cpp ./Compiler/helpers/char_source.cpp ./Compiler/src/lexer.cpp ./Compiler/src/parser.cpp ./Compiler/src/translator.cpp ./Compiler/src/compiler.cpp -std=c++17 -o cpm/cpm"

$main_comp

if [ ! -d ./cpm/details ];
then
	mkdir cpm/details
fi

echo "${cmp}" > cpm/details/cpp_compiler_path

