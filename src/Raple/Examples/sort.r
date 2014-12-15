
//
//	Examples of sorting algorithms implemented on Raple
//	15.12.2014
//
//	qsort is a quick sort algorithm (a - array, f - first index, l - last index)
//
//	bsort is a primitive bubble sort algorithm
//


sub qsort(a, f, l) {
	var i = f, 
		j = l,
		x = a[(f+l)/2];

	while (i <= j) {
		while (a[i] < x) i = i+1;
		while (a[j] > x) j = j-1;
		
		if (i <= j) {
			if (a[i] > a[j]) {
				var tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}

			i = i+1;
			j = j-1;
		}
	}

	if (i < l) qsort(a, i, l);
	if (f < j) qsort(a, f, j);
}

sub bsort(a) {
	var n = a.size(),
		i = n-1;

	while (i > 0) {
		var j = 0;

		while (j < i) {
			if (a[j] > a[j+1]) {
				var tmp = a[j];
				a[j] = a[j+1];
				a[j+1] = tmp;
			}

			j = j+1;
		}

		i = i-1;
	}
}

