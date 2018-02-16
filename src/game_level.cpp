#include "game_level.h"

#include <vector>
#include <fstream>
#include <sstream>

GameLevel::GameLevel() {
}

void GameLevel::load(const GLchar *filename, GLuint level_width, GLuint level_height) {
    bricks.clear();

    GLuint tile_code;
    GameLevel level;

    std::string line;
    std::ifstream fstream {filename};
    std::vector<std::vector<GLuint>> tile_data;

    if (fstream) {
        while (std::getline(fstream, line)) {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tile_code) {
                row.push_back(tile_code);
            }
            tile_data.push_back(row);
        }

        if (tile_data.size() > 0) {
            init(tile_data, level_width, level_height);
        }
    }
}

void GameLevel::draw(SpriteRenderer &renderer) {
    for (auto &tile : bricks) {
        if (!tile.is_destroyed) {
            tile.draw(renderer);
        }
    }
}

GLboolean GameLevel::isCompleted() {
    for (auto &tile : bricks) {
        if (!tile.is_solid && !tile.is_destroyed) {
            return GL_FALSE;
        }
    }
    return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tile_data, GLuint level_width, GLuint level_height) {
    GLuint height = tile_data.size();
    GLuint width = tile_data[0].size();
    GLfloat unit_width = level_width / static_cast<GLfloat>(width);
    GLfloat unit_height = level_height / height;

    for (GLuint y = 0; y < height; y++) {
        for (GLuint x = 0; x < width; x++) {
            if (tile_data[y][x] == 1) { // Solid
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                GameObject obj {
                    pos, size,
                    ResourceManager::getTexture("block_solid"),
                    glm::vec3(0.8f, 0.8f, 0.7f)};

                obj.is_solid = GL_TRUE;

                bricks.push_back(obj);
            }
            else if (tile_data[y][x] > 1) {
                glm::vec3 color = glm::vec3(1.0f);
                if (tile_data[y][x] == 2) {
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                } else if (tile_data[y][x] == 3) {
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                } else if (tile_data[y][x] == 4) {
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                } else if (tile_data[y][x] == 5) {
                    color = glm::vec3(1.0f, 0.5f, 0.0f);
                }

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);

                bricks.push_back(GameObject {pos, size, ResourceManager::getTexture("block"), color});
            }
        }
    }
}

