/*
  Example : Simple mutual exclusion example using timing instructions.
  Synopsis: This is a producer/consumer example where the mutual
exclusion is enforced through the use of timing instructions, and
being able to precisely determine the execution times of operations on
it.
  Author  : Ben Lickly and Hiren Patel
  
*/

---------------------------------------
Running the Example
---------------------------------------
1. Go to the $PRET_ISS top directory.
$ cd $PRET_ISS

2. Compile the example.
$ ./scripts/compile_threads.py examples/sync_example_cases/

3. Executing the example.
$ ./src/pret.exe examples/sync_example_cases/ -1

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
 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~

---------------------------------------------------- 
	
---------------------------------------------------- 
	 Simulation Statistics
---------------------------------------------------- 
Cycles executed : -1

thread[0], , # cycles: 21600
thread[0], # instructions: 480
thread[1], , # cycles: 21600
thread[1], # instructions: 954
thread[2], , # cycles: 21600
thread[2], # instructions: 676
thread[3], , # cycles: 21600
thread[3], # instructions: 190
thread[4], , # cycles: 21594
thread[4], # instructions: 189
thread[5], , # cycles: 21594
thread[5], # instructions: 189

