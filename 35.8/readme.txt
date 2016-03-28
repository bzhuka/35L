First I tested the singlethread version that I downloaded.
Running "make clean check" outputted:
real	0m48.517s
user	0m48.532s
sys	0m0.003s

To make my multi-threaded version I basically just divided the number of rows
by the number of threads and alternated the different rows of the pictures
with different threads.  For example with 2 threads I just alternated let one
thread do odd numbered rows and the other thread do even numbered rows.  For 3
threads I would let one thread to rows%3 == 0, another rows%3 == 1, and the
last rows%3 == 2.  I didn't really run into any issues.  I had to change their
display into a 3d array though, or it would be all out of order.

Multithreading made it run a lot faster.  It ran roughly twice as fast for
twice the number of threads, losing that near the end.  This still has a lot
of potential for improving speed and performace however.
