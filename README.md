# CPU_Simulator-

# Simple CPU Emulator

This project is a simple CPU emulator written in C that simulates basic CPU instructions like addition, subtraction, loading, storing data, and more. The program accepts user input to perform a series of predefined instructions. The state of the CPU, including registers and memory, can be saved and loaded from a binary file.

## Features
- **ADD**, **SUB**, **MOV**: Perform arithmetic operations and move values to registers.
- **LOAD**, **STORE**: Load values from memory into registers and store register values in memory.
- **DISC**: Load the state of the CPU from a file.
- **LAYO**: Print the current state of the CPU, including register values and memory content.
- **EXIT**: End the program and clear the CPU state.

## CPU Structure
- **Registers**: The CPU has 6 registers (`R0` to `R5`).
- **Memory**: The CPU uses 256 bytes of memory.
- **WSR (Working Status Register)**: Used to control the CPU’s state (e.g., 1 for running, 0 for stopped).
- **IP (Instruction Pointer)**: Keeps track of the next instruction to be executed.

## Files
- **store.bin**: This file is used to save the state of the CPU (registers and memory) when instructions are executed. It allows for retrieving a previous state using the `DISC` command.

## How to Use
1. **Build and Run**:
   - Compile the program using `gcc`:
     ```
     gcc cpu_emulator.c -o cpu_emulator
     ```
   - Run the program:
     ```
     ./cpu_emulator
     ```

2. **Commands**:
   - After starting the program, you'll be prompted to enter a command (`START` or `EXIT`).
     - Type `START` to begin.
     - Type `EXIT` to terminate the program.

3. **Instruction Input**:
   - Once the CPU is started, you can enter the following instructions:
     - `ADD Rdest, Rsrc1, Rsrc2`: Adds two registers and stores the result in `Rdest`.
     - `SUB Rdest, Rsrc1, Rsrc2`: Subtracts `Rsrc2` from `Rsrc1` and stores the result in `Rdest`.
     - `MOV Rdest, value`: Moves an immediate value into the specified register.
     - `LOAD Rdest, address`: Loads the value from the specified memory address into `Rdest`.
     - `STORE Rsrc, address`: Stores the value from `Rsrc` into the specified memory address.
     - `LAYO`: Displays the current state of the CPU (registers and memory).
     - `DISC n`: Loads the CPU state from the nth previous instruction stored in the file.
     - `EXIT`: Stops the CPU and exits the instruction loop.

4. **Example**:
   ```
   Enter command (START/EXIT): START
   CPU->WSR: 1
   Enter instruction : MOV R0, 5
   Enter instruction : MOV R1, 10
   Enter instruction : ADD R2, R0, R1
   Enter instruction : LAYO
   Enter instruction : EXIT
   ```

   In this example, the program initializes the CPU, moves values into registers, adds them, and displays the CPU state.

## Error Handling
If an error occurs during file operations or memory allocation, the program will terminate and print an appropriate error message using the `perror` function.

## Program Structure
- **CPU Initialization**: Initializes CPU memory, registers, `WSR`, and `IP`.
- **Instruction Execution**: Depending on the command entered by the user, the CPU performs operations on registers or memory.
- **State Management**: CPU states are saved in a binary file after each instruction is executed, allowing for rollback with the `DISC` command.

## Future Improvements
- Implement more complex instructions (e.g., branching, conditional operations).
- Add input validation for robustness.
- Extend memory capacity and register count.
- Implement an interactive assembler to convert assembly instructions into binary code.

## Conclusion
This simple CPU emulator demonstrates basic instruction processing and state management, providing a foundation for more complex CPU emulation projects. The emulator stores its state after each operation and supports restoring previous states, making it versatile for educational purposes and further extension.

