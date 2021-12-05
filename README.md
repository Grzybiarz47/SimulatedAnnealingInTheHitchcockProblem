# Simulated Annealing In The Hitchcock Problem

Project was created for the needs of Engineering Work.
Objectives of the project:
 - find shortest paths between warehouses and shops in given graph using Dijkstra Algorithm
 - solve The Hitchcock Problem using Transportation Simplex Method (optimal solution)
 - solve The Hitchcock Problem using Simulated Annealing Method (approximal solution)
 
 Requirements:
 - g++ compiler (min ver. 9.3.0)
 - cmake (min ver. 3.0.0)
 
 Building of the project:
 - ./build.sh
 - set debug/release mode in inc/debug.hpp file
 - in debug mode ./run.sh (path to input text file - examples in example data)
 - in release mode directly use ./Hitchcock (path to input text file) from /bin folder
 - you can delete setup and binary file using ./clean.sh
 
 Input file:
 - first line: n - number of nodes in graph
 - next n lines: for each node list of adjacent nodes and distance to them
 - empty line "---"
 - list of shops and demand for each of them
 - empty line "---"
 - list of warehouses and supply for each of them
 - empty line "---"
 
  Output file:
  - paths from every shop to every warehouse (if exist) with cost
  - empty line "---"
  - integer denoting cargo which should be taken from coresponding shop to warehouse
  - empty line "---" 
