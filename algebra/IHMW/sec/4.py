import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle, Wedge

fig, ax = plt.subplots(figsize=(7, 7))

circle_center = (1, 4)
circle_radius = 3
circle = Circle(
    circle_center,
    circle_radius,
    facecolor='green',
    edgecolor='black',
    alpha=0.5,
)
ax.add_patch(circle)

wedge_center = (-4, -3)
wedge = Wedge(
    wedge_center,
    r=1000,                        
    theta1=-60,                 
    theta2=60,                  
    facecolor='blue',
    edgecolor='black',
    alpha=0.5,
)
ax.add_patch(wedge)

ax.set_xlim(-20, 20)
ax.set_ylim(-20, 20)

ax.axhline(0, linewidth=0.8)
ax.axvline(0, linewidth=0.8)

ax.grid(True)

plt.show()
