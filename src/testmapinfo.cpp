
#include "testmapinfo.hpp"
#include "map_parser.hpp"
#include "pos.hpp"

#include "vec2d.hpp"
#include <cstdint>
#include <tuple>
#include <vector>

const std::uint32_t mapWidth = 32;
const std::uint32_t mapHeight = 12;

MapInfo testMapInfoObject1() {
	/* Create the background tiles. */
	std::vector<std::vector<std::uint16_t>> backgroundTiles;
	for (std::uint32_t j = 0; j < mapHeight; j++) {
		std::vector<uint16_t> row;
		for (std::uint32_t i = 0; i < mapWidth; i++) {
			/* Grass. */
			row.push_back(23);
		}
		backgroundTiles.push_back(row);
	}

	/* Add path tiles. */
	for (std::uint16_t j = 3; j <= 4; j++) {
		for (std::uint16_t i = 0; i < 20; i++) {
			backgroundTiles[j][i] = 92;
		}
	}

	/* TODO: Create better enemy path. */
	std::vector<Vec2D> enemyPath;
	enemyPath.push_back(Vec2D{ Pos{ 0, 4 * 64 }, Pos{ 1280, 4 * 64 } });

	return MapInfo{ 64, 20, 12, backgroundTiles, enemyPath };
}


