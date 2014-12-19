
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
   foreach (var i in { a.size()/2-1 -> 0 }) {
      shiftdown(a, i, a.size());
   }

   foreach (var i in { a.size()-1 -> 1 }) {
      swap(a, 0, i);
      shiftdown(a, 0, i);
   }
}

fn main() {
   var a = {8,1,2,6,4,3,9,5,7};
   heapsort(a);
   io.println(a);
}