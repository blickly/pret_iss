/*
  Example : VGA graphic example.
  Synopsis: This example implements the VGA driver. It is used in the context of a sprite-based drawing of an arcade game. 
  Author  : Sungjun Kim
  
*/

---------------------------------------
Running the Example
---------------------------------------
1. Go to the $PRET_ISS top directory.
$ cd $PRET_ISS

2. Compile the example.
$ ./scripts/compile_threads.py examples/graphic/

3. Executing the example.
$ ./src/pret.exe examples/graphic 450000000


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
VGA emul mode

vga initial refresh

vga refreshed

vga refreshed

vga refreshed

start dump

end dump

vga refreshed

vga refreshed

vga refreshed

vga refreshed

---------------------------------------------------- 

---------------------------------------------------- 
	 Simulation Statistics
---------------------------------------------------- 
Cycles executed : 450000000

thread[0], , # cycles: 89999994
thread[0], # instructions: 14999999
thread[1], , # cycles: 89999994
thread[1], # instructions: 14999999
thread[2], , # cycles: 89999994
thread[2], # instructions: 14388455
thread[3], , # cycles: 89999994
thread[3], # instructions: 789474
thread[4], , # cycles: 89999994
thread[4], # instructions: 789474
thread[5], , # cycles: 89999994
thread[5], # instructions: 789473

Core 0: # instructions: 46756874, CPI: 1.92485
Time elapsed: 198.719 seconds

4. Output file is stored in a dump out_fd.dmp. Rename to pbm file.
$ mv out_fd.dmp out_fd.pbm.

5. Use a PBM viewer to open the file.
