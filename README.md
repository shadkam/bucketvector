bucketvector
============

a vector implementation to store data in buckets - as against continuous memory location - more efficient in many cases

To build: 
take both files bucketvector.h and testclass.cpp, and (after editing, if you feel),
$ g++ -Wall -Wextra -o testclass testclass.cpp

To run:
$ ./testclass

Why This:
stl vectors, when they reach capacity, resize themselves (involves allocating total bigger space at a new location) and copy the existing elements over there. This takes time.
bucketvector stores elements in an initial stl-vector, and once it reaches capacity, it allocates a new bucket, and stores the reference of the bucket with it. If a bucket gets filled up, it creates a new bucket - there is no copying involved.
While accessing, based on the index, it calculates which bucket (or initial stl-vector) the element would have been stored, and fetches it from there - thus at access time, slightly more CPU cycles would be used, but it would still be O1.

This performance improvement would be more and more visible as the sizeof vectorElement is more (1), and as numOfElements in vector grow (2) .. 
To make a decision if it would improve things, please edit testclass, replace MyVectorElem with your own vector element, and build / run testclass .. I am pasting the results I got with MyVectorElem ..

Table1 (vv denotes stl-vector, sv denotes bucketvector)
------
numElems, vv.push, vv.access, sv.push, sv.access
255, 0, 0, 0, 0
510, 0, 0, 0, 0
1020, 10, 0, 0, 0
2040, 0, 0, 0, 0
4080, 10, 0, 0, 0
8160, 20, 0, 10, 0
16320, 30, 0, 10, 0
32640, 70, 10, 20, 10
65280, 140, 10, 50, 10
130560, 280, 20, 100, 30
261120, 570, 50, 220, 50
522240, 1140, 90, 440, 90
1044480, 2300, 170, 890, 170
2088960, 4600, 340, 1790, 350
4177920, -1, -1, 3590, 690

As can be seen, the push time, at higher num of elements, is almost 2.5 times less if bucketvector is used. Access time, however, is little bit more in case of bucketvector - but the "little bit" is, in fact, really "little bit" :)

PS: Very few methods of the vector have been implemented - as at this point i just wanted to test plain push and access times .. other methods, if need is felt, can be added .. 

PS1: last row of Table1 shows -1 for stl-vector, as at that size, allocation had failed - bucketvector, though, handled that size properly
