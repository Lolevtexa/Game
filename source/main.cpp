#include "mainScene.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
// #include <windows.h>

void setTitle(sf::RenderWindow &window) {
  window.setTitle(utf8_to_wstring(Resource::localization["window name"]));
}

void setFullscreen(sf::RenderWindow &window, bool &isFullscreen) {
  if (!isFullscreen) {
    window.create(sf::VideoMode::getDesktopMode(), "", sf::Style::Fullscreen);
    setTitle(window);
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "fullscreen";
    isFullscreen = true;
  }
}

void setWindowed(sf::RenderWindow &window, bool &isFullscreen) {
  if (isFullscreen) {
    window.create(sf::VideoMode(800, 600),
                  utf8_to_wstring(Resource::localization["window name"]),
                  sf::Style::Default);
    setTitle(window);
    window.setVerticalSyncEnabled(true);

    Resource::userSettings["screen mode"] = "windowed";
    isFullscreen = false;
  }
}

int main() {
  // ShowWindow(GetConsoleWindow(), SW_HIDE);

  bool isFullscreen = Resource::userSettings["screen mode"] != "fullscreen";
  sf::RenderWindow window;
  if (!isFullscreen) {
    setFullscreen(window, isFullscreen);
  } else {
    setWindowed(window, isFullscreen);
  }

  MainScene mainScene(
      [&window]() { window.close(); },
      [&window, &isFullscreen]() { setFullscreen(window, isFullscreen); },
      [&window, &isFullscreen]() { setWindowed(window, isFullscreen); },
      [&window]() { setTitle(window); }, window.getSize());

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
      }

      mainScene.eventProcessing(event);
      Resource::writeJson("assets/settings.json", Resource::userSettings);
    }
    mainScene.update();

    window.clear(sf::Color::White);
    // Draw your game objects here
    window.draw(mainScene);
    window.display();
  }

  return 0;
}
