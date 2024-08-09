#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Resource {
public:
  static const int characterSize = 24;
  static const int lineSpacing = characterSize / 2;

  static const sf::Font defaultFont;

  static nlohmann::json localization;
  static nlohmann::json userSettings;

  static const sf::Color focusedColor;
  static const sf::Color unfocusedColor;
  static const sf::Color transparentColor;

  static sf::Texture background;

  static sf::Music *loadBackgroundMusic() {
    sf::Music *music = new sf::Music();
    if (!music->openFromFile("assets/music/hound-dog-jazz.mp3")) {
      throw std::runtime_error("Cannot load music");
    }

    return music;
  }

  static std::vector<std::string> listLocalizations() {
    std::string directoryPath = "assets/localizations";
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
            jsonFiles.push_back(directoryPath + "/" + path.filename().string());
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

  static nlohmann::json loadJson(const std::string &filename) {
    nlohmann::json j;

    try {
      j = nlohmann::json();
      std::ifstream file(filename);
      file >> j;
    } catch (const std::exception &e) {
      std::cerr << "Cannot load localization: " << filename << " " << e.what()
                << std::endl;
    }

    return j;
  }

  static void writeJson(const std::string &filename, nlohmann::json j) {
    try {
      std::ofstream file(filename);
      file << j;
    } catch (const std::exception &e) {
      std::cerr << "Cannot write localization: " << filename << " " << e.what()
                << std::endl;
    }
  }
};

const sf::Font Resource::defaultFont = []() {
  sf::Font font;
  if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    throw std::runtime_error("Cannot load font");
  }

  return font;
}();

nlohmann::json Resource::userSettings =
    Resource::loadJson("assets/settings.json");
nlohmann::json Resource::localization =
    Resource::loadJson(Resource::userSettings["localization"]);

const sf::Color Resource::focusedColor = []() { return sf::Color(0, 0, 0); }();

const sf::Color Resource::unfocusedColor = []() {
  return sf::Color(95, 95, 95);
}();

const sf::Color Resource::transparentColor = []() {
  return sf::Color(0, 0, 0, 0);
}();

sf::Texture Resource::background = []() {
  sf::Texture texture;
  if (!texture.loadFromFile("assets/images/background.png")) {
    throw std::runtime_error("Cannot load texture");
  }

  return texture;
}();

static std::wstring utf8_to_wstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  return converter.from_bytes(str);
}
