#!/usr/bin/env python
####################################################################
#Copyright (c) 2009 The Regents of the University of California.
#All rights reserved.
#
#Permission is hereby granted, without written agreement and without
#license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in all
# copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
#                         PT_COPYRIGHT_VERSION_2
#                         COPYRIGHTENDKEY
####################################################################

import sys
import os
import simulator

pret_dir = os.environ['PRET_ISS']
debugger_dir = os.path.join(pret_dir, 'src/debugger')
sys.path.append(debugger_dir)
import libpretdbg
pret = libpretdbg

class PretController():
   """Controller for the robot that is controlled by our PRET core"""
   wheel_actuator_address = 0x80000400
   bump_sensor_address = 0x80000508
   left = 0x01
   right = 0x02
   motor_actuator_address = 0x80000404
   stop = 0x01
   go = 0x02

   x_sensor_address = 0x80000500
   y_sensor_address = 0x80000504

   block_length = 1024

   def __init__(self, robot, filename):
      """Initialize robot as well as PRET simulator"""
      self.load(filename)
      self.robot = robot

   def load(self, filename):
      """Load the text file containing the names of the SREC files to run."""
      assert os.path.exists(filename), "Unable to find file " + filename
      pret.load(filename)

   def run_one_cycle(self):
      """Run one cycle of the PRET processor simulation, and then use the
      output there to drive the robot"""
      if not pret.run(1):
        print "Requested end of simulation"
        sys.exit()
      if self.robot.crashed():
        print "Robot crashed!"
        sys.exit()
      
      if self.robot.wall_detection():
        print "Detected crash"
        command = pret.read_memory(PretController.bump_sensor_address)
        if command == 0:
            pret.write_memory(PretController.bump_sensor_address, 1);
        
      
        

      # Pull commands out of actuators
      wheel_command = pret.read_memory(PretController.wheel_actuator_address)
      if wheel_command != 0:
        print "Wheel command was %d" % wheel_command          
        pret.write_memory(PretController.wheel_actuator_address, 0)
        if wheel_command == PretController.left:
            self.robot.steer_left()
        elif wheel_command == PretController.right:
            self.robot.steer_right()
    
      motor_command = pret.read_memory(PretController.motor_actuator_address)
      if motor_command != 0:
        print "Motor command was %d" % motor_command
        pret.write_memory(PretController.motor_actuator_address, 0)
        if motor_command == PretController.go:
            self.robot.go()
        elif motor_command == PretController.stop:
            self.robot.stop()

      # Push data into sensors
      pret.write_memory(PretController.x_sensor_address,
                        int(PretController.block_length * self.robot.get_x()))
      pret.write_memory(PretController.y_sensor_address,
                        int(PretController.block_length * self.robot.get_y()))

        

