
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
   qsort(a);
   io.println(a);
}