Raple
=====

Raple is dynamically typed programming language which I developing for fun. Earlier source code was hosted on codeplex.com and now I moved it to github.

This project has a standart compiler architecture with lexical and syntax analyzers, compiler and virtual machine with runtime environment. So it's very interesting problem to create compilers and I'm trying to do it.

Raple doesnt have garbage collector and working under Windows yet.

Example of program on Raple
=====

To show the main features of Raple I have implemented an heap sort algorithm.

```ruby

sub xrange(l, r) {
   var a = {};
   while (l >= r) {
      a.add(l--);
   }
   return a;
}

sub shiftdown(a, i, j) {
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

sub heapsort(a) {
   foreach (var i in xrange(a.size()/2 - 1, 0)) {
      shiftdown(a, i, a.size());
   }

   foreach (var i in xrange(a.size()-1, 1)) {
      swap(a, 0, i);
      shiftdown(a, 0, i);
   }
}

sub main() {
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
