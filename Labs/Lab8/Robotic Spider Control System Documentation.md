# Robotic Spider Control System Documentation

## Overview

This lab involves creating a software system to control a robotic spider using memory-mapped I/O (MMIO) on a DE10-Nano SoC board. The codebase includes classes to interface with hardware components, manage motor operations, and coordinate movements of the spider robot.

## File Structure

- [`Main.cpp`](Main.cpp): The entry point of the application, initializing the spider and handling user commands.
- [`Spider.cpp`](Spider.cpp): Defines the [`Spider`](Spider) class, orchestrating the movements of the robot by controlling its legs.
- [`SpiderLeg.cpp`](SpiderLeg.cpp): Implements the [`SpiderLeg`](SpiderLeg.cpp) class, representing a single leg composed of multiple joints.
- [`ServoMotor.cpp`](ServoMotor.cpp): Contains the [`ServoMotor`](ServoMotor.cpp) class, managing individual servo motors via MMIO.
- [`MMap.cpp`](MMap.cpp) and [`MMap.h`](MMap.h): Define the [`MMap`](MMap.h) class for handling memory mapping of device registers.
- [`hps_0.h`](hps_0.h): Provides hardware-specific definitions required for MMIO.
- [`Makefile`](Makefile): Contains build instructions for compiling the project.

## Classes and Functionality

### [`MMap`](MMap.h) Class (MMap.h)

Manages memory-mapped I/O operations:

- **Methods:**
  - [`map(addr_base, addr_span)`](MMap.cpp): Establishes a memory mapping to the specified physical address range.
  - [`unmap()`](MMap.cpp): Releases the existing memory mapping.
  - [`isMapped()`](MMap.h): Checks if the mapping has been established.
  - [`getMotorStart(motorId)`](MMap.h): Computes the virtual address of a motor's first register.
  - [`Motor_Reg32_Write(motorId, regOffset, value)`](MMap.h): Writes a 32-bit value to a motor's register.
  - [`Motor_Reg32_Read(motorId, regOffset)`](MMap.h): Reads a 32-bit value from a motor's register.

### [`ServoMotor`](ServoMotor.cpp) Class (ServoMotor.cpp)

Controls individual servo motors:

- **Attributes:**
  - [`m_fAngle`](ServoMotor.cpp): Current angle of the servo motor.
  - [`m_speed`](ServoMotor.cpp): Operating speed of the servo motor.
  - [`m_nMotorID`](ServoMotor.cpp): Motor ID corresponding to the hardware PWM index.
  - [`_mmio`](ServoMotor.cpp): Pointer to an [`MMap`](MMap.h) instance for MMIO operations.

- **Methods:**
  - [`ServoMotor(mmio, motorId)`](ServoMotor.cpp): Constructor initializing the motor and setting default PWM values.
  - [`Move(fAngle)`](ServoMotor.cpp): Moves the servo to a specified angle, updating the PWM duty cycle.
  - `SetSpeed(speed)`: Sets the operating speed by updating the PWM delay.
  - [`IsReady()`](ServoMotor.cpp): Checks if the servo has completed its last movement.
  - [`Reset()`](ServoMotor.cpp): Resets the servo to its default position.
  - [`speedToDelay(s)`](ServoMotor.cpp): Converts a speed value to an appropriate delay in clock cycles.

### [`SpiderLeg`](SpiderLeg.cpp) Class (SpiderLeg.cpp)

Represents a leg with three joints (hip, knee, ankle):

- **Attributes:**
  - [`m_szMotor`](SpiderLeg.cpp): Array of [`ServoMotor`](ServoMotor.cpp) instances for each joint.
  - [`m_reverse`](SpiderLeg.cpp): Indicates if angle interpretations should be reversed.

- **Methods:**
  - [`SpiderLeg(mmio, Joint0_MotorID, Joint1_MotorID, Joint2_MotorID, reverse)`](SpiderLeg.cpp): Constructor initializing motors.
  - `MoveJoint(JointID, fAngle)`: Moves a specific joint to a given angle.
  - [`IsReady()`](ServoMotor.cpp): Checks if all joints have completed movements.
  - [`Reset()`](ServoMotor.cpp): Resets all joints to default positions.
  - [`GetfAngle(JointID)`](ServoMotor.cpp): Gets the current angle of a specific joint.

### [`Spider`](Spider) Class (Spider.cpp)

Controls the entire spider robot:

- **Attributes:**
  - [`m_szLeg`](Spider.cpp): Array of [`SpiderLeg`](SpiderLeg.cpp) instances for each leg.
  - [`lastStep`](Spider.cpp), [`lastDir`](Spider.cpp): Track the last movement step and direction.
  - [`_mmio`](ServoMotor.cpp): Pointer to an [`MMap`](MMap.h) instance.

- **Methods:**
  - [`Spider()`](Spider.cpp): Constructor initializing legs and MMIO interface.
  - `Init()`: Initializes the spider's legs to default positions.
  - `Standup()`: Moves the spider to a standing position.
  - `MoveForward()`: Coordinates legs for forward movement.
  - [`MoveTripod(TripodID, JointID, AngleF, AngleM, AngleB)`](Spider.cpp): Moves a set of legs simultaneously.
  - [`IsReady()`](ServoMotor.cpp): Checks if all legs have completed movements.
  - [`WaitReady()`](Spider.cpp): Waits until all movements are complete.
  - [`Reset()`](ServoMotor.cpp): Resets the spider to its initial position.

## Constants and Definitions

- **Servo Limits:**
  - [`DEGREE_MIN`](ServoMotor.cpp): -90 degrees
  - [`DEGREE_MAX`](ServoMotor.cpp): 90 degrees
  - [`SPEED_MIN`](ServoMotor.cpp): 0
  - [`SPEED_MAX`](ServoMotor.cpp): 100

- **PWM Settings:**
  - [`FREQ`](ServoMotor.cpp): Clock frequency (50 MHz)
  - [`T_20MS`](ServoMotor.cpp): PWM period for a 20 ms cycle
  - [`PWM_MIN`](ServoMotor.cpp): Corresponds to -90 degrees
  - [`PWM_MAX`](ServoMotor.cpp): Corresponds to 90 degrees

- **Registers Offsets:**
  - [`PWM_PERIOD`](ServoMotor.cpp): Offset for PWM period register
  - [`PWM_DC`](ServoMotor.cpp): Offset for duty cycle register
  - [`PWM_DELAY`](ServoMotor.cpp): Offset for delay register
  - [`PWM_READY`](ServoMotor.cpp): Offset to check if the servo is ready
  - [`PWM_ABORT`](ServoMotor.cpp): Offset to abort operations

## Building and Running

### Build Instructions

Use the provided [`Makefile`](Makefile) to compile the project:

```sh
make
```
### Execution

Run the compiled executable:

```sh
./spider
```

Follow the on-screen prompts to control the spider:

### Commands:
- `f`: Move forward
- `s`: Stop the application

### Lab Objectives
- **Understanding MMIO:** Learn how to interface with hardware registers in C++ using memory-mapped I/O.
- **Servo Motor Control:** Implement control logic for servo motors using PWM signals.
- **Robotics Programming:** Coordinate multiple servos to perform complex movements.
- **Modular Design:** Develop a modular codebase that can be extended for various robotic applications.

### Notes
- **Hardware Configuration:** Ensure motor IDs and hardware connections match the definitions in the code.
- **Parameter Adjustments:** Modify constants like `DELAY_MIN` and `DELAY_MAX` based on your pre-lab calculations and hardware specifications.
- **Safety Precautions:** Always test movements carefully to prevent damage to the hardware.

### Summary
This lab provides hands-on experience with low-level hardware interfacing and robotics control. By understanding and manipulating MMIO, PWM signals, and servo mechanics, you gain valuable insights into embedded systems programming and robotic kinematics.