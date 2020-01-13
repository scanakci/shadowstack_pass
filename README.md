# llvm-pass-skeleton

A front-end pass for implementing Shadow Stack

Build:

    $ cd llvm-pass-skeleton
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:

    $ clang -Xclang -load -Xclang build/skeleton/libSkeletonPass.* something.c

Thanks to Adrian Sampson <adrian@radbox.org> because of his amazing tutorial (https://www.cs.cornell.edu/~asampson/blog/llvm.html)
