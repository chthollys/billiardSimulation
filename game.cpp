#include "game.h"


/* === Ball Position Engine Definition STARTS HERE === */

std::vector<float> References::generateBallsPositionX(int ballCount, float ballRadius, sf::Vector2f playGroundDimension) {
    std::vector<float> array;

    if (ballCount == 16) {
        float baseX = playGroundDimension.x * 3 / 4;  // Base X position for the triangle
        for (int i = 0; i < 5; ++i) {  // 5 rows
            float rowOffset = 2 * ballRadius * i;  // Horizontal shift for each row
            for (int j = 0; j <= i; ++j) {  // Number of balls in the row
                array.emplace_back(baseX + rowOffset);
            }
        }
        array.emplace_back(playGroundDimension.x / 4);  // Position for the cue ball
    }

    return array;
}


std::vector<float> References::generateBallsPositionY(int ballCount, float ballRadius, sf::Vector2f playGroundDimension) {
    std::vector<float> array;

    if (ballCount == 16) {
        float middleY = playGroundDimension.y / 2;  // Vertical center of the table
        for (int i = 0; i < 5; ++i) {  // 5 rows
            for (int j = 0; j <= i; ++j) {
                float verticalOffset = (j - i / 2.0f) * 2 * ballRadius;  // Center the balls in the row
                array.emplace_back(middleY + verticalOffset);
            }
        }
        array.emplace_back(middleY);  // Position for the cue ball
    }

    return array;
}


std::vector<sf::Vector2f> References::generateBallsPositions(int ballcount, float ballRadius, sf::Vector2f playGroundDimension, sf::Vector2f offset) {
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

std::vector<sf::Vector2f> References::generateHolesPositions(int holeCount, float holeRadius, sf::Vector2f playGroundDimension, sf::Vector2f offset) {
    std::vector<sf::Vector2f> array;
    std::vector<sf::Vector2f> positions = {
        hole_topLeft,       // Hole TopLeft
        hole_bottomLeft,    // Hole BottomLeft
        hole_topMid,        // Hole TopMid
        hole_bottomMid,     // Hole BottomMid
        hole_topRight,      // Hole TopRight
        hole_bottomRight    // Hole BottomRight
    };
    
    std::cout << "Generating positions for " << holeCount << " holes." << std::endl;

    for(int i = 0; i < positions.size(); i++) {
        array.push_back(positions[i]);
        std::cout << "Generated positions for " << " hole-" << i + 1 << std::endl;
    }
    
    std::cout << "Generated positions for " << holeCount << " holes." << std::endl;

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

// Setter Functions

void Ball::setPosition(sf::Vector2f position) {
    this->shape.setPosition(position);
}

void Ball::setVelocity(sf::Vector2f velocity) {
    this->velocity = velocity;
}

/* === SolidBall Class Definition STARTS HERE === */

SolidBall::SolidBall(sf::Vector2f position, sf::Color color) : Ball(position, color) {
    shape.setRadius(ball_radius);
    shape.setPosition(position);
    shape.setOrigin(ball_radius, ball_radius);
    shape.setFillColor(color);
}
/* === StripedBall Class Definition STARTS HERE === */

StripedBall::StripedBall(sf::Vector2f position, sf::Color color) : Ball(position, color) {
    shape.setRadius(ball_radius - ball_border_width / 2);
    shape.setPosition(position);
    shape.setOrigin(ball_radius - ball_border_width / 2, ball_radius - ball_border_width / 2);
    shape.setOutlineThickness(ball_border_width);
    shape.setOutlineColor(ballBorderColor);
    shape.setFillColor(color);
}

/* === BlackBall Class Definition STARTS HERE === */

BlackBall::BlackBall(sf::Vector2f position, sf::Color color) : Ball(position, color) {
    shape.setRadius(ball_radius);
    shape.setPosition(position);
    shape.setOrigin(ball_radius, ball_radius);
    shape.setFillColor(color); // Typically black
}

/* === CueStick Class Definition STARTS HERE === */

CueStick::CueStick() : power(0.0f) {
    if (!stickTexture.loadFromFile("./img/cue_stick.png")) {
        std::cerr << "Failed to load cue stick texture!" << std::endl;
    }
    stickSprite.setTexture(stickTexture);
    stickSprite.setOrigin(0, stickTexture.getSize().y / 2);
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

        sf::Vector2f direction = endPosition - startPosition;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length != 0) {
            direction /= length;
        }

        float offsetDistance = std::min(maxOffsetDistance, minOffsetDistance + length / 2.0f);
        sf::Vector2f stickPosition = startPosition + direction * offsetDistance;
        stickSprite.setPosition(stickPosition);
        stickSprite.setRotation(std::atan2(direction.y, direction.x) * 180.0f / phi);

        power = std::min(100.0f, length / force_scaling_factor);
    }
}

void CueStick::draw(sf::RenderWindow& window) {
    if (isDragging) {
        window.draw(stickSprite);
    }
}

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
        return sf::Vector2f(0, 0);
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
    return distance <= ((hole_radius + ballRadius) * 0.7); // Ball is in hole if it overlaps with the hole radius
}

sf::Vector2f Hole::getPosition() const {
    return shape.getPosition();
}

/* === Table Class Definitions === */

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
    this->playerTurn = 1; // Set playerTurn to Player 1 by default
    this->playerScores[0] = 0; // Player 1 score
    this->playerScores[1] = 0; // Player 2 score
    this->isCueBallPocketed = false; // Initialize to prevent undefined behavior

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
    ballPositions = generateBallsPositions(ballCount, ball_radius, table_dimension, table_offset);

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

        Ball* newBall;
        if (i == ballCount - 1) {  // Last ball is the cue ball
            newBall = new Ball(ballPositions[i], ballColors[i]);
            cueBall = newBall;

            // Store the initial position for teleportation
            initialCueBallPosition = ballPositions[i];

            std::cout << "Cue ball initialized." << std::endl;
        } else if (i == 7) {  // The 8th ball is the BlackBall
            newBall = new BlackBall(ballPositions[i], sf::Color::Black);
            std::cout << "BlackBall initialized." << std::endl;
        } else if (i % 2 == 0) {  // Even-indexed balls as SolidBall
            newBall = new SolidBall(ballPositions[i], ballColors[i]);
            std::cout << "SolidBall initialized." << std::endl;
        } else {  // Odd-indexed balls as StripedBall
            newBall = new StripedBall(ballPositions[i], ballColors[i]);
            std::cout << "StripedBall initialized." << std::endl;
        }

        balls.push_back(newBall);
        std::cout << "Ball " << i << " initialized at position ("
                  << ballPositions[i].x << ", " << ballPositions[i].y << ")" << std::endl;
    }

    std::cout << "Balls initialized." << std::endl;
}




void Game::initHoles() {
    std::vector<sf::Vector2f> holesPosition = generateHolesPositions(holeCount, hole_radius, table_dimension, table_offset);

    if (holesPosition.size() < holeCount) {
        std::cout << "Error: ballColors has fewer elements than expected." << std::endl;
        return; // Exit to prevent invalid access
    }

    for (int i = 0; i < holesPosition.size(); i++){
        std::cout << "Initializing hole " << i << std::endl;
        Hole* newHole = new Hole(holesPosition[i]);
        holes.push_back(newHole);
        std::cout << "Hole " << i << " initialized at position (" << holesPosition[i].x << ", " << holesPosition[i].y << ")" << std::endl;
    }
}

void Game::initSoundEffects() {
    if (!collisionSoundBuffer.loadFromFile("./sound/ball_collide.wav")) {
        std::cerr << "Failed to load collision sound!" << std::endl;
    } 
    collisionSound.setBuffer(collisionSoundBuffer);

    if (!cueStickHitBuffer.loadFromFile("./sound/pool_ball_hit.wav")) {
        std::cerr << "Failed to load cue stick hit sound!" << std::endl;
    }
    cueStickHitSound.setBuffer(cueStickHitBuffer);
}

void Game::initFontText() {
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);

    turnText.setFont(font);
    turnText.setCharacterSize(30);
    turnText.setFillColor(sf::Color::White);
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

void Game::updateUI() {
    // Update teks skor
    scoreText.setString(
        "Player 1: " + std::to_string(playerScores[0]) + 
        "  Player 2: " + std::to_string(playerScores[1])
    );
    scoreText.setCharacterSize(40);  // Ukuran font yang lebih besar
    scoreText.setFillColor(sf::Color::White);  // Warna putih
    scoreText.setPosition(window_width / 2 - scoreText.getLocalBounds().width / 2, 10.f);  // Tengah horizontal

    // Update teks giliran pemain
    turnText.setString("Turn: Player " + std::to_string(playerTurn));
    turnText.setCharacterSize(40);  // Ukuran font yang lebih besar
    turnText.setFillColor(sf::Color::White);  // Warna putih
    turnText.setPosition(window_width / 2 - turnText.getLocalBounds().width / 2, 50.f);  // Tengah horizontal
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

    this->initHoles();
    std::cout << "Holes initialized." << std::endl;

    this->initFontText();
    std::cout << "Font Text UI initialized." << std::endl;

    this->initSoundEffects();
    std::cout << "Sound effects initialized." << std::endl;

    std::cout << "Game created successfully." << std::endl;
}

// Destructor
Game::~Game() {
    delete this->window;
    for(auto& ball : balls) {
        delete ball;
        ball = nullptr; // Extra safe step for memory leaks case
    }
    for(auto& hole : holes) {
        delete hole;
        hole = nullptr;
    }
}

// Getter Functions
const bool Game::running() const {
    bool isRunning = this->window && this->window->isOpen();
    // std::cout << "Game running: " << std::boolalpha << isRunning << std::endl;
    return isRunning;
}


// Functions
void Game::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;

            case sf::Event::MouseButtonPressed:
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->window));
                    float dx = mousePosition.x - cueBall->getPosition().x;
                    float dy = mousePosition.y - cueBall->getPosition().y;
                    float distanceSquared = dx * dx + dy * dy;

                    if (distanceSquared <= ball_radius * ball_radius) {
                        // Mouse clicked inside the cue ball
                        if (isCueBallDraggable) {
                            std::cout << "Dragging the cue ball..." << std::endl;
                        }
                    } else {
                        // Mouse clicked outside the cue ball
                        if (isCueBallDraggable) {
                            isCueBallDraggable = false; // Disable dragging
                        }

                        if (!areBallsMoving()) {
                            // Start dragging the cue stick
                            cueStick.startDragging(cueBall->getPosition());
                        }
                    }
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (ev.mouseButton.button == sf::Mouse::Left) {
                    if (!isCueBallDraggable) {
                        // Apply force to the cue ball when the cue stick is released
                        sf::Vector2f direction = cueStick.getDirection(cueBall->getPosition());
                        float power = cueStick.getPower();
                        cueBall->applyForce(direction * power);
                        cueStick.stopDragging();
                        
                        float volume = std::min(100.0f, power); // Cap volume at 100
                        cueStickHitSound.setVolume(volume);
                        cueStickHitSound.play();
                    }
                }
                break;

            default:
                break;
        }
    }
}



void Game::update() {
    this->pollEvents();
    
    bool ballPocketed = false;      // Indikator apakah ada bola masuk ke lubang
    bool cueBallPocketed = false;  // Indikator apakah bola putih masuk ke lubang
    static bool playerScored = false;     // Indikator apakah pemain mendapatkan poin
    static bool allBallsStopped = true;  // Status bola berhenti, default true untuk awal permainan

    if (isCueBallDraggable) {
        isDraggingCueBall = true;
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->window));

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // Calculate the desired position of the cue ball
            sf::Vector2f desiredPosition = mousePosition;

            // Check collisions with other balls
            bool canMove = true;
            for (Ball* ball : balls) {
                if (ball != cueBall) {
                    float distance = std::sqrt(std::pow(ball->getPosition().x - desiredPosition.x, 2) +
                                            std::pow(ball->getPosition().y - desiredPosition.y, 2));
                    if (distance <= ball_radius * 2) {
                        canMove = false;
                        break; // Stop checking further if a collision is found
                    }
                }
            }

            // Move the cue ball only if there are no collisions
            if (canMove) {
                cueBall->setPosition(desiredPosition);
            }
        } else {
            isDraggingCueBall = false; // Reset dragging state when the button is released
        }
    }


    if (cueStick.isDrag() && !isCueBallDraggable) {
        sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*this->window));
        cueStick.update(mousePosition);
    }

    for (Ball* ball : balls) {
        ball->update();
    }

    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            // Skip collisions for the cue ball while dragging
            if (isDraggingCueBall && (balls[i] == cueBall || balls[j] == cueBall)) {
                continue;
            }

            if (balls[i]->checkCollision(*balls[j])) {
                balls[i]->resolveCollision(*balls[j]);
                // Add collision sound effect
                float collisionIntensity = std::min(
                    100.0f, 
                    static_cast<float>(std::sqrt(std::pow(balls[i]->getVelocity().x, 2) + std::pow(balls[i]->getVelocity().y, 2)) +
                    std::sqrt(std::pow(balls[j]->getVelocity().x, 2) + std::pow(balls[j]->getVelocity().y, 2)))
                );
                collisionSound.setVolume(collisionIntensity); // Volume based on intensity (0 - 100)
                collisionSound.play();
            }
        }
    }

    for (Hole* hole : holes) {
        for (auto it = balls.begin(); it != balls.end();) {
            if (hole->isBallInHole((*it)->getPosition(), ball_radius)) {
                if (*it == cueBall) {
                    cueBallPocketed = true;
                    std::cout << "Cue ball fell into the hole! Teleporting to initial position." << std::endl;
                    cueBall->setVelocity({0, 0});
                    cueBall->setPosition(initialCueBallPosition);
                    isCueBallDraggable = true;
                    ++it;
                } else {
                    playerScores[playerTurn - 1] += 1;  // Tambahkan skor ke pemain aktif
                    playerScored = true;  // Tandai bahwa pemain mendapat poin
                    Ball* ball = *it;
                    if (dynamic_cast<SolidBall*>(ball)) {
                        pocketedSolidBalls.push_back(ball);
                    } else if (dynamic_cast<StripedBall*>(ball)) {
                        pocketedStripedBalls.push_back(ball);
                    }
                    it = balls.erase(it);
                }
            } else {
                ++it;
            }
        }
    }

    // Periksa apakah semua bola sudah berhenti
    bool currentBallsStopped = !areBallsMoving();

    if (currentBallsStopped && !allBallsStopped) {
        if (cueBallPocketed) {
            // Penalti jika bola putih masuk
            cueBallPocketed = false;  // Reset indikator
            playerTurn = (playerTurn % 2) + 1;  // Ganti giliran pemain
        } else if (!playerScored) {
            // Ganti giliran pemain hanya jika tidak mendapatkan poin
            playerTurn = (playerTurn % 2) + 1;
            
        } else {
            // Pemain mencetak poin, giliran tidak berubah
        }

        // Reset indikator playerScored hanya setelah giliran diproses
        playerScored = false;
    }

    allBallsStopped = currentBallsStopped;  // Update status berhenti bola

    this->updateUI();  // Perbarui tampilan UI
}



void Game::render() {
    this->window->clear(window_color);

    table.draw(*this->window);

    for (Hole* hole : holes) {
        hole->draw(*this->window);
    }

    for (Ball* ball : balls) {
        ball->draw(*this->window);
    }

    // Render pocketed solid balls
    for (size_t i = 0; i < pocketedSolidBalls.size(); ++i) {
        sf::Vector2f position(50 , 50 + i * (2 * ball_radius + 10)); // Top-left alignment
        pocketedSolidBalls[i]->setPosition(position);
        pocketedSolidBalls[i]->draw(*this->window);
    }

    // Render pocketed striped balls
    for (size_t i = 0; i < pocketedStripedBalls.size(); ++i) {
        sf::Vector2f position(window_width - 50, 50 + i * (2 * ball_radius + 10)); // Top-right alignment
        pocketedStripedBalls[i]->setPosition(position);
        pocketedStripedBalls[i]->draw(*this->window);
    }

    cueStick.draw(*this->window);

    this->window->draw(scoreText);
    this->window->draw(turnText);

    this->window->display();
}


