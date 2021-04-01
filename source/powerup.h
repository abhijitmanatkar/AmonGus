#include "main.h"
#include "grid.h"

#ifndef POWERUP_H
#define POWERUP_H


class Powerup {
public:
    Powerup() {}
    Powerup(float x, float y, char type);
    char type;
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bb();
    bool deleted;
private:
    VAO *object;
};

#endif // BALL_H
