Raple
=====

Raple (recursive acronym - Raple's Almost Programming LanguagE) - dynamically typed programming language which I developing for fun. It so happened that it also was my diploma project in University.

По-русски
=====

Raple, название является рекурсивным акронимом Raple is Almost Programming LanguagE, - это язык программирования с динамической типизацией, который я разрабатывал для удовольствия еще будучи студентом. Вообще я всегда мечтал создать свой язык, пусть он не получил бы мировую известность, но сам факт, что я создал свой язык был для меня важен :) Начало его разработки было примерно в 2008-м году, в последующий год/два я переписывал его полностью несколько раз, и так случилось, что он стал моим дипломным проектом в 2012-м году. В тех пор я к нему не прикосался :) Исходный код проекта хостился на http://raple.codeplex.com.
Создал этот репозиторий для того, чтобы продолжать его разрабатывать в свободное время.

Вообще, это безумно интересная и увлекательная задача, разработка своего языка.

Пример программы на Raple
=====

Следующий код на языке Raple выводит в консоль только положительные числа из массива.
Выполнение программы начинается с выполнения функции main. В этой функции создается лямбда-функция или вложенная функция, которая является предикатом для выборки положительных чисел из массива. Затем вызывается функция filter, которая проходит по массиву и печатает все элементы, удовлетворяющие переданному предикату.

<code>
   sub filter(array, predicate) {
      var size = array.size();
      var i = 0;
      while (i < size) {
         if (predicate(array[i]) == 1)
            io.println(array[i]);
            
         i=i+1;
      }
   }

</code>


Подробности
=====

Проект состоит из следующих основных частей:
- лексический анализатор
- синтаксический анализатор
- компилятор
- виртуальная машина и среда выполнения

То есть типичный подход к разработке языков :)
Весь проект написан на С++, в среде Microsoft Visual Studio.

Код покрыт модульными тестами.
