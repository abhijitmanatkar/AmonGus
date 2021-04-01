#include "main.h"
#include "grid.h"

#ifndef ENEMY_H
#define ENEMY_H


class Enemy {
public:
    Enemy() {}
    Enemy(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bounding_box_t bb();
    glm::vec2 findPlayer(char maze[SIZE][SIZE], int dist[SIZE][SIZE][SIZE][SIZE], float player_x, float player_y);
    bool deleted;
private:
    VAO *object;
};

#endif // BALL_H
