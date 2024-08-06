#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Resource {
public:
  static const int characterSize = 24;
  static const int lineSpacing = characterSize / 2;

  static const sf::Color focusedColor;
  static const sf::Color unfocusedColor;
  static const sf::Color transparentColor;

  static const sf::Font defaultFont;

  static nlohmann::json localization;

  static sf::Music *loadBackgroundMusic() {
    sf::Music *music = new sf::Music();
    if (!music->openFromFile("assets/music/hound-dog-jazz.mp3")) {
      throw std::runtime_error("Cannot load music");
    }

    return music;
  }

  static std::vector<std::string> listLocalizations() {
    std::string directoryPath = "assets/localization";
    std::vector<std::string> jsonFiles;

    try {
      if (!std::filesystem::exists(directoryPath) ||
          !std::filesystem::is_directory(directoryPath)) {
        std::cerr << "Directory does not exist or is not a directory."
                  << std::endl;
        return jsonFiles;
      }

      for (const auto &entry :
           std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
          auto path = entry.path();
          if (path.extension() == ".json") {
            jsonFiles.push_back("assets/localization/" +
                                path.filename().string());
          }
        }
      }
    } catch (const std::filesystem::filesystem_error &e) {
      std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard exception: " << e.what() << std::endl;
    }

    return jsonFiles;
  }

  static void loadLocalization(const std::string &filename) {
    try {
      localization = nlohmann::json();
      std::ifstream file(filename);
      file >> localization;
    } catch (const std::exception &e) {
      std::cerr << "Cannot load localization: " << e.what() << std::endl;
    }
  }
};

const sf::Color Resource::focusedColor = []() { return sf::Color(0, 0, 0); }();

const sf::Color Resource::unfocusedColor = []() {
  return sf::Color(127, 127, 127);
}();

const sf::Color Resource::transparentColor = []() {
  return sf::Color(0, 0, 0, 0);
}();

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();

nlohmann::json Resource::localization = nlohmann::json();