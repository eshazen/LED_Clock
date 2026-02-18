import tkinter as tk
import math
import time
import sys

# Window setup
WIDTH = 1000
HEIGHT = 1000
CENTER_X = WIDTH // 2
CENTER_Y = HEIGHT // 2

# Ring radii ~ mm
RADIUS_FAST    = 114*4     # fast outer ring
RADIUS_12      = 107.5*4   # hours dots
RADIUS_5       = 99.5*4
RADIUS_4       = 90*4
RADIUS_3       = 76*4
RADIUS_2       = 63.5*4
RADIUS_1       = 50*4

# RADIUS_SECONDS = 200
# RADIUS_MINUTES = 160
# RADIUS_HOURS   = 120

DOT_RADIUS = 6
D5_RADIUS = 10
UPDATE_MS = 1000 # ~16.7 ms (60 Hz)

# Colors
BG_COLOR = "black"
INACTIVE_COLOR = "#333333"
SEC_COLOR = "white"
MIN_COLOR = "#00FF00"
HOUR_COLOR = "#2040FF"

root = tk.Tk()
root.title("Circular Clock")
canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg=BG_COLOR)
canvas.pack()

auto = 1
hour = 0
minute = 0
second = 0

if len(sys.argv) > 3:
    hour = int(sys.argv[1])
    minute = int(sys.argv[2])
    second = int(sys.argv[3])
    auto = 0
    


def polar_to_cartesian(angle_deg, radius):
    angle_rad = math.radians(angle_deg - 90)
    x = CENTER_X + radius * math.cos(angle_rad)
    y = CENTER_Y + radius * math.sin(angle_rad)
    return x, y

def draw_rotated_rect(cx, cy, w, h, angle_deg, color):
    angle = math.radians(angle_deg)
    cos_a = math.cos(angle)
    sin_a = math.sin(angle)

    # Rectangle corners centered at origin
    corners = [
        (-w/2, -h/2),
        ( w/2, -h/2),
        ( w/2,  h/2),
        (-w/2,  h/2),
    ]

    points = []
    for x, y in corners:
        rx = x * cos_a - y * sin_a + cx
        ry = x * sin_a + y * cos_a + cy
        points.extend([rx, ry])

    canvas.create_polygon(points, fill=color, outline="")

# draw ring with 12 LEDs
def draw_12ring(radius, active_index, active_color):
    for i in range(12):
        angle = i * 30
        x, y = polar_to_cartesian(angle, radius)
        color = active_color
        canvas.create_oval(
            x - D5_RADIUS,
            y - D5_RADIUS,
            x + D5_RADIUS,
            y + D5_RADIUS,
            fill=color,
            outline=""
        )

# light up LED at index and +/- width
def draw_rect_ring(radius, index, width, active_color):
    RECT_W = 10
    RECT_H = 20

    for i in range(60):
        angle = i * 6
        x, y = polar_to_cartesian(angle, radius)
        color = INACTIVE_COLOR
        if i == index:
            color = active_color
        if width and (  (((i-1)%60) == index) or (((i+1)%60) == index)):
            color = active_color
        if (width == 2) and (  (((i-2)%60) == index) or (((i+2)%60) == index)):
            color = active_color
        draw_rotated_rect(x, y, RECT_W, RECT_H, angle, color)

# draw ring with 60 LEDs
def draw_ring(radius, active_index, active_color, inactive_color):
    for i in range(60):
        angle = i * 6
        x, y = polar_to_cartesian(angle, radius)
        color = active_color if i == active_index else inactive_color
        canvas.create_oval(
            x - DOT_RADIUS,
            y - DOT_RADIUS,
            x + DOT_RADIUS,
            y + DOT_RADIUS,
            fill=color,
            outline=""
        )

def update():
    global second, minute, hour

    canvas.delete("all")

    if auto:
        now = time.localtime()
        second = now.tm_sec
        minute = now.tm_min
        hour = (now.tm_hour % 12) * 5 + minute // 12
    else:
        second = second + 1
        if second > 59:
            second = 0
            minute = minute + 1
            if minute > 59:
                minute = 0
                hour = hour + 1
        hour = (hour % 12) * 5 + minute // 12

    # Draw rings
    draw_ring(RADIUS_FAST, second, INACTIVE_COLOR, SEC_COLOR)

    draw_12ring( RADIUS_12, 0, "#FF0000")

    # draw_rect_ring(RADIUS_4, minute, 1, MIN_COLOR)
    draw_rect_ring(RADIUS_5, minute, 0, MIN_COLOR)

#    draw_rect_ring(RADIUS_1, hour, 2, HOUR_COLOR)
#    draw_rect_ring(RADIUS_2, hour, 1, HOUR_COLOR)
    draw_rect_ring(RADIUS_3, hour, 0, HOUR_COLOR)

    root.after(UPDATE_MS, update)

update()
root.mainloop()
