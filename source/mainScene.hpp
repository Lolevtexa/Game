#pragma once
#include "widgets/activatable/clickable/button.hpp"
#include "widgets/activatable/clickable/radioButton.hpp"
#include "widgets/activatable/clickable/slider.hpp"
#include "widgets/activatable/horizontalWigets.hpp"
#include "widgets/activatable/number.hpp"
#include "widgets/activatable/text.hpp"
#include "widgets/constantable/image.hpp"

class MainScene : public sf::Drawable {
private:
  const float menuButtonRatio = 1.f / 4.f;
  const float settingsButtonRatio = 1.f / 3.f;

  int menuButtonWidth;
  int settingsButtonWidth;
  int buttonHeight = 50;
  int buttonIndent = 10;

  std::vector<Button *> menuButtons;

  int settingsPage = 0;
  std::vector<std::vector<Button *>> settingsButtons{4};

  std::vector<AText *> textsByKey;
  std::vector<ANumber *> numbers;
  Slider *musicVolumeSliders;

  sf::Music *backgroundMusic = Resource::loadBackgroundMusic();
  Image background;

public:
  template <typename Exit, typename SetFullscreen, typename SetWindowed,
            typename SetTitle>
  MainScene(Exit exit, SetFullscreen setFullscreen, SetWindowed setWindowed,
            SetTitle setTitle, sf::Vector2u windowSize)
      : menuButtonWidth(windowSize.x * menuButtonRatio),
        settingsButtonWidth(windowSize.x * settingsButtonRatio),
        background(&Resource::background) {
    backgroundMusic->setVolume(Resource::userSettings["music volme"]);
    backgroundMusic->setLoop(true);
    backgroundMusic->play();

    background.setBound(0, 0, static_cast<float>(windowSize.x),
                        static_cast<float>(windowSize.y), 0);

    addMainButton(std::vector<std::string>({"buttons", "new game"}),
                  [this]() { setSettingsPage(1); });
    addMainButton(std::vector<std::string>({"buttons", "load game"}),
                  [this]() { setSettingsPage(2); });
    addMainButton(std::vector<std::string>({"buttons", "settings"}),
                  [this]() { setSettingsPage(3); });
    addMainButton(std::vector<std::string>({"buttons", "exit"}), exit);
    updateMainButtonBounds();

    addSettingsTextButton(1,
                          std::vector<std::string>({"buttons", "start game"}),
                          []() { std::cout << "Start game" << std::endl; });
    addSettingsTextButton(1, std::vector<std::string>({"buttons", "back"}),
                          [this]() { setSettingsPage(0); });

    addSettingsTextButton(2, std::vector<std::string>({"buttons", "back"}),
                          [this]() { setSettingsPage(0); });

    addSettingsSliderButton(
        3, std::vector<std::string>({"buttons", "music volume"}),
        musicVolumeSliders,
        [this]() {
          Resource::userSettings["music volme"] =
              musicVolumeSliders->getValue();
        },
        Resource::userSettings["music volme"]);
    addSettingsRadioButton(
        3, std::vector<std::string>({"buttons", "screen mode"}),
        std::vector<std::vector<std::string>>(
            {{"buttons", "windowed"}, {"buttons", "fullscreen"}}),
        {setWindowed, setFullscreen});
    addSettingsRadioButton(3, std::vector<std::string>({"buttons", "language"}),
                           getLocalizationsNames(),
                           getLocalizationsFunc([this, setTitle]() {
                             updateLocalization();
                             setTitle();
                           }));
    addSettingsTextButton(3, std::vector<std::string>({"buttons", "back"}),
                          [this]() { setSettingsPage(0); });
  }

  ~MainScene() {
    while (menuButtons.size()) {
      delete *(menuButtons.rbegin());
      menuButtons.pop_back();
    }

    for (auto &settingsButton : settingsButtons) {
      while (settingsButton.size()) {
        delete *(settingsButton.rbegin());
        settingsButton.pop_back();
      }
    }

    backgroundMusic->stop();
  }

  void eventProcessing(sf::Event event) {
    for (auto &button : menuButtons) {
      button->eventProcessing(event);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->eventProcessing(event);
    }

    if (event.type == sf::Event::Resized) {
      background.setBound(0, 0, static_cast<float>(event.size.width),
                          static_cast<float>(event.size.height), 0);
      menuButtonWidth = event.size.width / 4;
      settingsButtonWidth = event.size.width / 2;
    }
  }

  void update() {
    for (auto &button : menuButtons) {
      button->update();
    }

    for (auto &button : settingsButtons[settingsPage]) {
      button->update();
    }

    updateMainButtonBounds();
    updateSettingsButtonsBound();
    backgroundMusic->setVolume(musicVolumeSliders->getValue());
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(background, states);

    for (auto &button : menuButtons) {
      target.draw(*button, states);
    }

    for (auto &button : settingsButtons[settingsPage]) {
      target.draw(*button, states);
    }
  }

private:
  void setSettingsPage(int page) {
    settingsPage = settingsPage == page ? 0 : page;
  }

  void addMainButton(const std::vector<std::string> &localizationKeys,
                     std::function<void()> func) {
    AText *text = new AText(localizationKeys);
    textsByKey.emplace_back(text);
    menuButtons.emplace_back(new Button(func, {text}));
  }

  void addMainButton(const std::wstring &text, std::function<void()> func) {
    AText *textButton = new AText(text);
    menuButtons.emplace_back(new Button(func, {textButton}));
  }

  void updateMainButtonBounds() {
    for (int i = 0; i < menuButtons.size(); i++) {
      menuButtons[i]->setBound(buttonIndent,
                               buttonIndent + i * (buttonHeight + buttonIndent),
                               menuButtonWidth, buttonHeight, buttonIndent);
    }
  }

  void addSettingsTextButton(int page, const std::vector<std::string> &textKeys,
                             std::function<void()> func) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);
    settingsButtons[page].emplace_back(new Button(func, {text}));
  }

  void addSettingsTextButton(int page, const std::wstring &text,
                             std::function<void()> func) {
    AText *textButton = new AText(text);
    settingsButtons[page].emplace_back(new Button(func, {textButton}));
  }

  void
  addSettingsRadioButton(int page, const std::vector<std::string> textKeys,
                         const std::vector<std::vector<std::string>> &textsKeys,
                         const std::vector<std::function<void()>> &funcs) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);

    std::vector<Button *> buttons;
    for (int i = 0; i < textsKeys.size(); i++) {
      AText *text = new AText(textsKeys[i]);
      textsByKey.emplace_back(text);
      buttons.emplace_back(new Button(funcs[i], {text}));
    }

    settingsButtons[page].emplace_back(
        new RadioButton([]() {}, {text}, buttons));
  }

  void addSettingsRadioButton(int page, const std::vector<std::string> textKeys,
                              const std::vector<std::wstring> &textsKeys,
                              const std::vector<std::function<void()>> &funcs) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);

    std::vector<Button *> buttons;
    for (int i = 0; i < textsKeys.size(); i++) {
      AText *text = new AText(textsKeys[i]);
      textsByKey.emplace_back(text);
      buttons.emplace_back(new Button(funcs[i], {text}));
    }

    settingsButtons[page].emplace_back(
        new RadioButton([]() {}, {text}, buttons));
  }

  void addSettingsSliderButton(int page, std::vector<std::string> textKeys,
                               Slider *&slider, std::function<void()> func,
                               int defaultValue = 50) {
    AText *text = new AText(textKeys);
    textsByKey.emplace_back(text);

    slider = new Slider(defaultValue);

    std::function<int()> getValue = [&slider]() { return slider->getValue(); };
    ANumber *number = new ANumber(getValue);
    numbers.emplace_back(number);

    settingsButtons[page].emplace_back(
        new Button(func, {new AHorizontalWigets({text, number}), slider}));
  }

  void updateSettingsButtonsBound() {
    for (int i = 0; i < settingsButtons.size(); i++) {
      int deltaY = 0;
      for (int j = 0; j < settingsButtons[i].size(); j++) {
        settingsButtons[i][j]->setBound(
            buttonIndent + menuButtonWidth + buttonIndent,
            buttonIndent + deltaY, settingsButtonWidth, buttonHeight,
            buttonIndent);
        deltaY += settingsButtons[i][j]->getBound().getSize().y + buttonIndent;
      }
    }
  }

  template <typename Func>
  std::vector<std::function<void()>>
  getLocalizationsFunc(Func updateLocalization) {
    std::vector<std::function<void()>> loadLanguageFuncs;
    for (auto &langugage : Resource::listLocalizations()) {
      loadLanguageFuncs.emplace_back([langugage, updateLocalization]() {
        Resource::localization = Resource::loadJson(langugage);
        Resource::userSettings["localization"] = langugage;
        updateLocalization();
      });
    }

    return loadLanguageFuncs;
  }

  std::vector<std::wstring> getLocalizationsNames() {
    std::vector<std::wstring> localizationsNames;
    std::vector<std::string> listLocalizations = Resource::listLocalizations();

    for (auto &langugage : listLocalizations) {
      nlohmann::json localization = Resource::loadJson(langugage);
      localizationsNames.emplace_back(
          utf8_to_wstring(localization["language"]));
    }

    return localizationsNames;
  }

  void updateLocalization() {
    for (auto &text : textsByKey) {
      text->resetString();
    }

    updateMainButtonBounds();
    updateSettingsButtonsBound();
  }
};
