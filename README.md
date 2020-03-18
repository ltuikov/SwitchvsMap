# Test results without optimization
Testing switch vs. map verbatim, without
any linear map guessing of the switch by the compiler.
The results show that the map is about 2.5-3x faster,
as expected.
```
$time ./test-lookup
Testing data size of 1e+06 lookups, in seconds
   map:  0.002903
switch:  0.008037
   map:  0.002899
switch:  0.008032
Testing data size of 1e+09 lookups, in seconds
   map:  2.837281
switch:  7.812687
   map:  2.840947
switch:  7.855245

real    0m48.449s
user    0m45.184s
sys     0m3.095s
$_
```

# Test results with -O2 optimization
The compiler figures out the linear transformation
of the map, and uses it. However a map is still
slightly faster.

It's worth to note that this is a very simple linear
transformation, and should a switch-case be used
for a more complex map, the compiler may not be able
to figure it out and may resort to a switch-case
whose timings are shown above.
```
$time ./test-lookup
Testing data size of 1e+06 lookups, in seconds
   map:  0.001334
switch:  0.001300
   map:  0.001301
switch:  0.001295
Testing data size of 1e+09 lookups, in seconds
   map:  1.290082
switch:  1.297465
   map:  1.298246
switch:  1.301629

real    0m29.591s
user    0m26.350s
sys     0m3.114s
$_
```
