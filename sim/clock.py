import tkinter as tk
import math
import time

# Window setup
WIDTH = 500
HEIGHT = 500
CENTER_X = WIDTH // 2
CENTER_Y = HEIGHT // 2

RADIUS_SECONDS = 200
RADIUS_MINUTES = 160
RADIUS_HOURS = 120

DOT_RADIUS = 6
UPDATE_MS = 1000

# Colors
BG_COLOR = "black"
INACTIVE_COLOR = "#333333"
SEC_COLOR = "red"
MIN_COLOR = "green"
HOUR_COLOR = "blue"

root = tk.Tk()
root.title("Circular Clock")
canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg=BG_COLOR)
canvas.pack()

def polar_to_cartesian(angle_deg, radius):
    angle_rad = math.radians(angle_deg - 90)
    x = CENTER_X + radius * math.cos(angle_rad)
    y = CENTER_Y + radius * math.sin(angle_rad)
    return x, y

def draw_ring(radius, active_index, active_color):
    for i in range(60):
        angle = i * 6
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

def update_clock():
    canvas.delete("all")

    now = time.localtime()
    second = now.tm_sec
    minute = now.tm_min
    hour = (now.tm_hour % 12) * 5 + minute // 12  # map hour to 0–59

    draw_ring(RADIUS_SECONDS, second, SEC_COLOR)
    draw_ring(RADIUS_MINUTES, minute, MIN_COLOR)
    draw_ring(RADIUS_HOURS, hour, HOUR_COLOR)

    root.after(UPDATE_MS, update_clock)

update_clock()
root.mainloop()
