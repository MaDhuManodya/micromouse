# Micro Mouse

## Overview
This project is a comprehensive implementation for a robot that integrates motor control, encoders, sensors, and an OLED display.

## Features
- **Motor Control:** Uses the SparkFun TB6612 motor driver library to control two motors.
- **Encoders:** Utilizes interrupts to read encoder values for precise motor speed control.
- **IR Sensors:** Reads values from IR sensors to detect obstacles and adjust motor speed.
- **MPU9250:** Integrates an MPU9250 for potential use in balancing or additional navigation control.
- **OLED Display:** Displays real-time data such as encoder values, PID outputs, and sensor readings.

## Components
- **Motors:** Controlled using the SparkFun TB6612 motor driver.
- **Encoders:** Read via interrupts for accurate motor control.
- **IR Sensors:** Detect obstacles and assist in navigation.
- **MPU9250:** Provides additional navigation and balancing capabilities.
- **OLED Display:** Shows real-time system data for monitoring and debugging.

## Setup
1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/robot-control-system.git
    cd robot-control-system
    ```

2. **Install dependencies:**
    - Adafruit_GFX
    - Adafruit_SSD1306
    - SparkFun_TB6612
    - MPU9250

3. **Wiring:**
    - Connect the motors, encoders, IR sensors, MPU9250, and OLED display according to the pin definitions in the code.

4. **Upload the code:**
    - Open the project in your Arduino IDE.
    - Select the appropriate board and port.
    - Upload the code to your microcontroller.

## Usage
- **Motor Control:** The robot uses the SparkFun TB6612 motor driver to control the motors.
- **Encoders:** Encoder values are read using interrupts to maintain precise speed control.
- **IR Sensors:** IR sensors detect obstacles and adjust motor speed to avoid collisions.
- **MPU9250:** Provides additional functionality for balancing and navigation.
- **OLED Display:** Real-time data is displayed on the OLED screen for monitoring and debugging purposes.



![mm2](https://github.com/user-attachments/assets/aeacb368-e462-4903-9786-caa83a383c9f)
![mm1](https://github.com/user-attachments/assets/23ddad3a-62bb-48b0-b401-bb04a89565bc)



https://github.com/user-attachments/assets/59ed93db-5aa2-4418-b381-dc3ea9f1b4f6
