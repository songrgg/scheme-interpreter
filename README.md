scheme-interpreter
==================

It's a simple scheme interpreter which was written in C on *nix platform.

For run this scheme interpreter, you just need to download it on a *nix operating system, compile them with GCC or CLANG Compiler, it's easy to just go to the code folder and run:
$ gcc *.c -o scheme
$ ./sheme
scheme> (define (fact n) 
         (if (= n 0)
             1
             (* n (fact (- n 1)))))
=> nil
scheme> (fact 10)
=> 3628800
