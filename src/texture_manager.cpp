
#include "texture_manager.hpp"

#include "path.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <filesystem>

const std::string textureFilePathEnding = ".png";

TextureManager::TextureManager() {
	/* Iterate over all the tile subdirectories. */
	for (auto &entry : std::filesystem::directory_iterator(TILE_ROOT_DIRECTORY)) {
		if (entry.is_directory()) {
			for (auto &subdirEntry : std::filesystem::directory_iterator(entry)) {
				std::string filename = subdirEntry.path().filename();
				std::string filenameEnding = filename.substr(filename.size() - textureFilePathEnding.size());
				if (subdirEntry.is_regular_file() && filenameEnding == textureFilePathEnding) {
					try {
						std::uint16_t textureId = std::stoi(filename.substr(0, filename.size() - textureFilePathEnding.size()));
						sf::Texture t;
						t.loadFromFile(subdirEntry.path());
						textures_[textureId] = t;
						std::cout << "Successfully loaded texture '" << filename << "'." << std::endl;
					}
					catch (std::exception /* &e */) {
						/* Ignore errors. Assume that they are non fatal and come from non texture files that happen to be there. E.g. .DS_Store etc.*/
					}
				}
			}
		}
	}

}


sf::Texture &TextureManager::GetTexture(std::uint16_t textureId) {
	try {
		return textures_.at(textureId);
	}
	catch (std::out_of_range /* &e */) {
		throw TextureDoesNotExistError();
	}
}


