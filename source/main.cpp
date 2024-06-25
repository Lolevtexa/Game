#include "mainScene.hpp"
#include <SFML/Graphics.hpp>

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");
  window.setFramerateLimit(60);

  MainScene mainScene([&window]() { window.close(); });

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      mainScene.eventProcessing(event);
    }
    mainScene.update();

    window.clear(sf::Color::White);
    // Draw your game objects here
    window.draw(mainScene);
    window.display();
  }

  return 0;
}
