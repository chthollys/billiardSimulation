# Billiard Simulation with SFML version 2.6.1 (32-bit)

This project is a 2D billiards simulation game built using C++ and the Simple and Fast Multimedia Library (SFML). It demonstrates physics-based interactions between balls, including collisions, friction, and restitution, along with cue stick control and table boundaries.

## Project Structure

The project files are organized as follows:

- **`main.cpp`**: Contains the main game loop that initializes and runs the game.
- **`game.h`** and **`game.cpp`**: Define the main classes for game mechanics, including `Game`, `Ball`, `CueStick`, and `Table`.
- **`*.dll` Files**: Required SFML dynamic libraries.

## Key Classes and Components

### 1. `Game` Class

- **Purpose**: Acts as the main engine, managing the game loop, event handling, updating objects, and rendering graphics.
- **Methods**:
  - `initVariables()`: Initializes essential variables.
  - `initWindow()`: Creates the game window.
  - `initBalls()`: Generates initial positions and colors of billiard balls.
  - `pollEvents()`: Handles user input and event processing.
  - `update()`: Updates the state of game objects every frame.
  - `render()`: Renders all game objects to the window.
  - `areBallsMoving()`: Checks if any ball is still in motion.

### 2. `Ball` Class

- **Purpose**: Represents each billiard ball with properties like position, color, and velocity.
- **Methods**:
  - `applyForce()`: Applies force to a ball, setting it in motion.
  - `checkCollision()`: Checks for collisions with other balls.
  - `resolveCollision()`: Handles collision response with another ball, applying physics calculations to adjust velocity based on normal and tangential components.
  - `update()`: Updates position based on velocity, applies friction, and checks for wall collisions.

### 3. `CueStick` Class

- **Purpose**: Allows the player to control the cue stick and strike the cue ball.
- **Methods**:
  - `startDragging()`: Initiates the aiming process when the player holds down the left mouse button.
  - `update()`: Adjusts the cue stick's position and rotation based on the mouse position, updating shot power and direction.
  - `stopDragging()`: Releases the cue stick, applying the calculated force to the cue ball.
  
### 4. `Table` Class

- **Purpose**: Renders the billiard table, including walls and shadows.
- **Methods**:
  - `draw()`: Draws the table and its components (walls, shadows, corners) on the screen.

### 5. `References`, `SizeRef`, `ColorRef`, and `SizePositionRef` Structs

These structs store game constants, including:
- **SizeRef**: Stores the dimensions for the window, table, balls, and cue stick.
- **ColorRef**: Contains color definitions for balls and table elements.
- **SizePositionRef**: Calculates wall and corner positions based on the table size.
- **References**: Inherits from the above structs and includes position generation functions.

## Physics

The billiards simulation uses basic physics concepts:
- **Collision Resolution**: The `resolveCollision` method in `Ball` uses the normal and tangential components of velocity to simulate realistic collisions.
- **Friction and Restitution**: Friction is applied to slow down balls gradually, while restitution controls the elasticity of collisions, reducing velocity slightly on each collision to simulate energy loss.
- **Cue Stick Mechanics**: The cue stick's drag length determines the power and direction of the force applied to the cue ball.

## Compilation and Execution

### Prerequisites

- **SFML**: Make sure SFML is installed on your system, and its include and library paths are accessible.

### Compiling the Project

To compile the project, use the following command, adjusting the paths to SFML libraries if needed:

```bash
g++ main.cpp game.cpp -o app -I"path_to_sfml/include" -L"path_to_sfml/lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
