import math

class Simulator:
  """Simulator - This class provides a simulation of our robot example"""

  def __init__(self):
    """Initialize time and a single robot"""
    self.time = 0
    self.robot = Robot()

  def get_robot(self):
    """Get a given robot"""
    return self.robot

  def get_time(self):
    """Get current simulation time"""
    return self.time

  def increment_time(self):
    """Increment simulation time"""
    self.time = self.time + 1
    if not self.robot.crashed():
      self.robot.increment_time()


class Robot:
  """Robot - This class provides a robot abstraction"""

  def __init__(self):
    """Initialize robot parameters"""
    #self.time = 0
    self.x = 20
    self.y = 20
    self.speed = 0
    self.heading = 0
    self.wheel_dir = 0
    self.max_wheel_angle = 0.1
    self.wheel_increment = 0.005
    self.speed_increment = 0.05
 
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
    #self.time = self.time + 1
    self.heading = self.heading + self.wheel_dir
    self.x = self.x + math.sin(self.heading) * self.speed
    self.y = self.y + math.cos(self.heading) * self.speed

  def crashed(self):
    """Return if the robot has crashed into a wall"""
    if 10 < self.x < 1190 and 10 < self.y < 790:
	    return False
    else:
	    return True
