# C-Compiler
A C program compiler using lex and yacc

**Usage** 

```
lex c.l
yacc -d c.y
cc lex.yy.c y.tab.c -ll
./a.out < test.c
```
