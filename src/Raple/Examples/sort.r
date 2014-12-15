
//
//	Examples of sorting algorithms implemented on Raple
//	15.12.2014
//
//	qsort is a quick sort algorithm (a - array, f - first index, l - last index)
//
//	bsort is a primitive bubble sort algorithm
//


sub swap(a, i, j) {
   var tmp = a[i];
   a[i] = a[j];
   a[j] = tmp;
}

sub qsort(a, f, l) {
   var i = f, 
      j = l,
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

sub bsort(a) {
   var n = a.size(),
      i = n-1;

   while (i-- > 0) {
      var j = -1;

      while (j++ < i) {
         if (a[j] > a[j+1])
            swap(a, j, j+1);
      }
   }
}
