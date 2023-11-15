
#pragma once

#include <cstdint>
#include <vector>

enum class TileType {
    Path,
    Ground,
    Water,
};

class Tile {
private:
    TileType type_;
	std::uint16_t id_;

public:
    Tile(TileType type, std::uint16_t id) : type_(type), id_(id) {}
    TileType type() const;
    
	std::uint16_t Id() const { return id_; }

    // TODO: Do we need this method?
    bool isEmpty() const;

};
