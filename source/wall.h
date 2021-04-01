#include "main.h"

#ifndef WALL_H
#define WALL_H

class Wall
{
public:
    Wall() {}
    Wall(float x, float y, char type);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bb();
    char type;

private:
    VAO *object;
};

#endif 
