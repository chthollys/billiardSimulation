#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "game.h"

// int main() {
//     std::cout << "Program start" << std::endl;

//     sf::RenderWindow window(sf::VideoMode(800, 600), "Billiard Simulation");
//     std::cout << "Window initialized" << std::endl;

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         window.clear();
//         window.display();
//     }

//     std::cout << "Exiting program..." << std::endl;
//     return 0;
// }


int main() {

    std::cout << "Program start" << std::endl;
    // Initialize Game
    Game game;
    
    std::cout << "Game Calling start" << std::endl;

    // Game Loop
    while (game.running()) {

        // Update
        game.update();

        // Render
        game.render();

    }
    
    std::cout << "Program ended" << std::endl;

    return 0;
}
