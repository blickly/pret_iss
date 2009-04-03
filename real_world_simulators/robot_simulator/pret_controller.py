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

class PretController:
   """Controller for the robot that is controlled by our PRET core"""
   wheel_actuator_address = 0x80000400
   left = 0x01
   right = 0x02
   wheel_print_string= ["", "left", "right"]
   motor_actuator_address = 0x80000404
   stop = 0x01
   go = 0x02
   reverse = 0x03
   motor_print_string = ['',"stop", "go", "backup"]

   x_sensor_address = 0x80000500
   y_sensor_address = 0x80000504
   bump_sensor_address = 0x80000508

   block_length = 1024

   def __init__(self, robot, filename):
      """Initialize robot as well as PRET simulator"""
      self.load(filename)
      self.robot = robot

   def load(self, filename):
      """Load the text file containing the names of the SREC files to run."""
      assert os.path.exists(filename), "Unable to find file " + filename
      pret.load(filename)

   def run_controller(self):
      """Run one cycle of the PRET processor simulation, and then use the
      output there to drive the robot"""
      if not pret.run(1):
        print "Requested end of simulation"
        sys.exit()
      if self.robot.crashed():
        print "Robot crashed!"
        sys.exit()
      
      # Pull commands out of actuators
      wheel_command = pret.read_memory(PretController.wheel_actuator_address)
      if wheel_command != 0:
        print "Wheel command was " \
            + PretController.wheel_print_string[wheel_command]          
        pret.write_memory(PretController.wheel_actuator_address, 0)
        if wheel_command == PretController.left:
            self.robot.steer_left()
        elif wheel_command == PretController.right:
            self.robot.steer_right()
    
      motor_command = pret.read_memory(PretController.motor_actuator_address)
      if motor_command != 0:
        print "Motor command was " \
            + PretController.motor_print_string[motor_command]
        pret.write_memory(PretController.motor_actuator_address, 0)
        if motor_command == PretController.go:
            self.robot.go()
        elif motor_command == PretController.stop:
            self.robot.stop()
        elif motor_command == PretController.reverse:
            self.robot.backup()

      # Push data into sensors
      pret.write_memory(PretController.x_sensor_address,
                        int(PretController.block_length * self.robot.get_x()))
      pret.write_memory(PretController.y_sensor_address,
                        int(PretController.block_length * self.robot.get_y()))

      if self.robot.wall_detection():
        pret.write_memory(PretController.bump_sensor_address, 1);
        


class KeyboardController:
   """Controller for the robot that is controlled by a user's keyboard"""

   def __init__(self, robot):
      """Initialize robot we are controlling"""
      self.robot = robot

   def run_controller(self):
      """Read input from the keyboard, and use it to drive the robot"""
      import pygame
      for event in pygame.event.get():
        if event.type == pygame.QUIT:
          sys.exit()
        elif event.type == pygame.KEYDOWN:
          if event.key == pygame.K_LEFT:
            self.robot.steer_left()
          elif event.key == pygame.K_RIGHT:
            self.robot.steer_right()
          elif event.key == pygame.K_UP:
            self.robot.speed_up()
          elif event.key == pygame.K_DOWN:
            self.robot.slow_down()
