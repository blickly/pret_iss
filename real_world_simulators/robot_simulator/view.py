import sys
import simulator

class DisplayView:
  """Use pygame to display the simulation to the screen"""

  def __init__(self, sim, width, height, boxsize, display_from_file = False):
    """Initialize pygame display"""
    import pygame
    self.sim = sim
    self.width, self.height = width, height
    self.boxsize = boxsize

    if display_from_file:
      self.load_file = open('save.out')
      self.time = -1
    else:
      self.load_file = False

    pygame.init()
    self.screen = pygame.display.set_mode((self.width, self.height))
    self.background = pygame.Surface(self.screen.get_size())
    self.background = self.background.convert()
    self.background.fill((0,0,0))

  def decode_line_from_file(self, line):
    """Decode a line from a saved file"""
    try:
      split_line = line.split()
      time_str = split_line.pop(0)
      split_line = [pos_str.lstrip('[').rstrip(']').partition(',')
                        for pos_str in split_line]
      robot_pos = [(float(x),float(y)) for (x,_,y) in split_line]
      return int(time_str), robot_pos
    except:
      print "Parsing error in save file on line '%s'" % line

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
    if self.load_file:
      while self.time < self.sim.get_time():
        current_line = self.load_file.readline()
        if current_line == None or current_line == '':
          print "Simulation finished"
          self.load_file.close()
          exit(0)
        self.time, robotpos = self.decode_line_from_file(current_line)
      for pos in robotpos:
        pygame.draw.circle(self.screen, pygame.color.Color("white"),
                 (pos[0] * self.boxsize, self.height - pos[1] * self.boxsize),
                 self.boxsize/2)
    else:
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
    self.file = open('save.out', 'w')

  def display(self):
    """Save current robot locations to a file"""
    this_line = "%d" % self.sim.get_time()
    for r in self.sim.get_robots():
      this_line += " [%f,%f]" % (r.get_x(), r.get_y())
    this_line += '\n'
    self.file.write(this_line)
