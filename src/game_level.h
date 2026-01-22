#pragma once

#include <vector>

#include "game_object.h"


class GameLevel {
public:
    std::vector<GameObject> bricks {0};

    GameLevel();

    void load(const char *filename, unsigned int level_width, unsigned int level_height);

    void draw(SpriteRenderer &renderer);

    bool isCompleted();

private:
    void init(std::vector<std::vector<unsigned int>> tile_data, unsigned int level_width, unsigned int level_height);
};
