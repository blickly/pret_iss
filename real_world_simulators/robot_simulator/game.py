#!/usr/bin/env python

import os,sys
import pygame
from pygame.locals import *

import simulator
import pret_controller

class Game:
  """Game - This class interfaces simulator to pygame."""
    
  def __init__(self, width=640, height=480, boxsize=40, filename=""):
    """Initialize screen, real world simulator, and, if needed,
    PRET core simulator to control a robot"""
    pygame.init()
    self.width = width
    self.height = height
    self.boxsize = boxsize
    self.screen = pygame.display.set_mode((self.width, self.height))

    self.sim = simulator.Simulator(width / boxsize, self.height / boxsize)

    if filename == "":
      self.control = self.keyboard_control
    else:
      self.control = self.pret_control
      for r in self.sim.get_robots():
        pret_robot = r
        break
      self.controller = pret_controller.PretController(pret_robot,
                                                       filename)

  def keyboard_control(self):
    """Do keyboard-based control"""
    for event in pygame.event.get():
      if event.type == pygame.QUIT: 
        sys.exit()
      elif event.type == KEYDOWN:
        for robot in self.sim.get_robots():
          if event.key == K_LEFT:
            robot.steer_left()
          elif event.key == K_RIGHT:
            robot.steer_right()
          elif event.key == K_UP:
            robot.speed_up()
          elif event.key == K_DOWN:
            robot.slow_down()

  def pret_control(self):
    """Do control based on output of a PRET core"""
    self.controller.run_one_cycle()

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

if __name__ == "__main__":
    window = Game(filename = "control_program")
    window.MainLoop()
