import pygame
from math import sin, cos, pi
from os import system
from time import sleep
import subprocess

USED_JOYSTICK = 0
MAXIMUM = 255

browser_location = r'C:\Program Files\Nightly\firefox.exe'

def open_video_feed():
    system('"'+browser_location+'"' + r' 192.168.1.1:8080/?action=stream')

def open_putty_connection():
    command = 'plink Arduino'
    sp = subprocess.Popen(command, bufsize=20, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    return sp

# the angles at which the wheels' axes are pointing
axis_angles = [
    pi / 2,
    7 / 6 * pi,
    11 / 6 * pi
]

# using the angles, we determine the direction in which wheels will move when powered
wheel_directions = [(-sin(theta), cos(theta)) for theta in axis_angles]


# function to compute dot-product of two 2-dimensional vectors
def dot(x, y):
    return x[0] * y[0] + x[1] * y[1]


# some definitions for the axis numbers
leftX = 0
leftY = 1
triggers = 2
rightX = 3
rightY = 4

# Define some colors for printing
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)


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

pygame.init()

# open the putty connection, write the maximum
sp = open_putty_connection()
# sleep(1)
# sp.stdin.write((str(MAXIMUM)+' ').encode('ascii'))
# sp.stdin.flush()
rate = 0

# Set the width and height of the screen [width,height]
size = [500, 400]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("Controller Input")

# Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()

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
    movement = (axes[leftX], -axes[leftY])
    rotation = axes[triggers]
    textPrint.print(screen, "Movement: {}".format(movement))
    textPrint.print(screen, "Rotation: {}".format(rotation))

    wheel_velocities = [dot(movement, wheel_direction) + rotation for wheel_direction in wheel_directions]

    textPrint.print(
        screen,
        "Wheel velocities: " + ', '.join('{:f}'.format(wv) for wv in wheel_velocities)
    )

    old_rate = rate
    s = ''
    rate = int(MAXIMUM*abs(rotation) + 0.5)
    if rate != old_rate:
        if rate < 0:
            rate = 0
        elif rate > MAXIMUM:
            rate = MAXIMUM
        s = chr((rate // 16) + ord('a')) + chr((rate % 16) + ord('a'))
        sp.stdin.write(s.encode('ascii'))
        sp.stdin.flush()

    textPrint.print(screen, str(rate) + str(s))

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    # Limit to 20 frames per second
    clock.tick(60)

# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit()
