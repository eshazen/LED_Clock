import tkinter as tk
import math
import time

# Window setup
WIDTH = 500
HEIGHT = 500
CENTER_X = WIDTH // 2
CENTER_Y = HEIGHT // 2

# Ring radii
RADIUS_FAST    = 240
RADIUS_SECONDS = 200
RADIUS_MINUTES = 160
RADIUS_HOURS   = 120

DOT_RADIUS = 6
UPDATE_MS = int(1000 / 12)

# Colors
BG_COLOR = "black"
INACTIVE_COLOR = "#333333"
FAST_COLOR = "orange"
SEC_COLOR = "red"
MIN_COLOR = "green"
HOUR_COLOR = "blue"

FAST_SPOTS = 12

root = tk.Tk()
root.title("Circular Clock")
canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg=BG_COLOR)
canvas.pack()

fast_index = 0

def polar_to_cartesian(angle_deg, radius):
    angle_rad = math.radians(angle_deg - 90)
    x = CENTER_X + radius * math.cos(angle_rad)
    y = CENTER_Y + radius * math.sin(angle_rad)
    return x, y

def draw_ring(radius, active_index, active_color, spots):
    for i in range(spots):
        angle = i * (360 / spots)
        x, y = polar_to_cartesian(angle, radius)
        color = active_color if i == active_index else INACTIVE_COLOR
        canvas.create_oval(
            x - DOT_RADIUS,
            y - DOT_RADIUS,
            x + DOT_RADIUS,
            y + DOT_RADIUS,
            fill=color,
            outline=""
        )

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

def draw_hour_ring(active_index):
    RECT_W = 10
    RECT_H = 20

    for i in range(60):
        angle = i * 6
        x, y = polar_to_cartesian(angle, RADIUS_HOURS)
        color = HOUR_COLOR if i == active_index else INACTIVE_COLOR
        draw_rotated_rect(x, y, RECT_W, RECT_H, angle, color)

def update():
    global fast_index
    canvas.delete("all")

    now = time.localtime()
    second = now.tm_sec
    minute = now.tm_min
    hour = (now.tm_hour % 12) * 5 + minute // 12

    draw_ring(RADIUS_FAST, fast_index, FAST_COLOR, FAST_SPOTS)
    draw_ring(RADIUS_SECONDS, second, SEC_COLOR, 60)
    draw_ring(RADIUS_MINUTES, minute, MIN_COLOR, 60)
    draw_hour_ring(hour)

    fast_index = (fast_index + 1) % FAST_SPOTS
    root.after(UPDATE_MS, update)

update()
root.mainloop()
