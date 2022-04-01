$cpp_path = ""

$oldPreference = $ErrorActionPreference
$ErrorActionPreference = 'stop'
try{if(Get-Command gcc){$cpp_path = (Get-Command gcc).Path}}
Catch {}
Finally {$ErrorActionPreference=$oldPreference}

$oldPreference = $ErrorActionPreference
$ErrorActionPreference = 'stop'
try{if(Get-Command gcc){$cpp_path = (Get-Command g++).Path}}
Catch {}
Finally {$ErrorActionPreference=$oldPreference}

$oldPreference = $ErrorActionPreference
$ErrorActionPreference = 'stop'
try{if(Get-Command gcc){$cpp_path = (Get-Command clang).Path}}
Catch {}
Finally {$ErrorActionPreference=$oldPreference}

if ( $cpp_path -eq "" ) {
    $cpp_path= Read-Host -Prompt "Compiler could not be found. Please specify the path : "
} else {
    $confirmation = Read-Host "Compiler that was found $cpp_path. Use it? [y/n]"
    if ($confirmation -ne 'y') {
        $cpp_path = Read-Host "Please specify the path : "
    }
}

if (Test-Path -Path "cpm") {
    Write-Output "./cpm already exist. Compilation..."
} else {
    mkdir cpm > $null
}

& $cpp_path ./Compiler/src/main.cpp ./Compiler/AST/ast_nodes.cpp ./Compiler/helpers/char_source.cpp ./Compiler/src/lexer.cpp ./Compiler/src/parser.cpp ./Compiler/src/translator.cpp ./Compiler/src/compiler.cpp -std=c++17 -o cpm/cpm > $null

if (Test-Path -Path "cpm/details") {
} else {
    mkdir cpm/details > $null
}

$cpp_path > cpm/details/cpp_compiler_path
