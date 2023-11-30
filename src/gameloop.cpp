#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>

#include "gameloop.hpp"

#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <vector>

#include "texture_manager.hpp"
#include "background_renderer.hpp"
#include "cannon.hpp"
#include "footsoldier.hpp"
#include "enemy_path_renderer.hpp"
#include "game.hpp"
#include "gamegrid.hpp"
#include "gameobject.hpp"
#include "map_tile_selector_renderer.hpp"
#include "testmapinfo.hpp"
#include "tower.hpp"

int GameLoop::Play() {
    int windowWidth = 20 * 64;
    int windowHeight = 12 * 64;
    
	// Toolbar dimensions
    int toolbarWidth = 68;
    int toolbarHeight = window_.getSize().y;

	TextureManager t = TextureManager();
	
	std::map<std::string, std::uint16_t> weaponNameIdMapping;
	weaponNameIdMapping["greencannon"] = 248;
	weaponNameIdMapping["redcannon"] = 249;

	GameGrid &grid = game_.GetGrid();
	/*
  // Renderers.
	BackgroundRenderer ikkuna(window_, grid);
	MapTileSelectorRenderer tileSelector(window_, grid);
	// For debug purposes. 
  	EnemyPathRenderer enemyPathRenderer(window_, grid);
	*/
    /* Renderers. */
    
	BackgroundRenderer ikkuna(window_, grid);
    /* For debug purposes.*/
    EnemyPathRenderer enemyPathRenderer(window_, grid);

    sf::View startView(sf::FloatRect(0, 0, windowWidth, windowHeight));
    sf::View applicationView(sf::FloatRect(0, 0, windowWidth, windowHeight));

    window_.setView(startView);

    // create startbutton
    sf::RectangleShape startButton(sf::Vector2f(200, 100));
    startButton.setFillColor(sf::Color(252, 213, 38));
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setOutlineThickness(4);

    // Center the button horizontally
    sf::FloatRect startButtonBounds = startButton.getLocalBounds();
    startButton.setOrigin(
	startButtonBounds.left + startButtonBounds.width / 2.0f, 0);
    startButton.setPosition(window_.getSize().x / 2.0f, 400);

    sf::Font font;
    if (!font.loadFromFile("../fonts/open-sans/OpenSans-Italic.ttf")) {
		std::cout << "Error in font loading" << std::endl;
		return -1;
    }

    // Add text for the startButton
    sf::Text playText;
    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(20);
    playText.setFillColor(sf::Color::White);

    // Center the text within the button
    sf::FloatRect playBounds = playText.getLocalBounds();
    playText.setOrigin(playBounds.left + playBounds.width / 2.0f,
		       playBounds.top + playBounds.height / 2.0f);
    playText.setPosition(
	startButton.getPosition().x,
	startButton.getPosition().y + startButton.getSize().y / 2.0f);

    // Create title for the game.
    sf::Text title;
    title.setString("Clash Of Armies");
    title.setCharacterSize(60);
    title.setFont(font);
    title.setFillColor(sf::Color::Red);

    // Center the text horizontally
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.left + titleBounds.width / 2.0f, 0);
    title.setPosition(window_.getSize().x / 2.0f, 100);

    // Create a vertical toolbar on the right side using a rectangle
    sf::RectangleShape toolbar(sf::Vector2f(toolbarWidth, toolbarHeight));
    toolbar.setPosition(window_.getSize().x - toolbarWidth, 0);
    toolbar.setFillColor(sf::Color(128, 128, 128));

    // Towers to protect base: Cannon, MissileLauncher and FighterPlane
    std::vector<std::string> towersVec = {"../rsrc/tiles/weapons/248.png",
					  					  "../rsrc/tiles/weapons/249.png"};

    // Add buttons to the toolbar (tower buttons)
    sf::Texture cannon, bigCannon;

    // Load tank texture
    if (!cannon.loadFromFile(towersVec[0])) {
		std::cout << "cannon loadFromFile problem." << std::endl;
		return -1;
    }

    // Load bigCannon texture
    if (!bigCannon.loadFromFile(towersVec[1])) {
		std::cout << "bigCannon loadFromFile problem." << std::endl;
		return -1;
    }

    // Creating sprites for the towers.
    sf::Sprite cannonSprite(cannon);
    sf::Sprite bigCannonSprite(bigCannon);

    // Set positions of the buttons
    cannonSprite.setPosition(window_.getSize().x - toolbarWidth + 3,
			     60);  // Toolbar x-coordinate + 20

    // 20 + tank x-coordinate + tank width + 20
    bigCannonSprite.setPosition(window_.getSize().x - toolbarWidth + 3, 140);

    bool startButtonClicked = false;

    // TODO: Experimental
	

    /* TODO: Use std::unique. */
    // std::optional<Tower *> towerClicked;
	
	
    std::optional<Cannon*> towerClicked;
    /* Tower id. */
    /* TODO: Create mapping from tower id's to towers and vice versa. */
    std::optional<std::uint16_t> selectedTower;
    MapTileSelectorRenderer tileSelector(window_, grid, t, selectedTower);
    std::optional<int> towerType;


	// TODO: Replace with proper solution.
	std::vector<sf::Sprite> gameSprites;

    // Load window
    while (window_.isOpen()) {
		sf::Event event;

		while (window_.pollEvent(event)) {
			switch (event.type) {
				case (sf::Event::Closed):
					if (!game_.Objects().empty()) {
						// Release dynamically allocated memory
						int i = 0;
						while (i < game_.Objects().size()) {
							GameObject* curr = game_.Objects().at(i);
							game_.DeleteObject(curr);
							delete curr;
							i++;
						}
					}
					window_.close();
					break;

				// Button click handling
				case (sf::Event::MouseButtonPressed):
					if (event.mouseButton.button == sf::Mouse::Left) {
						sf::Vector2f mousePos = window_.mapPixelToCoords(
							sf::Mouse::getPosition(window_));

						if (startButton.getGlobalBounds().contains(mousePos)) {
							startButtonClicked = true;
							// Switch to application view
							window_.setView(applicationView);
						}
						
						if (startButtonClicked) {
							if (cannonSprite.getGlobalBounds().contains(mousePos)) {
								std::cout << "cannon button was clicked." << std::endl;
								
								// Cannon(int radius, int fireRate, int price, sf::Sprite sprite, Game& game, Pos position)
								// TODO: Check the position of the Cannon-instance.
								towerClicked = new Cannon(10, 10, 18, cannonSprite, game_, Pos{ 100, 100 });
								towerType = 1;
								if (selectedTower.has_value() && (selectedTower.value() == weaponNameIdMapping["greencannon"])) {
									selectedTower.reset();
								} else {
									selectedTower = weaponNameIdMapping["greencannon"];
								}

								selectedTower = weaponNameIdMapping["greencannon"];
							// TODO: When clicking on this button the
							// program should create a new tank instance.
							} else if (bigCannonSprite.getGlobalBounds().contains(mousePos)) {
								std::cout << "bigCannon button was clicked." << std::endl;
								// TODO: Check the position of the Cannon-instance.
								// Cannon(int radius, int fireRate, int price, sf::Sprite sprite, Game& game, Pos position)
								towerClicked = new Cannon(10, 10, 20, cannonSprite, game_, Pos{ 100, 60 });
								towerType = 2;
								if (selectedTower.has_value() && (selectedTower.value() == weaponNameIdMapping["redcannon"])) {
									selectedTower.reset();
								} else {
									selectedTower = weaponNameIdMapping["redcannon"];
								}
								
								selectedTower = weaponNameIdMapping["redcannon"];

							} else {
								if (towerClicked.has_value() && grid.TileAtCoordinate(mousePos.x / 64, mousePos.y / 64).isEmpty()) {
									std::cout << "placing tower" << std::endl;
									Cannon* can = towerClicked.value(); // dynamic_cast<Cannon*>(towerClicked.value());
									sf::Sprite canSprite;
									if (towerType.value() == 1) {
										canSprite.setTexture(cannon);
									} else if (towerType.value() == 2) {
										canSprite.setTexture(bigCannon);
									}
									int posX = mousePos.x / 64;
									int posY = mousePos.y / 64;
									std::cout << "canSpritePos(before) = (" << canSprite.getPosition().x << ", " << canSprite.getPosition().y << ")" << std::endl;
									canSprite.setPosition(posX * 64, posY * 64);
									std::cout << "canSpritePos(after) = (" << canSprite.getPosition().x << ", " << canSprite.getPosition().y << ")" << std::endl;
									grid.TileAtCoordinate(posX, posY).occupy();

									gameSprites.push_back(canSprite);
									// game_.AddObject(can); // game_.AddObject(canSprite);
									towerClicked.reset();
									selectedTower.reset();
									towerType.reset();


								} 
							}
						}
					}
					break;

				case (sf::Event::MouseButtonReleased):
					break;

				// Mouse hover event
				case (sf::Event::MouseMoved):
					// Mouse position in window coordinates.
					sf::Vector2f mousePos = window_.mapPixelToCoords(
					sf::Mouse::getPosition(window_));

					if (startButtonClicked) {
					} else {
						sf::Color color = startButton.getFillColor();
						if (startButton.getGlobalBounds().contains(mousePos)) {
							// change opacity of the hovered startButton
							color.a = 128;
							startButton.setFillColor(color);
						} else {
							// Restore opacity
							color.a = 255;
							startButton.setFillColor(color);
						}
					}
					break;
			}
		}
		
		window_.clear(sf::Color(200, 200, 200));
		
		if (!startButtonClicked) {
			window_.draw(startButton);
			window_.draw(playText);
			window_.draw(title);
		} else {
			// Draw the tiles
			ikkuna.Draw();

			// Draw the tile selector.
			tileSelector.Draw();

			// Debug: draw the enemy path.
			enemyPathRenderer.Draw();
			/*
			for (auto it : game_.Objects()) {
				window_.draw(it->GetSprite());
			}*/
			int i = 0;
			while (i < gameSprites.size()) {
				window_.draw(gameSprites.at(i));
				i++;
			}
			
			// Draw the toolbar and buttons inside it.
			window_.draw(toolbar);
			window_.draw(cannonSprite);
			window_.draw(bigCannonSprite);
		}

		window_.display();
		
    }
    return 0;
}
