#include "game.h"


/* === Ball Position Engine Definition STARTS HERE === */

std::vector<float> References::generateBallsPositionX (int ballCount, float ballRadius, sf::Vector2f playGroundDimension) {
    std::vector<float> array;
    if(ballCount == 16) {
        for(int i = 0; i <= 4; i++) {
            for(int ii = 0; ii <= i; ii++) {
                array.emplace_back((playGroundDimension.x * 3 / 4) - (playGroundDimension.x / 25) + 2 * i * ballRadius);
            }
        }
        array.emplace_back(playGroundDimension.x / 4);
    }
    return array;
}

std::vector<float> References::generateBallsPositionY (int ballCount, float ballRadius, sf::Vector2f playGroundDimension) {
    std::vector<float> array;
    float middlePoint = playGroundDimension.y / 2;
    if (ballCount == 16) {
        for (int i = 0; i <= 4; i++) {
            int currentOrder = i + 1;
            if (currentOrder == 1) { // First Order Ball
                array.push_back(middlePoint);
            }
            else if (currentOrder % 2 == 0) { // Even Order Ball
                for(int ii = 1; ii <= currentOrder / 2; ii++) {                
                    array.push_back(middlePoint - ballRadius * ii);
                    array.push_back(middlePoint + ballRadius * ii);
                }
            }
            else { // Odd Order Ball
                for(int ii = 1; ii <= (currentOrder - 1) / 2; ii++) {                
                    array.push_back(middlePoint - 2 * ballRadius * ii);
                    array.push_back(middlePoint + 2 * ballRadius * ii);
                }
                array.push_back(middlePoint);
            }
        }
        array.push_back(middlePoint);
    }
    return array;
}

std::vector<sf::Vector2f> References::generate(int ballcount, float ballRadius, sf::Vector2f playGroundDimension, sf::Vector2f offset) {
    std::vector<sf::Vector2f> array;
    std::cout << "Generating positions for " << ballCount << " balls." << std::endl;

    std::vector<float> arrayX = generateBallsPositionX(ballcount, ballRadius, playGroundDimension);
    std::cout << "Generated X-index positions " << std::endl;

    std::vector<float> arrayY = generateBallsPositionY(ballcount, ballRadius, playGroundDimension);
    std::cout << "Generated Y-index positions " << std::endl;

    if (arrayX.size() == arrayY.size()) {
        std::cout << "X-Index and Y-Index aligned, conducting merging" << std::endl;
        for(int i = 0; i < arrayX.size(); i++) {
            array.push_back(sf::Vector2f(arrayX[i] + offset.x, arrayY[i] + offset.y));
        }
    }
    std::cout << "Position generation complete." << std::endl;
    return array;
}

/* === positionDisplay Class Definition STARTS HERE === */

void References::displayPosition (std::string log, sf::Vector2f position) {
    std::cout << log << "-> " << "( " << position.x 
              << ", " << position.y << ")" << std::endl;
}

/* === Ball Class Definition STARTS HERE === */

Ball::Ball(sf::Vector2f position, sf::Color color) : velocity(0.f, 0.f) {
    shape.setRadius(ball_radius);
    shape.setPosition(position);
    shape.setFillColor(color);
    shape.setOrigin(ball_radius, ball_radius);
    shape.setOutlineThickness(ball_border_width);
    shape.setOutlineColor(ballBorderColor);
}

void Ball::draw(sf::RenderWindow& window) { // Pass by reference for better efficiency and maintain original updated state
    window.draw(shape);
}

void Ball::applyForce(const sf::Vector2f& force) {
    velocity += force;
}

bool Ball::checkCollision(const Ball& other) const {
    float distance = std::sqrt(std::pow(other.getPosition().x - getPosition().x, 2) +
                               std::pow(other.getPosition().y - getPosition().y, 2));
    return distance <= (ball_radius * 2 + 0.1f); // collision if distance <= 2 * radius
}

void Ball::resolveCollision(Ball& other) {
    sf::Vector2f delta = other.getPosition() - getPosition();
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

    // Smallest distance threshold to avoid excessive collision calculations
    if (distance < 1e-6f) return;

    // Normalized direction vector along the collision normal
    sf::Vector2f normal = delta / distance;
    float overlap = (2 * ball_radius - distance) / 2.0f;

    // Adjust overlap to avoid excessive separation
    float separationFactor = 0.5f; // Factor to reduce separation
    shape.move(-normal * overlap * separationFactor);
    other.shape.move(normal * overlap * separationFactor);

    // Tangent vector, perpendicular to normal
    sf::Vector2f tangent(-normal.y, normal.x);

    // Calculate the normal and tangential components of each ball's velocity
    float v1n = velocity.x * normal.x + velocity.y * normal.y;
    float v2n = other.velocity.x * normal.x + other.velocity.y * normal.y;
    float v1t = velocity.x * tangent.x + velocity.y * tangent.y;
    float v2t = other.velocity.x * tangent.x + other.velocity.y * tangent.y;

    // Apply conservation of momentum in the normal direction with restitution
    float newV1n = (v1n * (1 - restitution) + v2n * (1 + restitution)) / 2.0f;
    float newV2n = (v2n * (1 - restitution) + v1n * (1 + restitution)) / 2.0f;

    // Convert the scalar normal and tangential velocities back to vectors
    sf::Vector2f v1nVector = newV1n * normal;
    sf::Vector2f v1tVector = v1t * tangent;  // Keep tangential component unchanged
    sf::Vector2f v2nVector = newV2n * normal;
    sf::Vector2f v2tVector = v2t * tangent;  // Keep tangential component unchanged

    // Update the velocities of each ball by combining the normal and tangential components
    velocity = v1nVector + v1tVector;
    other.velocity = v2nVector + v2tVector;
}


void Ball::update() {
    // Move the ball by its current velocity
    shape.move(velocity);

    // Correct wall collision for x-axis
    if (shape.getPosition().x - ball_radius <= table_offsetX) {
        shape.setPosition(table_offsetX + ball_radius, shape.getPosition().y);
        velocity.x = -velocity.x * restitution; // Reverse x-direction with restitution
    }
    else if (shape.getPosition().x + ball_radius >= window_width - table_offsetX) {
        shape.setPosition(window_width - table_offsetX - ball_radius, shape.getPosition().y);
        velocity.x = -velocity.x * restitution; // Reverse x-direction with restitution
    }

    // Correct wall collision for y-axis
    if (shape.getPosition().y - ball_radius <= table_offsetY) {
        shape.setPosition(shape.getPosition().x, table_offsetY + ball_radius);
        velocity.y = -velocity.y * restitution; // Reverse y-direction with restitution
    }
    else if (shape.getPosition().y + ball_radius >= window_height - table_offsetY) {
        shape.setPosition(shape.getPosition().x, window_height - table_offsetY - ball_radius);
        velocity.y = -velocity.y * restitution; // Reverse y-direction with restitution
    }

    // Apply friction to gradually slow down the ball
    velocity *= friction;

    // Threshold to stop small movements
    if (std::abs(velocity.x) < minVelocityThreshold) velocity.x = 0.0f;
    if (std::abs(velocity.y) < minVelocityThreshold) velocity.y = 0.0f;
}



// Getter Functions

sf::Vector2f Ball::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const {
    return velocity;
}

/* === CueStick Class Definition STARTS HERE === */

CueStick::CueStick() : power(0.0f) {
    stick.setSize(sf::Vector2f(stick_length, stick_width)); // Length and thickness of the stick
    stick.setFillColor(sf::Color::White);
}

void CueStick::startDragging(const sf::Vector2f& cueBallPosition) {
    startPosition = cueBallPosition;
    isDragging = true;
}

void CueStick::stopDragging() {
    isDragging = false;
    power = 0.0f;
}

void CueStick::update(const sf::Vector2f& mousePosition) {
    if (isDragging) {
        endPosition = mousePosition;

        // Calculate direction vector from cue ball to mouse position
        sf::Vector2f direction = endPosition - startPosition;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Normalize direction vector to ensure unit length
        if (length != 0) {
            direction /= length;
        }

        // Calculate a dynamic offset based on drag distance, clamped between min and max
        float offsetDistance = std::min(maxOffsetDistance, minOffsetDistance + length / 2.0f);

        // Position the cue stick with the calculated offset distance from the cue ball
        sf::Vector2f stickPosition = startPosition + direction * offsetDistance;
        stick.setPosition(stickPosition);

        // Set rotation to make the cue stick point toward the mouse position
        stick.setRotation(std::atan2(direction.y, direction.x) * 180.0f / phi);

        // Calculate power based on drag distance
        power = std::min(100.0f, length / force_scaling_factor);

        // // Debugging output
        // std::cout << "Offset Distance: " << offsetDistance << std::endl;
        // std::cout << "Stick Position: (" << stickPosition.x << ", " << stickPosition.y << ")" << std::endl;
        // std::cout << "Power: " << power << std::endl;
    }
}

void CueStick::draw(sf::RenderWindow& window) {
    if (isDragging) {
        window.draw(stick);
    }
}

// Getter Functions 

bool CueStick::isDrag() const {
    return isDragging;
}

float CueStick::getPower() const {
    return power;
}

sf::Vector2f CueStick::getDirection(const sf::Vector2f& cueBallPosition) const {
    sf::Vector2f direction = cueBallPosition - endPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0.0f) {
        return sf::Vector2f(0, 0); // Avoid division by zero
    }
    return direction / length;
}


/* === Hole Class Definitions === */

Hole::Hole(sf::Vector2f position) {
    shape.setRadius(hole_radius);
    shape.setOrigin(hole_radius, hole_radius);
    shape.setPosition(position);
    shape.setFillColor(holeColor); // Default hole color
}

void Hole::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Hole::isBallInHole(const sf::Vector2f& ballPosition, float ballRadius) const {
    float distance = std::sqrt(std::pow(getPosition().x - ballPosition.x, 2) +
                               std::pow(getPosition().y - ballPosition.y, 2));
    return distance <= (hole_radius + ballRadius); // Ball is in hole if it overlaps with the hole radius
}

sf::Vector2f Hole::getPosition() const {
    return shape.getPosition();
}

/* === Table Class Definitions === */

void Table::initHoles() {
    holes.emplace_back(topLeftCornerPosition);
    holes.emplace_back(topRightCornerPosition);
    holes.emplace_back(bottomLeftCornerPosition);
    holes.emplace_back(bottomRightCornerPosition);

    // Middle holes (for sides)
    holes.emplace_back(sf::Vector2f(table_leftWall.x, table_topWall.y + table_height / 2));
    holes.emplace_back(sf::Vector2f(table_rightWall.x, table_topWall.y + table_height / 2));
}

// Constructor

Table::Table() {
    table.setSize(sf::Vector2f(table_width, table_height));
    table.setFillColor(tableColor);
    table.setOrigin(table_width / 2, table_height / 2);
    table.setPosition(window_width / 2, window_height / 2);

    // Walls
    topWall.setSize(sf::Vector2f(table_topBottomWall_width, table_topBottomWall_height));
    topWall.setOrigin(sf::Vector2f(table_topBottomWall_width / 2, table_topBottomWall_height / 2));
    topWall.setFillColor(tableWallColor);
    topWall.setPosition(table_topWall);
    topWallShadow.setSize(sf::Vector2f(table_topBottomWallShadow_width, table_topBottomWallShadow_height));
    topWallShadow.setOrigin(sf::Vector2f(table_topBottomWallShadow_width / 2, table_topBottomWallShadow_height / 2));
    topWallShadow.setPosition(table_topWallShadow);
    topWallShadow.setFillColor(tableShadowColor);
    
    bottomWall.setSize(sf::Vector2f(table_topBottomWall_width, table_topBottomWall_height));
    bottomWall.setOrigin(sf::Vector2f(table_topBottomWall_width / 2, table_topBottomWall_height / 2));
    bottomWall.setFillColor(tableWallColor);    
    bottomWall.setPosition(table_bottomWall);
    bottomWallShadow.setSize(sf::Vector2f(table_topBottomWallShadow_width, table_topBottomWallShadow_height));
    bottomWallShadow.setOrigin(sf::Vector2f(table_topBottomWallShadow_width / 2, table_topBottomWallShadow_height / 2));
    bottomWallShadow.setPosition(table_bottomWallShadow);
    bottomWallShadow.setFillColor(tableShadowColor);

    leftWall.setSize(sf::Vector2f(table_leftRightWall_width, table_leftRightWall_height));
    leftWall.setOrigin(sf::Vector2f(table_leftRightWall_width / 2, table_leftRightWall_height / 2));
    leftWall.setFillColor(tableWallColor);
    leftWall.setPosition(table_leftWall);
    leftWallShadow.setSize(sf::Vector2f(table_leftRightWallShadow_width, table_leftRightWallShadow_height));
    leftWallShadow.setOrigin(sf::Vector2f(table_leftRightWallShadow_width / 2, table_leftRightWallShadow_height / 2));
    leftWallShadow.setPosition(table_leftWallShadow);
    leftWallShadow.setFillColor(tableShadowColor);

    rightWall.setSize(sf::Vector2f(table_leftRightWall_width, table_leftRightWall_height));
    rightWall.setOrigin(sf::Vector2f(table_leftRightWall_width / 2, table_leftRightWall_height / 2));
    rightWall.setFillColor(tableWallColor);
    rightWall.setPosition(table_rightWall);
    rightWallShadow.setSize(sf::Vector2f(table_leftRightWallShadow_width, table_leftRightWallShadow_height));
    rightWallShadow.setOrigin(sf::Vector2f(table_leftRightWallShadow_width / 2, table_leftRightWallShadow_height / 2));
    rightWallShadow.setPosition(table_rightWallShadow);
    rightWallShadow.setFillColor(tableShadowColor);

    // Wall Corner
    topLeftCorner.setRadius(cornerRadius);
    topLeftCorner.setOrigin(cornerRadius, cornerRadius);
    topLeftCorner.setPosition(topLeftCornerPosition);
    topLeftCorner.setFillColor(tableWallColor);

    topRightCorner.setRadius(cornerRadius);
    topRightCorner.setOrigin(cornerRadius, cornerRadius);
    topRightCorner.setPosition(topRightCornerPosition);
    topRightCorner.setFillColor(tableWallColor);

    bottomLeftCorner.setRadius(cornerRadius);
    bottomLeftCorner.setOrigin(cornerRadius, cornerRadius);
    bottomLeftCorner.setPosition(bottomLeftCornerPosition);
    bottomLeftCorner.setFillColor(tableWallColor);

    bottomRightCorner.setRadius(cornerRadius);
    bottomRightCorner.setOrigin(cornerRadius, cornerRadius);
    bottomRightCorner.setPosition(bottomRightCornerPosition);
    bottomRightCorner.setFillColor(tableWallColor);

    this->initHoles();
}

// Functions

void Table::draw(sf::RenderWindow& window) {
    window.draw(table);
    
    window.draw(topWallShadow);
    window.draw(bottomWallShadow);
    window.draw(leftWallShadow);
    window.draw(rightWallShadow);

    window.draw(topWall);
    window.draw(bottomWall);
    window.draw(leftWall);
    window.draw(rightWall);

    window.draw(topLeftCorner);
    window.draw(topRightCorner);
    window.draw(bottomLeftCorner);
    window.draw(bottomRightCorner);

    // Debug Table Component Positions
    // displayPosition("table", table.getPosition());
    // displayPosition("topWallShadow", topWallShadow.getPosition());
    // displayPosition("bottomWallShadow", bottomWallShadow.getPosition());
    // displayPosition("leftWallShadow", leftWallShadow.getPosition());
    // displayPosition("rightWallShadow", rightWallShadow.getPosition());
    // displayPosition("topWall", topWall.getPosition());
    // displayPosition("bottomWall", bottomWall.getPosition());
    // displayPosition("leftWall", leftWall.getPosition());
    // displayPosition("rightWall", rightWall.getPosition());
    // displayPosition("topRightCorner", topRightCorner.getPosition());
    // displayPosition("topLeftCorner", topLeftCorner.getPosition());
    // displayPosition("bottomLeftCorner", bottomLeftCorner.getPosition());
    // displayPosition("bottomRightCorner", bottomRightCorner.getPosition());

    // displayPosition("rightWallShadow", rightWallShadow.getPosition());
    // std::cout << "Origin : (" << rightWallShadow.getOrigin().x << ", " << rightWallShadow.getOrigin().y << ")" << std::endl;
}


// Getter Functions

sf::Vector2f Table::getPosition() {
    return topWall.getPosition();
}

sf::Vector2f Table::getDimension() {
    return table.getSize();
}

/* 
=== Game Class Definition STARTS HERE === 
    
    Game Class acts as the game engine

*/

// Private Functions
void Game::initVariables() {
    this->window = nullptr;
    std::cout << "variable initialized" << std::endl;
}

void Game::initWindow() {
    this->videoMode.width = window_width;
    this->videoMode.height = window_height;
    this->window = new sf::RenderWindow(this->videoMode, "Billiard Simulation");

    if (this->window) {
        std::cout << "Window created successfully with width: " << this->videoMode.width 
                  << " and height: " << this->videoMode.height << std::endl;
    } else {
        std::cout << "Failed to create window." << std::endl;
    }

    this->window->setFramerateLimit(frLimit);
}


void Game::initBalls() {
    std::cout << "Starting ball initialization..." << std::endl;
    ballPositions = generate(ballCount, ball_radius, table_dimension, table_offset);

    // std::cout << "Ball positions generated." << std::endl;

    if (ballColors.size() < ballCount) {
        std::cout << "Error: ballColors has fewer elements than expected." << std::endl;
        return; // Exit to prevent invalid access
    }
    if (ballPositions.size() < ballCount) {
        std::cout << "Error: ballPositions has fewer elements than expected." << std::endl;
        return; // Exit to prevent invalid access
    }

    for (int i = 0; i < ballCount; ++i) {
        std::cout << "Initializing ball " << i << std::endl;
        Ball* newBall = new Ball(ballPositions[i], ballColors[i]);
        balls.push_back(newBall);
        std::cout << "Ball " << i << " initialized at position (" << ballPositions[i].x << ", " << ballPositions[i].y << ")" << std::endl;

        if (i == ballCount - 1) { // Last ball as cue ball
            cueBall = newBall;
            std::cout << "Cue ball initialized." << std::endl;
        }
    }
    // for(int i = 0; i <= ballCount; ++i) {
    //     std::cout << "Initializing ball " << i << std::endl;
    //     this->ball = new Ball(ballPositions[i], ballColors[i]);
    //     balls.push_back(ball);
    //     if (i == 15) { // Last ball in list as cue ball
    //         cueBall = ball;
    //     }
    // }
    std::cout << "Balls initialized" << std::endl;
}

void Game::resetBalls() {
    // Delete existing balls
    for (Ball* ball : balls) {
        delete ball;
    }
    balls.clear();

    // Reinitialize balls as needed
    this->initBalls();
}

bool Game::areBallsMoving() const {
    for (const Ball* ball : balls) {
        if (std::abs(ball->getVelocity().x) > minVelocityThreshold ||
            std::abs(ball->getVelocity().y) > minVelocityThreshold) {
            return true; // A ball is still in motion
        }
    }
    return false; // All balls have stopped
}

// Constructor 
Game::Game() {
    std::cout << "Starting game construction..." << std::endl;

    this->initVariables();
    std::cout << "Variables initialized." << std::endl;

    this->initWindow();
    std::cout << "Window initialized." << std::endl;

    this->initBalls();
    std::cout << "Balls initialized." << std::endl;

    std::cout << "Game created successfully." << std::endl;
}

// Destructor
Game::~Game() {
    delete this->window;
    for(auto& ball : balls) {
        delete ball;
        ball = nullptr; // Extra safe step for memory leaks case
    }
    std::cout << "Game resources cleaned up." << std::endl;
}

// Getter Functions
const bool Game::running() const {
    bool isRunning = this->window && this->window->isOpen();
    // std::cout << "Game running: " << std::boolalpha << isRunning << std::endl;
    return isRunning;
}


// Functions
void Game::pollEvents() {
    while(this->window->pollEvent(this->ev)) {

        switch(this->ev.type) {
            
        case sf::Event::Closed:
            std::cout << "Close event received." << std::endl;
            this->window->close();
            break;
        case sf::Event::MouseButtonPressed:
                if (ev.mouseButton.button == sf::Mouse::Left && !areBallsMoving()) {
                    cueStick.startDragging(cueBall->getPosition());
                }
                // std::cout << "(" << table.getPosition().x << ", " << table.getPosition().y << ")" << std::endl;
                break;
        case sf::Event::MouseButtonReleased:
            if (ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f direction = cueStick.getDirection(cueBall->getPosition());
                float power = cueStick.getPower();
                // std::cout << "Power : " <<power << std::endl;
                // std::cout << "Direction : " << direction.x << ", " << direction.y << std::endl;
                cueBall->applyForce(direction * power); // Apply force to cue ball
                cueStick.stopDragging();
            }
            break;
        default:
            // std::cout << "Other event received: " << this->ev.type << std::endl;
            break;
        } 
    }   
}
void Game::update() {

    this->pollEvents();

    if (cueStick.isDrag()) {
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->window));
        cueStick.update(mousePosition);
    }
    // Update each ball's position based on its velocity
    for (Ball* ball : balls) {
        ball->update();
    }

    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (balls[i]->checkCollision(*balls[j])) {
                balls[i]->resolveCollision(*balls[j]);
            }
        }
    }
}
void Game::render() {

    /*
        - Clear Old Frame
        - Render Objects
        - Display Frame in window
    */

    this->window->clear();

    // Draw Game Objects
    table.draw(*this->window);
    
    for(Ball* ball : balls) {
        ball->draw(*this->window); // Dereference usage for getting the correct argument type
    }

    cueStick.draw(*this->window);

    

    this->window->display();

}

