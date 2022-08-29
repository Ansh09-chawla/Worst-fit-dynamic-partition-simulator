Throughout the simulation your program will maintain an ordered list of dynamic partitions. Some
partitions will be marked as occupied, the rest will be marked as free. Occupied partitions will have a
numeric tag attached to it. Each partition will also contain its size in bytes, and the starting address. The
starting address of the first partition should be 0. Your simulator will manipulate this list of partitions as
a result of processing requests. Allocation requests will be processed by finding the most appropriately
sized partition and then allocating a memory from it. Deallocation requests will free up any relevant
occupied partitions, and also merging any adjacent free partitions.

The parameter page_size will denote the page size and requests will contain a list of all requests to
process. The requests are described using the Request class:

When tag>=0, then this is an allocation request, and the size field will then denote the size of the
request. If tag<0 then this is a deallocation request, in which case the size field is not used. You will
report the results of the simulation via the result parameter.


# memsim

skeleton code for Assignment 5 Question 1

# sample test files:
Make sure you also make your own test files.
```
$ $ wc -l test*.txt
      10 test0.txt
      17 test1.txt
       1 test2.txt
       1 test3.txt
   68807 test4.txt
 1000000 test5.txt
 1000000 test6.txt
      16 test7.txt
 2068852 total
```
---
# My results:
```
$ ./memsim 123 < test1.txt
pages requested:                58
largest free partition size:    129
largest free partition address: 221
elapsed time:                   0.001

$ ./memsim 321 < test2.txt
pages requested:                16
largest free partition size:    136
largest free partition address: 5000
elapsed time:                   0.000

$ ./memsim 111 < test3.txt
pages requested:                0
largest free partition size:    0
largest free partition address: 0
elapsed time:                   0.000

$ ./memsim 222 < test4.txt
pages requested:                896
largest free partition size:    995
largest free partition address: 5
elapsed time:                   0.005

$ ./memsim 333 < test5.txt

pages requested:                141824
largest free partition size:    11707
largest free partition address: 29781916
elapsed time:                   0.571

$ ./memsim 606 < test6.txt
pages requested:                3558653
largest free partition size:    8807
largest free partition address: 857672560
elapsed time:                   1.483

$ ./memsim 100000 < test7.txt
pages requested:                1
largest free partition size:    99894
largest free partition address: 106
elapsed time:                   0.000
```
