# Small Python-Like Interpreter in C

An educational interpreter written in C for a small subset of Python. It demonstrates tokenization, recursive-descent parsing, integer expression evaluation, variables, and executing a source file one line at a time.

## Supported syntax

- Integer literals
- Addition (`+`), subtraction (`-`), and multiplication (`*`)
- Parentheses and unary minus
- Variable assignment, such as `x = 2 + 3 * 4`
- `print(...)` with a variable or arithmetic expression
- Source files processed one line at a time

Multiplication has higher precedence than addition and subtraction. Parentheses can be used to control evaluation order.

## Example

Create a file named `test.py`:

```python
x = 2 + 3 * 4
print(x)
```

Expected output:

```text
14
```

## Build and run

GCC must be available in your terminal. From the project directory, compile the interpreter and pass the source file as an argument:

```sh
gcc -std=c11 -Wall -Wextra main.c parser.c lexer.c -o interpreter
./interpreter test.py
```

On Windows PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra main.c parser.c lexer.c -o interpreter.exe
.\interpreter.exe test.py
```

The source file must be in the current working directory, or you must provide its path.

## Project files

- `main.c` opens the source file and passes each line to the interpreter.
- `lexer.c` and `lexer.h` convert input characters into tokens.
- `parser.c` and `parser.h` parse and execute expressions, assignments, and `print` statements.

## Limitations

This project implements only a small Python-like subset. It uses integers and fixed-size arrays, supports up to 32 variables, and limits variable names to 31 characters. It does not currently support division, floating-point values, strings, comments, or general Python syntax.
