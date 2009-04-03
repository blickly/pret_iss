import sys
import simulator

class DisplayView:
  """Use pygame to display the simulation to the screen"""

  def __init__(self, sim, width, height, boxsize):
    """Initialize pygame display"""
    import pygame
    self.sim = sim
    self.width, self.height = width, height
    self.boxsize = boxsize

    pygame.init()
    self.screen = pygame.display.set_mode((self.width, self.height))
    self.background = pygame.Surface(self.screen.get_size())
    self.background = self.background.convert()
    self.background.fill((0,0,0))

  def display(self):
    """Display current simulation configuration graphically"""
    import pygame
    # Close screen if quit button is clicked
    for event in pygame.event.get():
      if event.type == pygame.QUIT: 
        sys.exit()

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

class SaveView:
  def __init__(self, sim):
    """Initialize simulator with this view"""
    self.sim = sim

  def display(self):
    """Save current robot locations to a file"""
    print self.sim.get_time(),
    for r in self.sim.get_robots():
      print "[%f,%f] " % (r.get_x(), r.get_y()),
    print ""
