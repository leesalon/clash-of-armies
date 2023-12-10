#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <vector>

#include "button_text.hpp"
#include "game.hpp"
#include "gamegrid.hpp"
#include "gameloop.hpp"
#include "menu_home.hpp"
#include "menu_level.hpp"
#include "music_manager.hpp"
#include "path.hpp"
#include "time.hpp"
#include "weapon_toolbar_renderer.hpp"

int main() {
  int windowWidth = 20 * 64;
  int windowHeight = 12 * 64;

  sf::RenderWindow window(
      sf::VideoMode(windowWidth + WeaponToolbarWidthPx, windowHeight),
      "Clash Of Armies");
  window.setFramerateLimit(ticksPerSecond);

  std::vector<std::pair<int, std::string>> levels =
      readLevels("../rsrc/level.csv");

  int page = 0;
  int gameLevel = 0;
  MusicManager musicManager;
  MenuLevel menuLevel(levels);
  MenuHome menuHome(musicManager);

  std::pair<int, int> resp;

  while (window.isOpen()) {
    switch (page) {
      case 1:
        // Level Selection Screen
        resp = menuLevel.run(window);
        page = resp.first;
        gameLevel = resp.second;

        break;
      case 2: {
        auto levelIt = std::find_if(levels.begin(), levels.end(),
                                    [gameLevel](const auto& level) {
                                      return level.first == gameLevel;
                                    });
        if (levelIt == levels.end()) {
          std::cerr << "Error: gameLevel " << gameLevel << " not found"
                    << std::endl;
          return -1;
        }

        std::cout << "Game Start: gameLevel is " << gameLevel << std::endl;
        std::string mapFilename = levelIt->second;
        std::string fullMapPath = "../rsrc/maps/" + mapFilename;

        MapInfo mapInfo = ParseMap(fullMapPath);
        GameGrid gg = GameGrid(mapInfo);
        Game game = Game(gg, gameLevel, mapInfo);
        GameLoop loop(window, game);

        loop.Play();

        // Getting outcome of loop
        std::uint8_t exitCode = game.GetExitCode();
        if (exitCode == 0 || exitCode == 3) {
          page = 1;
        } else if (exitCode == 1) {
          page = 3;
        } else {
          page = 4;
        }
        break;
      }
      // Game won page
      case 3: {
        auto onClick = [&page]() { page = 0; };
        sf::Font font;
        if (!font.loadFromFile("../fonts/pixieboy.ttf")) {
          std::cout << "Error in font loading" << std::endl;
        }

        ButtonText exitButton(
            sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.5f),
            sf::Vector2f(500, 350), onClick, "WOW you won very nice gg ez",
            font);

        sf::Event event;
        sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));

        while (page == 3) {
          while (window.pollEvent(event)) {
            switch (event.type) {
              case sf::Event::Closed:
                window.close();
                break;

              case sf::Event::MouseMoved:
                mousePos =
                    window.mapPixelToCoords(sf::Mouse::getPosition(window));
                exitButton.handleHover(mousePos);
                break;

              case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                  mousePos =
                      window.mapPixelToCoords(sf::Mouse::getPosition(window));
                  exitButton.handleClick(mousePos);
                }
                break;
              default:
                break;
            }
          }
          exitButton.draw(window);
          window.display();
        }
      }
      // Game lose page
      case 4: {
        auto onClick = [&page]() { page = 0; };
        sf::Font font;
        if (!font.loadFromFile("../fonts/pixieboy.ttf")) {
          std::cout << "Error in font loading" << std::endl;
        }

        ButtonText exitButton(
            sf::Vector2f(windowWidth / 2.0f, windowHeight / 2.5f),
            sf::Vector2f(500, 350), onClick, "Get Good kid", font);

        sf::Event event;
        sf::Vector2f mousePos =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));

        while (page == 4) {
          while (window.pollEvent(event)) {
            switch (event.type) {
              case sf::Event::Closed:
                window.close();
                break;

              case sf::Event::MouseMoved:
                mousePos =
                    window.mapPixelToCoords(sf::Mouse::getPosition(window));
                exitButton.handleHover(mousePos);
                break;

              case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                  mousePos =
                      window.mapPixelToCoords(sf::Mouse::getPosition(window));
                  exitButton.handleClick(mousePos);
                }
                break;
              default:
                break;
            }
          }
          window.clear();
          exitButton.draw(window);
          window.display();
        }
      }
      default:
        // Main Menu Screen
        if (page < 0) {
          return page;
        }

        page = menuHome.run(window);
        break;
    }
  }

  return 0;
}
