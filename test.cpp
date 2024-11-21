#include <iostream>
#include <vector>
/*

table-> ( 1250, 750)
topWallShadow-> ( 1250, 120)
bottomWallShadow-> ( 1250, 1380)
leftWallShadow-> ( 220, 750)
rightWallShadow-> ( 2280, 750)
topWall-> ( 1250, 120)
bottomWall-> ( 1250, 1380)
leftWall-> ( 220, 750)
rightWall-> ( 2280, 750)
topRightCorner-> ( 3310, 60)
topLeftCorner-> ( -810, 60)
bottomLeftCorner-> ( -810, 1440)
bottomRightCorner-> ( 3310, 1440)

// 2000 x 1200

(1425, 500),  // ball_1
(1475, 475),  // ball_2 
(1475, 525),  // ball_3
(1525, 450),  // ball_4
(1525, 500),  // ball_5
(1525, 550),  // ball_6
(1575, 425),  // ball_7
(1575, 475),  // ball_8
(1575, 525),  // ball_9
(1575, 575),  // ball_10
(1625, 400),  // ball_11
(1625, 450),  // ball_12
(1625, 500),  // ball_13
(1625, 550),  // ball_14
(1625, 600),  // ball_15
(500, 500)    // Cue Ball
 */

void printer(std::vector<float> arrayX, std::vector<float> arrayY) {
    if (arrayX.size() == arrayY.size()) {
        for(int i = 0; i < arrayX.size(); i++) {
            std::cout << "(" << arrayX[i] << ", " << arrayY[i] << ")" << std::endl;
        }
    }
}
std::vector<float> arrayY(int ballCount) {
    std::vector<float> array;
    if (ballCount == 15) {
        for (int i = 0; i <= 4; i++) {
            int currentOrder = i + 1;
            if (currentOrder == 1) { // First Order Ball
                array.emplace_back(500);
            }
            else if (currentOrder % 2 == 0) { // Even Order Ball
                for(int ii = 1; ii <= currentOrder / 2; ii++) {                
                    array.emplace_back(500 - 25 * (ii));
                    array.emplace_back(500 + 25 * (ii));
                }
            }
            else { // Odd Order Ball
                for(int ii = 1; ii <= (currentOrder - 1) / 2; ii++) {                
                    array.emplace_back(500 - 50 * (ii));
                    array.emplace_back(500 + 50 * (ii));
                }
                array.emplace_back(500);
            }
         }
        array.emplace_back(500);
    }
   return array;
}

std::vector<float> arrayX (int ballCount) {
    std::vector<float> array;
    if(ballCount == 15) {
        for(int i = 0; i <= 4; i++) {
            for(int ii = 0; ii <= i; ii++) {
                array.emplace_back(1425 + i * 50);
            }
        }
        array.emplace_back(500);
    }
   return array;
}


int main() {
    std::vector<float> ballPositionsY = arrayY(15);
    std::vector<float> ballPositionsX = arrayX(15);
    printer(ballPositionsX, ballPositionsY);
}

 // {
    //     sf::Vector2f(offsetTablePosition.x + 1425, offsetTablePosition.y + 500),
    //     sf::Vector2f(ballPositions[0].x + 2 * sizeOf.ball_radius, ballPositions[0].y - sizeOf.ball_radius),  // ball_2 1475, 475
    //     sf::Vector2f(1475, 525),  // ball_3
    //     sf::Vector2f(1525, 450),  // ball_4
    //     sf::Vector2f(1525, 500),  // ball_5
    //     sf::Vector2f(1525, 550),  // ball_6
    //     sf::Vector2f(1575, 425),  // ball_7
    //     sf::Vector2f(1575, 475),  // ball_8
    //     sf::Vector2f(1575, 525),  // ball_9
    //     sf::Vector2f(1575, 575),  // ball_10
    //     sf::Vector2f(1625, 400),  // ball_11
    //     sf::Vector2f(1625, 450),  // ball_12
    //     sf::Vector2f(1625, 500),  // ball_13
    //     sf::Vector2f(1625, 550),  // ball_14
    //     sf::Vector2f(1625, 600),  // ball_15
    //     sf::Vector2f(500, 500)    // Cue Ball
    // };