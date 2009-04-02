#!/usr/bin/env python

import os,sys
import getopt
import pygame

import simulator
import pret_controller

class Game:
  """Game - This class interfaces simulator to pygame."""
    
  def __init__(self, width=640, height=480, boxsize=40, 
               srec_files="", grid_file="grid.txt"):
    """Initialize screen, real world simulator, and, if needed,
    PRET core simulator to control a robot"""
    pygame.init()
    self.width = width
    self.height = height
    self.boxsize = boxsize
    self.screen = pygame.display.set_mode((self.width, self.height))

    self.sim = simulator.Simulator(width/boxsize, self.height/boxsize,
                                   grid_file)

    # Only use specified controller for first robot
    for r in self.sim.get_robots():
      control_robot = r
      break
    if srec_files == "":
      self.controller = pret_controller.KeyboardController(control_robot)
    else:
      self.controller = pret_controller.PretController(control_robot,
                                                       srec_files)

  def control(self):
    """Get control decisions based on active controller"""
    self.controller.run_controller()

    # (but also end the simulation if the window close button is clicked)
    for event in pygame.event.get():
      if event.type == pygame.QUIT: 
        sys.exit()

  def display(self):
    """Display current simulation configuration graphically"""
    # Clear screen
    self.screen.blit(self.background, (0, 0))

    # Display walls
    for x in xrange(self.width / self.boxsize):
      for y in xrange(self.height / self.boxsize):
        box = self.sim.read_grid(x,y)
        if box == simulator.Simulator.wall:
          pygame.draw.circle(self.screen, pygame.color.Color("gray"),
                             (self.boxsize*x, self.height - self.boxsize*y),
                             self.boxsize/2)
    # Print time
    if pygame.font:
      font = pygame.font.Font(None, 36)
      text = font.render("Time %d" % self.sim.get_time(), 1, (255, 0, 0))
      textpos = text.get_rect(x=self.width-200)
      self.screen.blit(text, textpos)

    # Display robot(s)
    for robot in self.sim.get_robots():
      pygame.draw.circle(self.screen, pygame.color.Color("white"),
                 (robot.get_x() * self.boxsize,
                  self.height - robot.get_y() * self.boxsize),
                         self.boxsize/2)

    pygame.display.flip()

  def MainLoop(self):
    """Run the main loop"""
    self.background = pygame.Surface(self.screen.get_size())
    self.background = self.background.convert()
    self.background.fill((0,0,0))
    self.time = 0
    while 1:
      self.control()
      self.display()
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
