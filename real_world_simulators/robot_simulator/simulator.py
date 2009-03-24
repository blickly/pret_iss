import math

class Simulator:
  """Simulator - This class provides a simulation of our robot example"""
  empty = 0
  block = 1
  robot = 2

  def __init__(self, width, height):
    """Initialize time and a single robot"""
    e = Simulator.empty
    b = Simulator.block
    r = Simulator.robot
    self.world = [[b,b,b,b,b,b,b,b,b,b,b,b,b,b,b],
                  [b,e,e,e,e,e,e,e,e,e,e,e,e,e,b],
                  [b,e,e,e,b,e,e,b,e,b,e,e,e,e,b],
                  [b,e,e,e,b,e,e,e,e,e,e,e,e,e,b],
                  [b,b,e,e,b,e,e,b,e,e,b,b,b,e,b],
                  [b,e,e,e,e,b,e,e,e,e,e,e,e,b,b],
                  [b,e,e,e,b,e,e,b,e,b,e,e,e,e,b],
                  [b,b,b,e,b,e,e,b,e,e,b,e,b,e,b],
                  [b,e,e,e,e,e,e,e,e,e,e,e,e,e,b],
                  [b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b]]
    self.time = 0
    self.width = width
    self.height = height
    self.robots = [Robot(self)]

  def read_grid(self, x, y):
    try:
      return self.world[y][x]
    except:
      return Simulator.block

  def get_robots(self):
    """Get a given robot"""
    return self.robots

  def get_time(self):
    """Get current simulation time"""
    return self.time

  def increment_time(self):
    """Increment simulation time"""
    self.time = self.time + 1
    for robot in self.get_robots():
      if not robot.crashed():
        robot.increment_time()

class Robot:
  """Robot - This class provides a robot abstraction"""
  speed_increment = 0.001

  def __init__(self, simulator, x=1, y=1, heading=(1,0)):
    """Initialize robot parameters"""
    self.sim = simulator
    self.x = x
    self.y = y
    self.heading = heading
    self.speed = 0.001
 
  def steer_left(self):
    """Move steering to the left"""
    (hx, hy) = self.heading
    (hx, hy) = (-hy, hx) # Rotate 90 degrees to left
    self.heading = (hx,hy)

  def steer_right(self):
    """Move steering to the right"""
    (hx, hy) = self.heading
    (hx, hy) = (hy, -hx) # Rotate 90 degrees to right
    self.heading = (hx,hy)

  def speed_up(self):
    """Accelerate"""
    self.speed = self.speed + Robot.speed_increment

  def slow_down(self):
    """Decelerate"""
    if self.speed > 0:
      self.speed = self.speed - Robot.speed_increment

  def get_x(self):
    """Get x coordinate"""
    return self.x

  def get_y(self):
    """Get y coordinate"""
    return self.y

  def increment_time(self):
    """Move the robot by a single time unit"""
    dx, dy = self.heading
    self.x = self.x + dx * self.speed
    self.y = self.y + dy * self.speed

  def crashed(self):
    """Return if the robot has crashed into a wall"""
    if self.sim.world[int(self.y+0.5)][int(self.x+0.5)] == Simulator.empty:
      return False
    else:
      return True
