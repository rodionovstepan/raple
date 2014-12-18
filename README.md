Raple
=====

Raple is dynamically typed programming language which I developing for fun. Earlier source code was hosted on codeplex.com and now I moved it to github.

This project has a standart compiler architecture with lexical and syntax analyzers, compiler and virtual machine with runtime environment. So it's very interesting problem to create compilers and I'm trying to do it.

Raple doesnt have garbage collector and working under Windows yet.

Example of program on Raple
=====

To show the main features of Raple I have implemented an heap sort algorithm.

```ruby

fn swap(a, i, j) {
  var tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

fn shiftdown(a, i, j) {
   var done = false, maxchild = 0;

   while ((i*2 + 1 < j) && done == false) {
      if (i*2 + 1 == j-1)
         maxchild = i*2 + 1;
      else if (a[i*2 + 1] > a[i*2 + 2])
         maxchild = i*2 + 1;
      else
         maxchild = i*2 + 2;

      if (a[i] < a[maxchild]) {
         swap(a, i, maxchild);
         i = maxchild;
      } else done = true;
   }
}

fn heapsort(a) {
   for (var i in { a.size()/2-1 -> 0 }) {
      shiftdown(a, i, a.size());
   }

   for (var i in { a.size()-1 -> 1 }) {
      swap(a, 0, i);
      shiftdown(a, 0, i);
   }
}

fn main() {
   var a = {9,1,7,2,6,3,5,4,8};
   heapsort(a);
   io.println(a);
}
```


Details
=====

Project parts:
- lexical analyzer
- syntax analyzer
- compiler to raple bytecode
- virtual machine & runtime environment.

Written on C++ in Microsoft Visual Studio environment with unit tests on CppTest.
