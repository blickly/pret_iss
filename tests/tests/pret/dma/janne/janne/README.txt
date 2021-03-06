/*
  Example : Janne algorithm. 
  Synopsis: Malardalen benchmark.
  Author  : Malardalen WCET benchmark.
  
*/

---------------------------------------
Running the Example
---------------------------------------
1. Go to the $PRET_ISS top directory.
$ cd $PRET_ISS

2. Compile the example.
$ ./scripts/compile_threads.py examples/janne/

3. Executing the example.
$ ./src/pret.exe examples/janne -1

---------------------------------------------------- 
	PRET Configuration
----------------------------------------------------

Number of threads         : 6
Number of register windows: 128
Number of deadline timers : 8
Number of PLL timers      : 1

Clock setup

Memory hierarchy
Main memory delay          : 15
Instruction/Data scratchpads
----------------------------------------------------

---------------------------------------------------- 
	 Start Simulation
---------------------------------------------------- 
(a,b) = (3,-9)
(a,b) = (14,11)
(a,b) = (17,23)
(a,b) = (19,13)
(a,b) = (22,29)
(a,b) = (24,19)
(a,b) = (27,47)
(a,b) = (29,37)
(a,b) = (31,27)
---------------------------------------------------- 


---------------------------------------------------- 
	 Simulation Statistics
---------------------------------------------------- 
Cycles executed : -1

thread[0], , # cycles: 114024
thread[0], # instructions: 19004
thread[1], , # cycles: 114024
thread[1], # instructions: 1001
thread[2], , # cycles: 114018
thread[2], # instructions: 1000
thread[3], , # cycles: 114018
thread[3], # instructions: 1000
thread[4], , # cycles: 114018
thread[4], # instructions: 1000
thread[5], , # cycles: 114018
thread[5], # instructions: 1000

