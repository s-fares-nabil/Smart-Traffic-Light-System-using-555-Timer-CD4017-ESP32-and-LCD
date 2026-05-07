# Smart Traffic Light System using 555 Timer, CD4017, ESP32, and LCD

## Short Project Description
This hardware-software synchronized traffic light controller implements traffic light logic using a 555 timer and CD4017 decade counter, with an ESP32 reading signals in real-time to display countdown timing on an I2C LCD display. The system dynamically adjusts timing based on hardware potentiometer adjustments.

## Features
- Hardware-based traffic light sequencing using 555 timer and CD4017
- Real-time signal reading from ESP32 GPIO pins
- Dynamic timing adjustment via potentiometer
- LCD display showing current light phase and countdown timer
- Solved timing instability issues caused by overlapping LED states
- Step-based timing logic implementation for accurate sequencing
- Non-blocking code design using millis() for responsive operation
- Visual feedback through external LEDs and LCD display

## Components
- **NE555 Timer IC** - Generates clock pulses for timing control
- **CD4017 Decade Counter** - Sequences through traffic light states (10-step counter)
- **ESP32 Development Board** - Reads hardware signals and controls LCD display
- **I2C LCD Display (16x2)** - Shows current state and countdown timing
- **LED Indicators** - Red, Yellow, Green traffic lights
- **Potentiometer** - Adjusts timing speed of the 555 timer circuit
- **Resistors & Capacitors** - For 555 timer circuit configuration
- **Breadboard & Jumper Wires** - Prototyping and connections

## Circuit Explanation

### Hardware Timing Circuit
The core timing logic is implemented in hardware:
1. **555 Timer Configured as Astable Multivibrator**:
   - Produces continuous square wave output
   - Frequency controlled by potentiometer (adjusts timing speed)
   - Output feeds into CD4017 clock input

2. **CD4017 Decade Counter**:
   - Sequential activation of output pins (Q0-Q9) on each clock pulse
   - Q0-Q3: Controls Red LED duration (4 steps)
   - Q4: Controls Yellow LED duration (1 step) 
   - Q5-Q9: Controls Green LED duration (5 steps)
   - Q10 triggers reset (pin 15) to restart sequence
   - Note: Final green step (Q9) overlaps with yellow (Q4) for smooth transition

### ESP32 Interface
- Reads LED states directly from hardware circuit:
  - RED signal → GPIO 32 (from LED or tap point)
  - YELLOW signal → GPIO 33 
  - GREEN signal → GPIO 25
- Uses internal pull-down resistors for stable reading
- Implements edge detection to measure phase durations
- Displays real-time information on I2C LCD via Wire library

### LCD Display
- Shows current traffic light state (RED/YELLOW/GREEN)
- Displays countdown timer in seconds for active phase
- Shows "Syncing..." during initial calibration
- Updates every second with remaining time

## Challenges and Solutions

### Challenge 1: Timing Instability from Overlapping States
**Problem**: Simple state detection (reading HIGH/LOW values) caused inaccurate timing measurements when LED states overlapped during transitions.

**Solution**: Implemented edge-triggered timing measurement:
- Detect rising edges (LOW→HIGH transitions) for each signal
- Measure time between rising edges to calculate phase duration
- Store dynamic duration values for each phase
- Only update timing measurements on valid state transitions

### Challenge 2: Invalid State Sequences
**Problem**: Noise or simultaneous transitions could cause invalid state sequences (e.g., Yellow after Green without Red first).

**Solution**: Added state validation logic:
- Track current valid state (0=Syncing, 1=RED, 2=YELLOW, 3=GREEN)
- Only accept YELLOW signal if current state is RED
- Only accept GREEN signal if current state is RED or YELLOW
- Prevents erroneous state changes from electrical noise

### Challenge 3: Initial Synchronization
**Problem**: System needed to measure at least one full cycle before displaying accurate timing.

**Solution**: Implemented synchronization state:
- Start in SYNCING state (currentState = 0)
- Wait until all three phases (RED, YELLOW, GREEN) have been measured at least once
- Display "Syncing Cycles.." and "Reading Pot..." during initialization
- Transition to normal operation after first complete cycle

### Challenge 4: Display Update Timing
**Problem**: Blocking delays would interfere with real-time signal reading.

**Solution**: Non-blocking design using millis():
- All timing based on millis() comparisons
- No delay() calls in main loop
- LCD updates only when timing values change
- Main loop continuously monitors input signals

## Future Improvements
1. **Add Pedestrian Crossing Feature**:
   - Add walk/don't walk signals
   - Implement button-triggered pedestrian phase

2. **Wireless Connectivity**:
   - Add ESP-NOW or WiFi for remote monitoring
   - Enable over-the-air (OTA) updates
   - Connect to traffic management system

3. **Enhanced Display**:
   - Add second LCD for pedestrian timing
   - Include graphical elements or icons
   - Add brightness control based on ambient light

4. **Power Efficiency**:
   - Implement sleep modes during low traffic periods
   - Add solar power option with battery backup
   - Optimize ESP32 power consumption

5. **Advanced Traffic Logic**:
   - Implement adaptive timing based on traffic density
   - Add emergency vehicle preemption
   - Coordinate with adjacent traffic lights

6. **Enclosure and Robustness**:
   - Design weatherproof enclosure
   - Add surge protection and signal filtering
   - Implement fault detection and diagnostics

## Repository Structure
```
Smart-Traffic-Light-System/
├── traffic_light.ino             # Main Arduino/ESP32 sketch
├── README.md                     # This file
├── circuit_diagram/              # Circuit diagrams and schematics
│   ├── schematic.png
│   └── breadboard_layout.png
├── photos/                       # Project photos
│   ├── assembled_circuit.jpg
│   ├── lcd_display.jpg
│   └── hardware_closeup.jpg
├── videos/                       # Demonstration videos
│   ├── demo_traffic_cycle.mp4
│   └── timing_adjustment.mp4
├── documentation/                # Additional documentation
│   ├── component_datasheets/
│   │   ├── NE555_datasheet.pdf
│   │   ├── CD4017_datasheet.pdf
│   │   └── ESP32_datasheet.pdf
│   └── timing_calculation.pdf
└── lib/                          # Any required libraries (if not using Library Manager)
```

## Suggested Media Placement
- **README Badges**: Add shields for Arduino IDE compatibility, license, etc.
- **Circuit Diagrams**: Place in `circuit_diagram/` folder with clear labels
- **Assembly Photos**: Show breadboard/wiring in `photos/` folder
- **Working Demo**: Short video in `videos/` showing traffic light cycling
- **Close-up Shots**: Detailed photos of connections and component placement
- **Timing Adjustment**: Video demonstrating potentiometer effect on speed

## How to Use This Project
1. **Hardware Assembly**:
   - Build the 555 timer astable circuit with potentiometer for speed control
   - Connect CD4017 outputs to LED indicators through current-limiting resistors
   - Tap LED signals for ESP32 input connections
   - Connect I2C LCD to ESP32 (SDA→GPIO21, SCL→GPIO22)

2. **Software Setup**:
   - Install Arduino IDE or PlatformIO
   - Add ESP32 board support if needed
   - Install LiquidCrystal_I2C library via Library Manager
   - Upload `traffic_light.ino` to ESP32

3. **Operation**:
   - Power on the circuit
   - Watch LCD display "Syncing Cycles.." and "Reading Pot..."
   - After first complete cycle, timing information will appear
   - Adjust potentiometer to change traffic light cycle speed
   - Observe LCD showing current state and countdown timer

## Notes for First-Year Engineering Students
- This project demonstrates mixed-signal design (analog timing circuit + digital processing)
- Shows importance of hardware-software synchronization
- Illustrates real-world considerations like switch debouncing (via edge detection)
- Teaches timing measurement techniques using microcontroller timers
- Provides foundation for more complex embedded systems projects

## License
This project is open source for educational purposes. Feel free to modify and adapt for learning.

---
*Smart Traffic Light System - A hardware-software synchronized traffic light controller*
*First-year Engineering Project*
