#!/usr/bin/env python

import os,sys
import getopt

import simulator
import pret_controller
import view

class Game:
  """Game - This class interfaces simulator to the controller and view."""
    
  def __init__(self, width=640, height=480, boxsize=40, 
               srec_files="", grid_file="grid.txt",
               save = False, load = False, speedup = 10000):
    """Initialize screen, real world simulator, and, if needed,
    PRET core simulator to control a robot"""
    self.width = width
    self.height = height
    self.boxsize = boxsize

    self.speedup = speedup
    if load:
       self.sim = simulator.Simulator(width/boxsize, height/boxsize,
                                      grid_file, time_increment = speedup)
    else:
       self.sim = simulator.Simulator(width/boxsize, height/boxsize,
                                      grid_file)
    if save:
      self.view = view.SaveView(self.sim)
    else:
      self.view = view.DisplayView(self.sim, width, height, boxsize, load)

    # Only use specified controller for first robot
    for r in self.sim.get_robots():
      control_robot = r
      break
    if srec_files == "":
      self.controller = pret_controller.KeyboardController(control_robot)
    else:
      self.controller = pret_controller.PretController(control_robot,
                                                       srec_files)

  def MainLoop(self):
    """Run the main loop"""
    self.time = 0
    while 1:
      self.controller.run_controller()
      if self.sim.get_time() % self.speedup == 0:
        self.view.display()
      self.sim.increment_time()

def main(argv):
  srec_foldername = ""
  grid = "grid.txt"
  save = False
  load = False
  speedup = 10000
  try:                                
    opts, args = getopt.getopt(argv, "p:g:sld:",
                               ["pret=", "grid=", "save", "load", "speed="])
  except getopt.GetoptError:
    print "Invalid arguments: %s" % argv
    sys.exit(2)  
  for opt, arg in opts:
    if opt in ("-p", "--pret"):
      srec_foldername = arg
      print arg
    elif opt in ("-g", "--grid"):
      grid = arg
    elif opt in ("-s", "--save"):
      save = True
    elif opt in ("-l", "--load"):
      load = True
    elif opt in ("-p", "--speed"):
      speedup = int(arg)

  window = Game(srec_files = srec_foldername, grid_file = grid,
                save = save, load = load, speedup = speedup)
  window.MainLoop()

if __name__ == "__main__":
  main(sys.argv[1:])
