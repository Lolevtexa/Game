#include "mainScene.hpp"
#include <SFML/Graphics.hpp>

void setFullscreen(sf::RenderWindow &window, bool &isFullscreen) {
  if (!isFullscreen) {
    window.create(sf::VideoMode::getDesktopMode(), "SFML Game",
                  sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    isFullscreen = true;
  }
}

void setWindowed(sf::RenderWindow &window, bool &isFullscreen) {
  if (isFullscreen) {
    window.create(sf::VideoMode(800, 600), "SFML Game", sf::Style::Default);
    window.setFramerateLimit(60);
    isFullscreen = false;
  }
}

int main() {
  bool isFullscreen = false;
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Game");
  window.setFramerateLimit(60);

  MainScene mainScene(
      [&window]() { window.close(); },
      [&window, &isFullscreen]() { setFullscreen(window, isFullscreen); },
      [&window, &isFullscreen]() { setWindowed(window, isFullscreen); });

  for (int i = 0; window.isOpen(); i++) {
    // std::cout << '\r' << i << "; ";
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
      }

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
