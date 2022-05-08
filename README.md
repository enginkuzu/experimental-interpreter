# experimental-interpreter
It is a small experimental interpreter example!  
Custom intermediate code input (recursive fibonacci example) parsing and interpreting.  
Optimizations didn't applied, application can be re-design.  

### Build and run
```
$ gcc -Wall -O3 stage1.c stage2.c stage3.c -o app && ./app 
fib(37) = 24157817
Interpreter runs in 1505 ms

fib(37) = 24157817
Real C function runs in 83 ms

Interpreter 18x slow!
```
