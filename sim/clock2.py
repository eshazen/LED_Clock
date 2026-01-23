import tkinter as tk
import math
import time

# Window setup
WIDTH = 500
HEIGHT = 500
CENTER_X = WIDTH // 2
CENTER_Y = HEIGHT // 2

# Ring radii
RADIUS_ORBIT   = 240   # new rotating outer ring
RADIUS_SECONDS = 200
RADIUS_MINUTES = 160
RADIUS_HOURS   = 120

DOT_RADIUS = 6
UPDATE_MS = 1000

# Colors
BG_COLOR = "black"
INACTIVE_COLOR = "#333333"
SEC_COLOR = "red"
MIN_COLOR = "green"
HOUR_COLOR = "blue"
ORBIT_COLOR = "orange"

root = tk.Tk()
root.title("Circular Clock")
canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg=BG_COLOR)
canvas.pack()

def polar_to_cartesian(angle_deg, radius):
    angle_rad = math.radians(angle_deg - 90)
    x = CENTER_X + radius * math.cos(angle_rad)
    y = CENTER_Y + radius * math.sin(angle_rad)
    return x, y

def draw_ring(radius, active_index=None, active_color=None, angle_offset=0):
    for i in range(60):
        angle = i * 6 + angle_offset
        x, y = polar_to_cartesian(angle, radius)

        if active_index is not None and i == active_index:
            color = active_color
        else:
            color = INACTIVE_COLOR

        canvas.create_oval(
            x - DOT_RADIUS,
            y - DOT_RADIUS,
            x + DOT_RADIUS,
            y + DOT_RADIUS,
            fill=color,
            outline=""
        )

def draw_rotating_ring(radius, angle_offset):
    for i in range(60):
        angle = i * 6 + angle_offset
        x, y = polar_to_cartesian(angle, radius)
        canvas.create_oval(
            x - DOT_RADIUS,
            y - DOT_RADIUS,
            x + DOT_RADIUS,
            y + DOT_RADIUS,
            fill=ORBIT_COLOR,
            outline=""
        )

def update_clock():
    canvas.delete("all")

    now = time.localtime()
    second = now.tm_sec
    minute = now.tm_min
    hour = (now.tm_hour % 12) * 5 + minute // 12

    # Rotating ring: advances 6 degrees per second
    orbit_angle = second * 6

    draw_rotating_ring(RADIUS_ORBIT, orbit_angle)
    draw_ring(RADIUS_SECONDS, second, SEC_COLOR)
    draw_ring(RADIUS_MINUTES, minute, MIN_COLOR)
    draw_ring(RADIUS_HOURS, hour, HOUR_COLOR)

    root.after(UPDATE_MS, update_clock)

update_clock()
root.mainloop()
