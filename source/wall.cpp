#include "wall.h"
#include "main.h"

Wall::Wall(float x, float y, char type)
{
    this->type = type;
    this->position = glm::vec3(x, y, 0);

    static GLfloat vertex_buffer_data[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f};

    static GLfloat color_buffer_data_x[] = {
        20./255., 12./255., 0., 1.,
        20./255., 12./255., 0., 1.,
        20./255., 12./255., 0., 1.,
        20./255., 12./255., 0., 1.,
        20./255., 12./255., 0., 1.,
        20./255., 12./255., 0., 1.};

    static GLfloat color_buffer_data_e[] = {
        0., 50./255., 0., 166./255.,
        0., 50./255., 0., 166./255.,
        0., 50./255., 0., 166./255.,
        0., 50./255., 0., 166./255.,
        0., 50./255., 0., 166./255.,
        0., 50./255., 0., 166./255.};

    for (int i = 0; i < 36; i++)
    {
        vertex_buffer_data[i] /= 1.0f;
    }

    if(this->type == 'x')
        this->object = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data, color_buffer_data_x, GL_FILL);
    else
        this->object = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data, color_buffer_data_e, GL_FILL);   
}

void Wall::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef

    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Wall::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Wall::bb()
{
    bounding_box_t bb;
    bb.x = (this->position).x;
    bb.y = (this->position).y;
    bb.height = 1.0f;
    bb.width = 1.0f;

    return bb;
}
