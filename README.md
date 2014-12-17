Raple
=====

Raple is dynamically typed programming language which I developing for fun. It so happened that it also was my diploma project in University.

По-русски
=====

Raple, название является рекурсивным акронимом Raple is Almost Programming LanguagE, - это язык программирования с динамической типизацией, который я разрабатывал для фана еще будучи студентом. Вообще, я всегда мечтал создать свой язык программирования - это очень интересно. Начал копать эту тему примерно в 2008-м году, в последующий год/два я переписывал несколько вариантов полностью с нуля, и так случилось, что он вырос во что-то рабочее. Раньше исходный код проекта хостился на http://raple.codeplex.com.
Создал этот репозиторий для того, чтобы продолжать его разрабатывать в свободное время.

Вообще, это безумно интересная и увлекательная задача.

Пример программы на Raple
=====

В качестве примера показана реализация пирамидальной сортировки.

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


Подробности
=====

Проект состоит из следующих основных частей:
- лексический анализатор
- синтаксический анализатор
- компилятор
- виртуальная машина и среда выполнения

То есть типичный подход к разработке языков и компиляторов. Javascript, PHP, Ruby, C#, Java и т.д. построены по такой схеме, естественно в разной степени усложненной.

Весь проект написан на С++, в среде Microsoft Visual Studio.

Код покрыт модульными тестами.
