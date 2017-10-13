import json
import pygame
from os import system
from time import sleep
from math import sin, cos, pi
from string import ascii_lowercase, ascii_uppercase
import serial

# Initialize the joysticks
pygame.init()
pygame.joystick.init()
USED_JOYSTICK = 1

ser = serial.Serial('COM7', 9600)
msg = ' 000000'
ser.write(msg.encode())
camera_dir = [0.0, 0.0]
camera_speed = 1

pickup = 0

FPS = 20
EXPORT_AS_JSON = False
COMPUTE_SPEEDS = True

# maps a float in [-1, 1] to a char in [A, Z] or [a,z] or '.'
def map_to_char(x):
    i = int(x * 26)
    if i > 26: i = 26
    elif i < -26: i = -26
    if i == 0:
        return '0'
    elif i > 0:
        return ascii_uppercase[i-1]
    else:
        return ascii_lowercase[-i-1]

def to_string(data):
    if COMPUTE_SPEEDS:
        # Remove the movement and rotation values
        del data['movement_x']
        del data['movement_y']
        del data['rotation']
    else:
        # Remove the relative wheel speeds
        del data['wheel0']
        del data['wheel1']
        del data['wheel2']
    if EXPORT_AS_JSON:
        return json.dumps(data)
    else:
        return ' ' + ''.join(map_to_char(value) for value in data.values())

for i in range(pygame.joystick.get_count()):
    name = pygame.joystick.Joystick(i).get_name()
    print(name)
    if name == 'Controller (XBOX 360 For Windows)':
        USED_JOYSTICK = i
print('Using joystick {}'.format(i))

browser_location = r'C:\Program Files\Nightly\firefox.exe'

def open_video_feed():
    system('"'+browser_location+'"' + r' 192.168.1.1:8080/?action=stream')

# no longer used method to connect using putty
# def open_putty_connection():
#     command = 'plink "Arduino"'
#     sp = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
#     return sp

# the angles at which the wheels' axes are pointing
axis_angles = [
    pi / 2,
    7 / 6 * pi,
    11 / 6 * pi
]

# using the angles, we determine the direction in which wheels will move when powered
wheel_directions = [(-sin(theta), cos(theta)) for theta in axis_angles]

def clamp(x, bounds):
    if x < bounds[0]:
        return bounds[0]
    elif x > bounds[1]:
        return bounds[1]
    return x


# function to compute dot-product of two vectors
def dot(x, y):
    return sum(a*b for (a, b) in zip(x, y))


# some definitions for the axis numbers
leftX = 0
leftY = 1
triggers = 2
rightX = 3
rightY = 4
CAMERA_RESET = 9

# Define some colors for printing
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

SPARSE = True

# This is a simple class that is used to print to the screen
# It has nothing to do with the joysticks, just outputting the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def print(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10


# Set the width and height of the screen [width,height]
size = [500, 400]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("Controller Input")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Get ready to print
textPrint = TextPrint()

# -------- Main Program Loop -----------
while not done:
    # clear all the events, also check if the close button was clicked
    for event in pygame.event.get():  # User did something
        if event.type == pygame.QUIT:  # If user clicked close
            done = True  # Flag that we are done so we exit this loop
        # most events are ignored

    # drawing step (clear the screen first)
    screen.fill(WHITE)
    textPrint.reset()

    # take the first joystick
    joystick = pygame.joystick.Joystick(USED_JOYSTICK)
    joystick.init()

    # Get the name from the OS for the controller/joystick
    name = joystick.get_name()
    textPrint.print(screen, "Joystick name: {}".format(name))

    axes = [round(joystick.get_axis(i), 2) for i in range(joystick.get_numaxes())]

    # read the buttons of the controller and quit is PAUSE (= 7) was pressed
    buttons = [k for k in range(joystick.get_numbuttons()) if joystick.get_button(k)]
    if 7 in buttons:
        done = True
    textPrint.print(screen, "Buttons pressed: " + ", ".join(str(k) for k in buttons))

    up = 4 in buttons
    down = 5 in buttons

    if up and not down:
        pickup = 135
    elif down and not up:
        pickup = 0

    # read the hat-switch (or d-pad) inputs
    # currently unused, but may be necessary later
    hats = joystick.get_numhats()
    textPrint.print(screen, "Number of hats: {}".format(hats))
    textPrint.indent()

    for i in range(hats):
        hat = joystick.get_hat(i)
        textPrint.print(screen, "Hat {} value: {}".format(i, str(hat)))
    textPrint.unindent()

    # give the analog sticks and triggers, and calculate the wheel velocities
    movement = list(map(
        lambda x: round(10*x)/10,
        [axes[leftX], -axes[leftY]]
    ))
    # movement = (axes[leftX], -axes[leftY])
    rotation = axes[triggers]
    camera_axes = list(map(
        lambda x: round(10*x)/10,
        [axes[rightX], -axes[rightY]]
    ))


    textPrint.print(screen, "Movement: {}".format(movement))
    textPrint.print(screen, "Rotation: {}".format(rotation))
    textPrint.print(screen, "Camera  : {}".format(camera_axes))


    wheel_velocities = [dot(movement, wheel_direction)/2 + rotation for wheel_direction in wheel_directions]
    camera_dir = [clamp(a + b * camera_speed / FPS, (-1, 1)) for a, b in zip(camera_dir, camera_axes)]
    if CAMERA_RESET in buttons:
        camera_dir = [0, 0]

    textPrint.print(
        screen,
        "Wheel velocities: " + ', '.join('{:f}'.format(wv) for wv in wheel_velocities)
    )

    # TODO: complete the dataset with camera and grabbing controls
    data = {
        'wheel0': wheel_velocities[0],
        'wheel1': wheel_velocities[1],
        'wheel2': wheel_velocities[2],
        'movement_x': movement[0],
        'movement_y': movement[1],
        'rotation': rotation,
        'camera0': camera_dir[0],
        'camera1': camera_dir[1],
        'pickup0': pickup
    }
    prevmsg = msg
    msg = to_string(data)
    textPrint.print(screen, msg)
    if msg != prevmsg or not SPARSE:
        ser.write(msg.encode())


    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    clock.tick(FPS)

# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit()
