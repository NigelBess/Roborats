BumpAvoid4.ino is arduino code for a four-wheeled robot with
two bumpers (one on each side in the front) that activate 
limit switches.

Using the state of these limit switches, the robot detects 
collisions and avoids obstacles. It also identifies if it is
stuck in a corner and navigates away from the corner.

The robot has two motors, one powering the left front wheel,
and the other powering the right front wheel. It turns by 
moving its driven wheels at different velocities.