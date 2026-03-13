# 🐭 Mouse Runner: Autonomous Maze-Solving Robot 

🏆 **Awarded 2nd Place at University Competition**

## 📌 Project Overview
Mouse Runner is an autonomous, Arduino-based robot designed to navigate complex mazes, follow paths, and avoid obstacles in real-time. This repository contains the control software for the robot, featuring both a multi-sensor maze-solving algorithm and a fallback dynamic search algorithm for line-following.

## ⚙️ Hardware & Pin Configuration
The robot is built using standard micro-controller components and motor drivers:
* **Motor Driver (e.g., L298N):** * Right Motors: `IN1 (Pin 2)`, `IN2 (Pin 3)`, `ENA (Pin 6 - PWM)`
  * Left Motors: `IN3 (Pin 4)`, `IN4 (Pin 5)`, `ENB (Pin 11 - PWM)`
* **IR Sensors (Line Detection):**
  * Left IR: `Pin 8`
  * Center IR: `Pin 9`
  * Right IR: `Pin 10`
* **Ultrasonic Sensor (Obstacle Detection):**
  * Trigger: `Pin A1`
  * Echo: `Pin A2`

## 🧠 Software & Logic

This repository includes two distinct operational modes/scripts:

### 1. Main Maze Solver (`maze_robot.ino`)
This script acts as the primary brain of the robot, combining line following, maze solving, and obstacle avoidance. 
* **Priority-Based Navigation:** The robot makes decisions based on a strict hierarchy: **Obstacle > Left > Straight > Right**.
* **Obstacle Avoidance:** Continuously polls the ultrasonic sensor. If an obstacle is detected within the stopping distance (15 units), it halts immediately, overriding all other movement logic.
* **Tunable Parameters:** Features easily adjustable variables for forward speed (`100`), turn speed (`130`), stopping distance, and turn delays (`250ms`).

### 2. Single-Sensor Line Follower (`1.ino`)
A highly resilient fallback script that allows the robot to track a black line using only a single IR sensor.
* **Dynamic Recovery Algorithm:** If the robot loses the line, it doesn't just stop. It initiates an alternating "zig-zag" search algorithm.
* **Expanding Search Radius:** The robot spins left and right, incrementally expanding its search time (by `250ms` each swap) until it re-acquires the path, preventing it from getting permanently stuck.

## 🚀 How to Use
1. Clone this repository.
2. Open either `maze_robot.ino` or `1.ino` in the Arduino IDE.
3. Select your Arduino board and COM port.
4. Verify and Upload the code to your micro-controller.
5. Adjust the `speedForward`, `speedTurn`, and `turnDelay` variables at the top of the scripts depending on your battery voltage and motor torque.
