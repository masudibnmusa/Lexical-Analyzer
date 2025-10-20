# C Lexical Analyzer (Tokenizer)

A comprehensive lexical analyzer for the C programming language that breaks down source code into tokens with detailed information.

## Features

- **Token Recognition**: Identifies 24 different token types including keywords, identifiers, literals, operators, and punctuation
- **Multiple Input Methods**: Accepts code via direct input or file loading
- **Detailed Output**: Shows token type, lexeme, line number, and column position
- **Comment Handling**: Supports both single-line (`//`) and multi-line (`/* */`) comments
- **Preprocessor Directives**: Recognizes `#include`, `#define`, etc.
- **Escape Sequence Support**: Properly handles escape sequences in strings and characters

## Token Types Supported

- **Keywords**: `if`, `else`, `while`, `for`, `return`, etc.
- **Data Types**: `int`, `float`, `char`, `void`, etc.
- **Identifiers**: Variable and function names
- **Literals**: Integer, float, string, and character constants
- **Operators**: Arithmetic, comparison, logical, assignment
- **Punctuation**: Parentheses, braces, brackets, semicolons, commas

## Building and Running

### Prerequisites
- GCC compiler
- Make (optional)

### Compilation
```bash
# Using GCC directly
gcc -o lexical_analyzer main.c

# Using Make
make
