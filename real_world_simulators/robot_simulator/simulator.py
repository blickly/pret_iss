import math

class SimpleSimulator:
  def __init__(self, width, height, grid_file="grid.txt"):
    """Initialize time and grid (no robots)"""
    self.time = 0
    self.grid = self.parse_grid_file(grid_file)
    self.time_increment = increment

  def parse_grid_file(self, filename):
    """Parse a text file that contains the maze configuration"""
    e = Simulator.empty
    w = Simulator.wall
    r = Simulator.robot
    grid = []
    for line in open(filename):
      gridrow = []
      for box in line:
        if box == "w":
          gridrow.append(w)
        elif box == " ":
          gridrow.append(e)
      grid.insert(0, gridrow)
    return grid

  def read_grid(self, x, y):
    """Read the contents of a given grid cell"""
    try:
      return self.grid[y][x]
    except:
      return Simulator.wall

  def get_robots(self):
    """Get a list of all the robots"""
    return self.robots

  def get_time(self):
    """Get current simulation time"""
    return self.time
class Simulator:
  """Simulator - This class provides a simulation of our robot example"""
  empty = 0
  wall = 1
  robot = 2

  def __init__(self, width, height, grid_file="grid.txt", time_increment = 1):
    """Initialize time and a single robot"""
    self.grid = self.parse_grid_file(grid_file)
    self.time = 0
    self.width = width
    self.height = height
    self.time_increment = time_increment
    self.robots = [Robot(self, start=(2,2)), RandomRobot(self, start=(12,5))]

  def parse_grid_file(self, filename):
    """Parse a text file that contains the maze configuration"""
    e = Simulator.empty
    w = Simulator.wall
    r = Simulator.robot
    grid = []
    for line in open(filename):
      gridrow = []
      for box in line:
        if box == "w":
          gridrow.append(w)
        elif box == " ":
          gridrow.append(e)
      grid.insert(0, gridrow)
    return grid

  def read_grid(self, x, y):
    """Read the contents of a given grid cell"""
    try:
      return self.grid[y][x]
    except:
      return Simulator.wall

  def get_robots(self):
    """Get a list of all the robots"""
    return self.robots

  def get_time(self):
    """Get current simulation time"""
    return self.time

  def increment_time(self):
    """Increment simulation time"""
    self.time = self.time + self.time_increment
    for robot in self.get_robots():
      if not robot.crashed():
        robot.increment_time()
    if self.caught_opponent():
      print "Caught opponent! (time = %d)" % (self.time)
      exit(0)

  def caught_opponent(self):
    """Return whether the good robot has caught its opponent"""
    good_robot = self.get_robots()[0]
    bad_robot = self.get_robots()[1]

    return (int(good_robot.y + 0.5) == int(bad_robot.y + 0.5) and
            int(good_robot.x + 0.5) == int(bad_robot.x + 0.5))


class Robot:
  """Robot - This class provides a robot abstraction"""
  speed_increment = 1.0 / 1000000
  bump_sensor_sensitivity = 100

  def __init__(self, simulator, start=(1,1), heading=(1, 0)):
    """Initialize robot parameters"""
    self.sim = simulator
    self.x, self.y = start
    self.heading = heading
    self.speed = 0.0
 
  def steer_left(self):
    """Move steering to the left"""
    (hx, hy) = self.heading
    (hx, hy) = (-hy, hx) # Rotate 90 degrees to left
    self.heading = (hx, hy)

  def steer_right(self):
    """Move steering to the right"""
    (hx, hy) = self.heading
    (hx, hy) = (hy, - hx) # Rotate 90 degrees to right
    self.heading = (hx, hy)

  def go(self):
    """Start robot"""
    self.speed = self.speed_increment

  def stop(self):
    """Stop robot"""
    self.speed = 0
    
  def backup(self):
    """Make the robot back up"""
    self.speed = -self.speed_increment

  def speed_up(self):
    """Accelerate"""
    self.speed = self.speed + self.speed_increment

  def slow_down(self):
    """Decelerate"""
    self.speed = self.speed - self.speed_increment

  def get_x(self):
    """Get x coordinate"""
    return self.x

  def get_y(self):
    """Get y coordinate"""
    return self.y

  def get_heading_string(self):
    """Get a character that corresponds to the current heading"""
    if self.heading == (1,0):
      return 'r'
    elif self.heading == (-1,0):
      return 'l'
    elif self.heading == (0,1):
      return 'u'
    elif self.heading == (0,-1):
      return 'd'

  def increment_time(self):
    """Move the robot by a single time unit"""
    dx, dy = self.heading
    self.x = self.x + dx * self.speed
    self.y = self.y + dy * self.speed

  def crashed(self):
    """Return if the robot has crashed into a wall"""
    if self.sim.grid[int(self.y + 0.5)][int(self.x + 0.5)] == Simulator.empty:
      return False
    else:
      return True
  
  def wall_detection(self):
    """Trying to emulate a bump sensor, very hacky way right now,
    just see's if the next step hits a wall"""
    dx, dy = self.heading
    next_x = self.x + dx * self.speed * self.bump_sensor_sensitivity
    next_y = self.y + dy * self.speed * self.bump_sensor_sensitivity
    if self.sim.grid[int(next_y + 0.5)][int(next_x + 0.5)] != Simulator.empty:
        return True
    return False

class RandomRobot(Robot):
  """RandomRobot - This class implements a robots that moves randomly"""
  def __init__(self, simulator, start=(1,1), heading=(1, 0)):
    """Initialize robot parameters"""
    self.sim = simulator
    self.x, self.y = start
    self.heading = heading
    self.speed = Robot.speed_increment / 2.0
 
  def increment_time(self):
    dx, dy = self.heading
    next_x = self.x + dx * self.speed * self.bump_sensor_sensitivity
    next_y = self.y + dy * self.speed * self.bump_sensor_sensitivity
    if self.sim.grid[int(next_y + 0.5)][int(next_x + 0.5)] != Simulator.empty:
      self.steer_left()
    Robot.increment_time(self)
