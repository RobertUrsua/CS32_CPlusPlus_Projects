## Notice!
The weird structure of this C++ project (absence of header files for classes, pointers to a redundant class etc) is due to the restrictions set by my professor. We were given this weird code skeleton to ensure that our classes didn't add public members and functions to the classes that we were tasked to implement.

# BruinNav

BruinNav is a text-based turn-by-turn navigation system for Los Angeles. It presents the shortest paths between two registered landmarks by applying the A* algorithm. The GeoSpatial data for this project was retrieved from the openstreetmap.org by our professor. This project was the final programming assignment for my Data Structures and Algorithms course at UCLA (CS 32)

## Things I learned, concepts I applied, and challenges I overcame:
* I learned how to properly manage, organize, and annotate large programs
* I developed a good mindset towards approaching huge projects: Develop incrementally and attempt to thoroughly test modules before moving on to other parts that operate on top of them.
* I learned how to develop thorough test cases.
* I applied the concepts of good data structure selection and usage of already existing algorithms for a real world problem.
  * I coded a simple Map data structure implemented as a Binary Search tree
  * I considered the optimal data structures for different kinds and usage of data, given the restrictions set by our professor:
    * Priority queue for heirarchy of nodes in A* Algorithm 
    * Map for associating Landmarks and GeoCoordinates with the roads in which they're located.
  * I implemented the A* algorithm after some self study.

### The only files that I did NOT write are:
* CareysMain.cpp
* spec.pdf
* provided.h
* validlocs.txt
* testmap.txt
* mapdata.txt
* testBruinNav.cpp
