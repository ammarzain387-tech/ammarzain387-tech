import math
import time
from dataclasses import dataclass

@dataclass
class Pose:
    x: float
    y: float
    theta: float  # radians

class DifferentialDriveRobot:
    def __init__(self, wheelbase=0.35, wheel_radius=0.08, max_speed=1.5):
        self.wheelbase = wheelbase
        self.wheel_radius = wheel_radius
        self.max_speed = max_speed
        self.pose = Pose(0.0, 0.0, 0.0)
        self.left_wheel_speed = 0.0
        self.right_wheel_speed = 0.0

    def set_wheel_speeds(self, left, right):
        self.left_wheel_speed = max(-self.max_speed, min(self.max_speed, left))
        self.right_wheel_speed = max(-self.max_speed, min(self.max_speed, right))

    def update(self, dt):
        vl = self.left_wheel_speed
        vr = self.right_wheel_speed

        v = (vl + vr) / 2.0
        w = (vr - vl) / self.wheelbase

        self.pose.x += v * math.cos(self.pose.theta) * dt
        self.pose.y += v * math.sin(self.pose.theta) * dt
        self.pose.theta += w * dt

    def stop(self):
        self.set_wheel_speeds(0.0, 0.0)

    def move_forward(self, speed=1.0):
        self.set_wheel_speeds(speed, speed)

    def turn(self, angular_speed=0.8):
        self.set_wheel_speeds(-angular_speed, angular_speed)

class Sensor:
    def __init__(self, name, angle_offset, max_range=3.0):
        self.name = name
        self.angle_offset = angle_offset
        self.max_range = max_range

    def read_distance(self, robot, obstacles):
        # Compute sensor point in robot coordinates
        sensor_x = math.cos(robot.pose.theta + self.angle_offset)
        sensor_y = math.sin(robot.pose.theta + self.angle_offset)

        x = robot.pose.x + sensor_x * 0.2
        y = robot.pose.y + sensor_y * 0.2

        min_dist = self.max_range
        for obs in obstacles:
            ox, oy, r = obs
            dx = x - ox
            dy = y - oy
            dist = math.hypot(dx, dy)
            if dist < r + 0.15:
                min_dist = min(min_dist, dist - r)
        return min_dist

class RobotController:
    def __init__(self, robot, sensors):
        self.robot = robot
        self.sensors = sensors

    def control_loop(self, obstacles, dt=0.1, duration=20.0):
        start = time.time()
        while time.time() - start < duration:
            distances = {s.name: s.read_distance(self.robot, obstacles) for s in self.sensors}

            left = distances.get("left", 10.0)
            right = distances.get("right", 10.0)
            front = distances.get("front", 10.0)

            if front < 0.7 or left < 0.6 or right < 0.6:
                # Obstacle detected, turn away from the closest side
                if left < right:
                    self.robot.turn(angular_speed=1.0)
                else:
                    self.robot.turn(angular_speed=-1.0)
            else:
                self.robot.move_forward(speed=1.0)

            self.robot.update(dt)
            print(f"Pose: x={self.robot.pose.x:.2f}, y={self.robot.pose.y:.2f}, theta={self.robot.pose.theta:.2f}")
            print(f"Distances: left={left:.2f}, front={front:.2f}, right={right:.2f}")
            time.sleep(dt)

        self.robot.stop()


if __name__ == "__main__":
    robot = DifferentialDriveRobot()
    sensors = [
        Sensor("left", math.radians(45)),
        Sensor("front", 0.0),
        Sensor("right", math.radians(-45)),
    ]

    # Obstacles: (x, y, radius)
    obstacles = [
        (1.2, 0.5, 0.3),
        (2.2, 1.5, 0.35),
        (1.8, -0.8, 0.25),
    ]

    controller = RobotController(robot, sensors)
    controller.control_loop(obstacles, dt=0.1, duration=15.0)
