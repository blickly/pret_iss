#!/usr/bin/env python

import os,sys
import getopt

import simulator
import pret_controller
import view

class Game:
  """Game - This class interfaces simulator to the controller and view."""
    
  def __init__(self, width=640, height=480, boxsize=40, 
               srec_files="", grid_file="grid.txt"):
    """Initialize screen, real world simulator, and, if needed,
    PRET core simulator to control a robot"""
    self.width = width
    self.height = height
    self.boxsize = boxsize

    self.sim = simulator.Simulator(width/boxsize, height/boxsize,
                                   grid_file)

    self.view = view.DisplayView(self.sim, width, height, boxsize)
    #self.view = view.SaveView(self.sim)

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
      if self.sim.get_time() % 1000 == 0:
        self.view.display()
      self.sim.increment_time()

def main(argv):
  srec_foldername = ""
  grid = "grid.txt"
  try:                                
    opts, args = getopt.getopt(argv, "p:g:", ["pret=", "grid="])
  except getopt.GetoptError:
    print "Invalid arguments: %s" % argv
    sys.exit(2)  
  for opt, arg in opts:
    if opt in ("-p", "--pret"):
      srec_foldername = arg
      print arg
    elif opt in ("-g", "--grid"):
      grid = arg

  window = Game(srec_files = srec_foldername, grid_file = grid)
  window.MainLoop()

if __name__ == "__main__":
  main(sys.argv[1:])
