#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>


struct SizeRef {
    // Window Dimension
    const float window_width = 2500.0f;
    const float window_height = 1500.0f;
    const int frLimit = 60;

    // Table Properties
    const float table_width = 2000.f;
    const float table_height = 1200.0f;
    sf::Vector2f table_dimension = sf::Vector2f(table_width, table_height);
    // Offset Property
    const float table_offsetX = (window_width - table_width) / 2;
    const float table_offsetY = (window_height - table_height) / 2;
    sf::Vector2f table_offset = sf::Vector2f(table_offsetX, table_offsetY);


    const float table_topBottomWall_width = table_width + 120.0f; // added 120.0f corresponding to the 2 times height
    const float table_topBottomWall_height = 60.0f;
    const float table_topBottomWallShadow_width = table_topBottomWall_width;
    const float table_topBottomWallShadow_height = table_topBottomWall_height;

    const float table_leftRightWall_width = 60.0f;
    const float table_leftRightWall_height = table_height;
    const float table_leftRightWallShadow_width = table_leftRightWall_width;
    const float table_leftRightWallShadow_height = table_leftRightWall_height;

    const float cornerRadius = 60.0f;
    const float shadowOffset = 15.0f;

    // Hole Properties
    const int holeCount = 6;
    const float hole_radius = 50.0f;

    // Ball Properties
    const int ballCount = 16;
    const float ball_border_width = 2.0f;
    const float ball_radius = 25.0f;

    // CueStick Properties
    const float stick_width = 5.0f;
    const float stick_length = 200.0f;
    const float minOffsetDistance = 40.0f; // Minimum distance from cue ball
    const float maxOffsetDistance = 150.0f; // Maximum distance based on drag

    // Pyhsics Properties
    const float force_scaling_factor = 5.0f;
    const float phi = 3.14159f;
    const float friction = 0.98f;
    const float minVelocityThreshold = 0.05f;
    const float restitution = 0.95f; // Coefficient of restitution (1.0 = elastic, 0.0 = inelastic)
};

struct ColorRef {
    // Ball Color Properties
    sf::Color ballBorderColor = sf::Color(255, 255, 255);
    std::vector<sf::Color> ballColors = {
        sf::Color(255, 215, 0),       // Yellow (Ball 1)
        sf::Color(255, 0, 0),         // Red (Ball 2)
        sf::Color(0, 0, 255),         // Blue (Ball 3)
        sf::Color(255, 69, 0),        // Orange (Ball 4)
        sf::Color(128, 0, 128),       // Purple (Ball 6)
        sf::Color(0, 128, 0),         // Green (Ball 5)
        sf::Color(139, 69, 19),       // Brown (Ball 8)
        sf::Color(255, 192, 203),     // Pink (Ball 9)
        sf::Color(255, 20, 147),      // Deep Pink (Ball 7)
        sf::Color(0, 255, 255),       // Cyan (Ball 10)
        sf::Color(255, 165, 0),       // Dark Orange (Ball 12)
        sf::Color(0, 100, 0),         // Dark Green (Ball 14)
        sf::Color(75, 0, 130),        // Indigo (Ball 11)
        sf::Color(128, 128, 0),       // Olive (Ball 15)
        sf::Color(86, 86, 86),        // Gray (Ball 13)
        sf::Color(255, 255, 255)      // White (Cue Ball)
    };

    // Table Color Properties
    sf::Color tableColor = sf::Color(0, 204, 0); // Bright green
    sf::Color tableWallColor= sf::Color(139, 69, 19); // Dark wood color
    sf::Color tableShadowColor = sf::Color(105, 58, 26);

    // Hole Color Properties
    sf::Color holeColor = sf::Color(0, 0, 0);
};

struct SizePositionRef : public SizeRef {
    
    // Ball Positions
    // std::vector<sf::Vector2f> ballPositions = generate(ballCount, ball_radius, table_dimension, table_offset);
    // Table Position Properties
    // const float shadowOffset = 15.0f;
    const sf::Vector2f table_topWall = sf::Vector2f(window_width / 2, (window_height - table_height - table_topBottomWall_height) / 2);
    const sf::Vector2f table_topWallShadow = sf::Vector2f(table_topWall.x, table_topWall.y);
    
    const sf::Vector2f table_bottomWall = sf::Vector2f(window_width / 2, (window_height + table_height + table_topBottomWall_height) / 2);
    const sf::Vector2f table_bottomWallShadow = sf::Vector2f(table_bottomWall.x, table_bottomWall.y);

    const sf::Vector2f table_leftWall = sf::Vector2f((window_width - table_width - table_leftRightWall_width) / 2, window_height / 2);
    const sf::Vector2f table_leftWallShadow = sf::Vector2f(table_leftWall.x, table_leftWall.y);

    const sf::Vector2f table_rightWall = sf::Vector2f((window_width + table_width + table_leftRightWall_width) / 2, window_height / 2);
    const sf::Vector2f table_rightWallShadow = sf::Vector2f(table_rightWall.x, table_rightWall.y);

    // Window : 3000 x 1800 // Table : 2000 x 1200
    // Window : 800 x 400 // Table : 700 x 300

    const sf::Vector2f topLeftCornerPosition = sf::Vector2f(table_topWall.x - table_width - cornerRadius, table_topWall.y - cornerRadius);
    const sf::Vector2f topRightCornerPosition = sf::Vector2f(table_topWall.x + table_width + cornerRadius, table_topWall.y - cornerRadius);
    const sf::Vector2f bottomLeftCornerPosition = sf::Vector2f(table_topWall.x - table_width - cornerRadius, table_bottomWall.y + cornerRadius);
    const sf::Vector2f bottomRightCornerPosition = sf::Vector2f(table_topWall.x + table_width + cornerRadius, table_bottomWall.y + cornerRadius);

    const sf::Vector2f hole_topLeft = table_offset;
    const sf::Vector2f hole_bottomLeft = sf::Vector2f(table_offsetX, table_offsetY + table_height);
    const sf::Vector2f hole_topMid = sf::Vector2f(window_width / 2, (window_height - table_height - table_topBottomWall_height * 0.5) / 2);
    const sf::Vector2f hole_bottomMid = sf::Vector2f(window_width / 2, (window_height + table_height + table_topBottomWall_height * 0.5) / 2);
    const sf::Vector2f hole_topRight = sf::Vector2f((window_width + table_width) / 2, table_offsetY);
    const sf::Vector2f hole_bottomRight = sf::Vector2f((window_width + table_width) / 2, (table_offsetY + table_height));

};

/* ------ Use This Struct Inheritance to Hold All Size, Position, and Color References ------ */

struct References : public SizePositionRef, public ColorRef {
    std::vector<float> generateBallsPositionX (int ballCount, float ballRadius, sf::Vector2f playGroundDimension);
    std::vector<float> generateBallsPositionY (int ballCount, float ballRadius, sf::Vector2f playGroundDimension);
    std::vector<sf::Vector2f> generateBallsPositions(int ballcount, float ballRadius, sf::Vector2f playGroundDimension, sf::Vector2f offset);
    void displayPosition(std::string log, sf::Vector2f position);
    std::vector<sf::Vector2f> generateHolesPositions (int holeCount, float holeRadius, sf::Vector2f playGroundDimension, sf::Vector2f offset);
}; 

/* ------------------------------------------------------------------------------------------ */

class Ball : private References {
private:
    // Private Variables
    sf::CircleShape shape;
    sf::Vector2f velocity;

public:
    // Constructor
    Ball(sf::Vector2f position, sf::Color color);

    // Functions
    void draw(sf::RenderWindow& window);
    void applyForce(const sf::Vector2f& force);
    bool checkCollision(const Ball& other) const;
    void resolveCollision(Ball& other);
    void update();
    void setPosition(const sf::Vector2f& position);
    void setVelocity(const sf::Vector2f& velocity);

    // Getter Functions
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    
};


class CueStick : private References {
private:
    sf::RectangleShape stick;
    sf::Vector2f startPosition;
    sf::Vector2f endPosition;
    bool isDragging = false;
    float power;

public:
    CueStick();
    void startDragging(const sf::Vector2f& cueBallPosition);
    void stopDragging();
    void update(const sf::Vector2f& mousePosition);
    void draw(sf::RenderWindow& window);

    // Getter Functions
    bool isDrag() const;
    float getPower() const;
    sf::Vector2f getDirection(const sf::Vector2f& cueBallPosition) const;
};

class Hole : private References {
private:
    sf::CircleShape shape;

public:
    Hole(sf::Vector2f position);
    void draw(sf::RenderWindow& window);
    bool isBallInHole(const sf::Vector2f& ballPosition, float ballRadius) const;
    // Getter Functions
    sf::Vector2f getPosition() const;
};

class Table : private References {
    private:
        sf::RectangleShape table;
        sf::RectangleShape topWall, bottomWall, leftWall, rightWall;
        sf::RectangleShape topWallShadow, bottomWallShadow, leftWallShadow, rightWallShadow;
        sf::CircleShape topLeftCorner, topRightCorner, bottomLeftCorner, bottomRightCorner;

    public:
        // Constructor
        Table();

        // Functions
        void draw(sf::RenderWindow& window);

        // Getter Functions
        sf::Vector2f getPosition();
        sf::Vector2f getDimension();

};

class Game : private References {

private:
    // Window Variables
    sf::RenderWindow* window;
    sf::Event ev;
    sf::VideoMode videoMode;

    // Game Objects
    Ball* ball;
    Ball* cueBall;
    std::vector<Ball*> balls;
    CueStick cueStick;
    Table table;
    std::vector<sf::Vector2f> ballPositions;
    Hole* hole;
    std::vector<Hole*> holes;
    sf::Vector2f initialCueBallPosition;
   

    // Private Functions
    void initVariables();
    void initWindow();
    void initBalls();
    void initHoles();
    void resetBalls();
    bool areBallsMoving() const;

public:
    // Constructor / Destructor
    Game();
    virtual ~Game();

    // Getter Functions
    const bool running() const; // SafePromising not to modify Object Class Member Variables and not modify returned value

    // Functions
    void pollEvents();
    void update();
    void render();

};
