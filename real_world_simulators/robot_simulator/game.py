#!/usr/bin/env python

import os,sys
import pygame
from pygame.locals import *

import simulator

class Game:
  """Game - This class interfaces simulator to pygame."""
    
  def __init__(self, width=1200,height=800):
    """Initialize screen"""
    pygame.init()
    self.width = width
    self.height = height
    self.screen = pygame.display.set_mode((self.width, self.height))

    self.sim = simulator.Simulator()

  def MainLoop(self):
    """Run the main loop"""
    self.background = pygame.Surface(self.screen.get_size())
    self.background = self.background.convert()
    self.background.fill((0,0,0))
    self.time = 0
    while 1:
      for event in pygame.event.get():
        if event.type == pygame.QUIT: 
          sys.exit()
        elif event.type == KEYDOWN:
          if   event.key == K_LEFT:
            self.sim.get_robot().steer_left()
          elif event.key == K_RIGHT:
            self.sim.get_robot().steer_right()
          elif event.key == K_UP:
            self.sim.get_robot().speed_up()
          elif event.key == K_DOWN:
            self.sim.get_robot().slow_down()

      self.screen.blit(self.background, (0, 0)) 
      if pygame.font:
        font = pygame.font.Font(None, 36)
        text = font.render("Time %s" % self.sim.get_time(), 1, (255, 0, 0))
        textpos = text.get_rect(x=300)
        self.screen.blit(text, textpos)

      pygame.draw.circle(self.screen, pygame.color.Color("white"),
                         (int(self.sim.get_robot().get_x()),
		          int(self.sim.get_robot().get_y())), 10)
      pygame.display.flip()
      self.sim.increment_time()

if __name__ == "__main__":
    window = Game()
    window.MainLoop()
