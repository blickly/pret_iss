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
   actuator_address = 0x80000400
   left_bump_sensor_address = 0x80000500
   right_bump_sensor_address = 0x80000504
   speed_address = 0x80000404
   
   left = 0x01
   right = 0x02
   dir_mask = 0x0f

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

      command = pret.read_memory(PretController.actuator_address)
      if command != 0:
        print "Command was %d" % command
      pret.write_memory(PretController.actuator_address, 0)
      if command & PretController.dir_mask == PretController.left:
        self.robot.steer_left()
      elif command & PretController.dir_mask == PretController.right:
        self.robot.steer_right()
    
      command = pret.read_memory(PretController.speed_address)
      if command != 0:
        print "Command was %d" % command
        pret.write_memory(PretController.speed_address, 0)
        robot.speed_increment =  command / 1000;
        if robot.speed_increment > 0:
            robot.speed_up()
        elif robot.speed_increment < 0:
            robot.speed_increment = -robot.speed_increment
            robot.slow_down()
        

