# Billiard Simulation with SFML version 2.6.1 (32-bit)

This project is a 2D billiards simulation game built using C++ and the Simple and Fast Multimedia Library (SFML). It demonstrates physics-based interactions between balls, including collisions, friction, and restitution, along with cue stick control and table boundaries.

## Project Structure

The project files are organized as follows:

- **`main.cpp`**: Contains the main game loop that initializes and runs the game.
- **`game.h`** and **`game.cpp`**: Define the main classes for game mechanics, including `Game`, `Ball`, `CueStick`, `Table`, `Hole`, and various utility structs.
- **`*.dll` Files**: Required SFML dynamic libraries.

## Key Classes and Components

### 1. `Game` Class

- **Purpose**: Acts as the main engine, managing the game loop, event handling, updating objects, and rendering graphics.
- **Methods**:
  - `initVariables()`: Initializes essential variables, including player scores and turn.
  - `initWindow()`: Creates the game window with specified dimensions and frame rate limit.
  - `initBalls()`: Generates initial positions and colors of billiard balls, including cue, solid, striped, and black balls.
  - `initHoles()`: Initializes the positions of the holes on the table.
  - `initSoundEffects()`: Loads sound effects for ball collisions and cue stick hits.
  - `initFontText()`: Sets up the font and text for displaying scores and turns.
  - `pollEvents()`: Handles user input and event processing, including mouse interactions with the cue ball and cue stick.
  - `update()`: Updates the state of game objects every frame, including ball movements, collisions, and scoring.
  - `render()`: Renders all game objects to the window, including balls, holes, and UI elements.
  - `areBallsMoving()`: Checks if any ball is still in motion.

### 2. `Ball` Class

- **Purpose**: Represents each billiard ball with properties like position, color, and velocity.
- **Methods**:
  - `applyForce()`: Applies force to a ball, setting it in motion.
  - `checkCollision()`: Checks for collisions with other balls.
  - `resolveCollision()`: Handles collision response with another ball, applying physics calculations to adjust velocity based on normal and tangential components.
  - `checkCollisionWithTrapezium()`: Checks for collisions with trapezium-shaped table walls.
  - `resolveCollisionWithTrapezium()`: Resolves collisions with trapezium-shaped table walls.
  - `update()`: Updates position based on velocity, applies friction, and checks for wall collisions.

### 3. `CueStick` Class

- **Purpose**: Allows the player to control the cue stick and strike the cue ball.
- **Methods**:
  - `startDragging()`: Initiates the aiming process when the player holds down the left mouse button.
  - `update()`: Adjusts the cue stick's position and rotation based on the mouse position, updating shot power and direction.
  - `stopDragging()`: Releases the cue stick, applying the calculated force to the cue ball.
  - `draw()`: Renders the cue stick on the screen.

### 4. `Table` Class

- **Purpose**: Renders the billiard table, including walls and shadows.
- **Methods**:
  - `draw()`: Draws the table and its components (walls, shadows, corners) on the screen.

### 5. `Hole` Class

- **Purpose**: Represents the holes on the billiard table where balls can fall in.
- **Methods**:
  - `draw()`: Draws the hole on the screen.
  - `isBallInHole()`: Checks if a ball is within the hole's radius.

### 6. `References`, `SizeRef`, `ColorRef`, and `SizePositionRef` Structs

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
$ bash -c 'g++ -g $(cygpath -u "path_to_workspaceFolder\main.cpp") $(cygpath -u "path_to_workspaceFolder\game.cpp") -o $(cygpath -u "path_to_workspaceFolder\app.exe") -I $(cygpath -u "path_to_sfml/include") -L $(cygpath -u "path_to_sfml/lib") -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio'
```

## Recent Updates

- **Ball Class Enhancements**: Improved collision detection and response with more accurate physics calculations.
- **CueStick Mechanics**: Enhanced cue stick control with better feedback and power scaling.
- **UI Improvements**: Updated score and turn display for better clarity.
- **Sound Effects**: Added sound effects for ball collisions and cue stick hits to enhance the gaming experience.
- **Trapezium Wall Collision**: Added logic handle for ball to wall collision to enhance the pyshics game works.