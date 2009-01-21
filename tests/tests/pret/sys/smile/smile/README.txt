/*
  Example : Smile using timing instructions.
  Synopsis: Example draws a smile face using the deadline instruction. 
  Author  : Ben Lickly
  
*/

---------------------------------------
Running the Example
---------------------------------------
1. Go to the $PRET_ISS top directory.
$ cd $PRET_ISS

2. Compile the example.
$ ./scripts/compile_threads.py examples/smile/

3. Executing the example.
$ ./src/pret.exe examples/smile/ 60000000

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
	 Start Simulation
---------------------------------------------------- 
   ooooo   
  /     \  
 |  o o  | 
(|   |   |)
 |  ___  | 
  \ \_/ /  
   \___/   
   ooooo   
  /     \  
 |  o o  | 
(|   |   |)
 |  ___  | 
  \ \_/ /  
   \___/   
   ooooo   
  /     \  
 |  o o  | 
(|   |   |)
 |  ___  | 
  \ \_/ /  
   \___/   
   ooooo   
  /     \  
 |  ^ ^  | 
(|   |   |)
 |  ___  | 
  \ \_/ /  
   \___/   
----------------------------------------------------

---------------------------------------------------- 
	 Simulation Statistics
---------------------------------------------------- 

Cycles executed : 60000000

thread[0], , # cycles: 11999994
thread[0], # instructions: 3012
thread[1], , # cycles: 11999994
thread[1], # instructions: 3016
thread[2], , # cycles: 11999994
thread[2], # instructions: 3053
thread[3], , # cycles: 11999994
thread[3], # instructions: 3016
thread[4], , # cycles: 11999994
thread[4], # instructions: 5173
thread[5], , # cycles: 11999994
thread[5], # instructions: 5148



