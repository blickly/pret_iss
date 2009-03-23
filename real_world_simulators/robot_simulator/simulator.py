import math

class Simulator:
  """Simulator - This class provides a simulation of our robot example"""

  def __init__(self, width, height):
    """Initialize time and a single robot"""
    self.time = 0
    self.width = width
    self.height = height
    self.robot = Robot(self)

  def get_robots(self):
    """Get a given robot"""
    yield self.robot

  def get_time(self):
    """Get current simulation time"""
    return self.time

  def increment_time(self):
    """Increment simulation time"""
    if not self.robot.crashed():
      self.time = self.time + 1
      self.robot.increment_time()


class Robot:
  """Robot - This class provides a robot abstraction"""

  def __init__(self, simulator, radius=10, x=20, y=20, heading=0):
    """Initialize robot parameters"""
    self.radius = radius
    self.sim = simulator
    self.x = x
    self.y = y
    self.heading = heading
    self.speed = 0
    self.wheel_dir = 0
    self.max_wheel_angle = 0.02
    self.wheel_increment = 0.001
    self.speed_increment = 0.02
 
  def steer_left(self):
    """Move steering to the left"""
    self.wheel_dir = self.wheel_dir + self.wheel_increment
    if self.wheel_dir > self.max_wheel_angle:
      self.wheel_dir = self.max_wheel_angle

  def steer_right(self):
    """Move steering to the right"""
    self.wheel_dir = self.wheel_dir - self.wheel_increment
    if self.wheel_dir < -self.max_wheel_angle:
      self.wheel_dir = -self.max_wheel_angle

  def speed_up(self):
    """Accelerate"""
    self.speed = self.speed + self.speed_increment

  def slow_down(self):
    """Decelerate"""
    if self.speed > 0:
      self.speed = self.speed - self.speed_increment

  def get_x(self):
    """Get x coordinate"""
    return self.x

  def get_y(self):
    """Get y coordinate"""
    return self.y

  def increment_time(self):
    """Move the robot by a single time unit"""
    self.heading = self.heading + self.wheel_dir
    self.x = self.x + math.cos(self.heading) * self.speed
    self.y = self.y + math.sin(self.heading) * self.speed

  def crashed(self):
    """Return if the robot has crashed into a wall"""
    if (self.radius < self.x < self.sim.width-self.radius and
        self.radius < self.y < self.sim.height-self.radius):
      return False
    else:
      return True
