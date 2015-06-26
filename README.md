Raple
=====

Raple is dynamically typed programming language which I developing for fun. 
Source code was moved to Github from Codeplex.

This lang has a standard compiler architecture with lexical and syntax analyzers, compiler and virtual machine with runtime environment. So it's very interesting problem to create compilers and I'm trying to do it.

Raple doesn't have garbage collector and Windows-only yet.

Example
=====

Implementation of Quick sort algorithm.

```ruby

fn swap(a, i, j) {
   var tmp = a[i];
   a[i] = a[j];
   a[j] = tmp;
}

fn qsort(a, f, l) {
   var i = f, j = l,
       x = a[(f+l)/2];
   while (i <= j) {
      while (a[i] < x) i++;
      while (a[j] > x) j--;
      
      if (i <= j) {
         if (a[i] > a[j])
            swap(a, i, j);
         i++;
         j--;
      }
   }
   if (i < l) qsort(a, i, l);
   if (f < j) qsort(a, f, j);
}

fn main() {
   var a = {8,1,2,6,4,3,9,5,7};
   qsort(a, 0, a.size()-1);
   io.println(a);
}
```


Details
=====

Project modules:
- lexical analyzer
- syntax analyzer
- compiler to raple bytecode
- virtual machine & runtime environment.

Written on C++ in Microsoft Visual Studio environment with unit tests on CppTest.
