# Source to HTML Converter

A C program that converts any C source code file into a colorful HTML file with syntax highlighting like VS Code.

### Features
- Converts .c file to .html with syntax highlighting
- Highlights Keywords, Comments, Strings in different colors
- Preserves indentation and formatting
- Generates ready-to-open HTML file
- Handles preprocessor directives

### How it Works
- Reads C source file line by line
- Identifies token types (keyword, comment, string, etc.)
- Wraps them in HTML <span> with CSS colors
- Generates final HTML with <pre> tag

### Tech Stack
- Language: C
- Concepts: File Handling, String Processing, HTML Generation
- Compiler: GCC

### How to Run
```bash
gcc main.c -o converter
./converter input.c output.html
# Now open output.html in browser
