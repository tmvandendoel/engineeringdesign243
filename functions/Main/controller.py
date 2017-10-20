import serial
import pygame
from os import system
from msvcrt import getch
from math import sin, cos, pi
from string import ascii_lowercase, ascii_uppercase

FPS = 20

# initialise the joysticks
pygame.init()
pygame.joystick.init()
USED_JOYSTICK = 1

# initialise the serial connection and reset all servos
try:
    ser = serial.serial_for_url('socket://192.168.1.1:2001', baudrate=115200, timeout=1/FPS)
except Exception:
    print('Error: no serial device found. Press any key to close.')
    getch()
    exit()

msg = ' 0000000'
while ser.read(1) != b'': pass
ser.write(msg.encode())
camera_dir = [0.0, 0.0]
camera_speed = 1
pickup = 0
lights = 0

# the angles at which the wheels' axes are pointing
axis_angles = [ # 0 = right, pi/2 = forward
    5 / 6 * pi, # left
    1 / 6 * pi, # right
    9 / 6 * pi # back
]

# method to keep a value in a certain interval. Used for message sending
def clamp(x, bounds):
    if x < bounds[0]:
        return bounds[0]
    elif x > bounds[1]:
        return bounds[1]
    return x

# maps a float in [-1, 1] to a char in [A, Z] or [a,z] or '.'
def map_to_char(x):
    i = clamp(int(x * 26), (-26, 26))
    if i == 0:
        return '0'
    elif i > 0:
        return ascii_uppercase[i-1]
    else:
        return ascii_lowercase[-i-1]


# turns the data dictionary into a message for the Arduino
def to_string(data):
    # Remove the movement and rotation values
    return ' ' + ''.join(map_to_char(value) for value in data.values())

# make sure we pick the right controller
# (used as a safeguard, since InputMapper's exclusive mode doesn't always work)
i = -1
for i in range(pygame.joystick.get_count()):
    name = pygame.joystick.Joystick(i).get_name()
    print(name)
    if name == 'Controller (XBOX 360 For Windows)':
        USED_JOYSTICK = i

if i == -1:
    print('Error: no joystick found. Press any key to close')
    getch()
    exit()
print('Using joystick {}'.format(i))

browser_location = r'C:\Program Files\Nightly\firefox.exe'

def open_video_feed():
    system('"'+browser_location+'"' + r' 192.168.1.1:8080/?action=stream')


# using the angles, we determine the direction in which wheels will move when powered
# this is perpendicular to the acis angles
wheel_directions = [(-sin(theta), cos(theta)) for theta in axis_angles]

# function to compute dot-product of two vectors, used for wheel speed calculations
def dot(x, y):
    return sum(a*b for (a, b) in zip(x, y))

# some definitions for the axis numbers and buttons
leftX        = 0
leftY        = 1
triggers     = 2
rightX       = 3
rightY       = 4

CAMERA_RESET = 9
PICKUP_UP    = 4
PICKUP_DOWN  = 5
LIGHTS_ON    = 0
LIGHTS_OFF   = 1

# Define some colors for printing
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# with this flag enabled, we only send a new message when the inputs have changed
# this saves power on the Arduino
SPARSE = True

# this is a simple class that is used to print to the screen
# it has nothing to do with the joysticks, just outputting the
# information
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


# set the width and height of the screen [width,height]
size = [500, 400]
screen = pygame.display.set_mode(size)
pygame.display.set_caption("Controller Input")

# loop until the user clicks the close button or presses [options] on the controller
done = False

# used to manage the screen updating
clock = pygame.time.Clock()

# get ready to print
textPrint = TextPrint()

# -------- Main Program Loop -----------
while not done:
    # clear all the events, also check if the close button was clicked
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        # most events are ignored

    # drawing step (clear the screen first)
    screen.fill(WHITE)
    textPrint.reset()

    # take the first joystick
    joystick = pygame.joystick.Joystick(USED_JOYSTICK)
    joystick.init()

    axes = [round(joystick.get_axis(i), 2) for i in range(joystick.get_numaxes())]

    # read the buttons of the controller and quit is PAUSE (= 7) was pressed
    buttons = [k for k in range(joystick.get_numbuttons()) if joystick.get_button(k)]
    if 7 in buttons:
        done = True
    textPrint.print(screen, "Buttons pressed: " + ", ".join(str(k) for k in buttons))

    up = PICKUP_UP in buttons
    down = PICKUP_DOWN in buttons

    lights_on = LIGHTS_ON in buttons
    lights_off = LIGHTS_OFF in buttons

    if up and not down:
        pickup = 1
    elif down and not up:
        pickup = 0

    if lights_on and not lights_off:
        lights = 1
    elif lights_off and not lights_on:
        lights = 0

    # give the analog sticks and triggers, and calculate the wheel velocities
    movement = list(map(
        lambda x: round(10*x)/10,
        [axes[leftX], -axes[leftY]]
    ))
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

    data = {
        'wheel0' : wheel_velocities[0],
        'wheel1' : wheel_velocities[1],
        'wheel2' : wheel_velocities[2],
        'camera0': camera_dir[0],
        'camera1': camera_dir[1],
        'pickup0': pickup,
        'lights' : lights
    }
    prevmsg = msg
    msg = to_string(data)
    textPrint.print(screen, msg)
    if msg != prevmsg or not SPARSE:
        ser.write(msg.encode())
    k_ = ser.read(1)
    if k_ != b'': print(k_)

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    clock.tick(FPS)

# close the window and quit.
pygame.quit()
