BEGIN {
  max = 0;
  min = 9999999
}

{
  e = $3 * 1000000000;
  sum += e;
  a[count++] = e;
  if (e > max)
    max = e;
  if (e < min)
    min = e;
}

END {
  avg = (sum / count);
  for (i = 0; i < count; i++) {
    if (a[i] < (avg * 10))
      _s += ((a[i] - avg) ^ 2);
	else
	  ignores++;
  }

  _s = _s / (count - ignores);
  printf("%10dns %10dns %10dns %10d %10d %10d\n", min, max, avg, count, ignores, sqrt(_s));
  #print min, max, avg, count, ignores, sqrt(_s);
}
