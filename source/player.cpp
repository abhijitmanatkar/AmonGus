#include "player.h"
#include "main.h"

Player::Player(float x, float y, color_t color)
{
    this->health = 20.0;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    float theta = glm::pi<float>() / 6;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static GLfloat vertex_buffer_data[] = {
        -0.35f, 0.5f, 0.0f,
        0.35f, 0.5f, 0.0f,
        -0.35f, -0.5f, 0.0f,
        0.35f, 0.5f, 0.0f,
        -0.35f, -0.5f, 0.0f,
        0.35f, -0.5f, 0.0f,
        // Inner rectangle
        -0.20f, 0.0f, 0.0f,
        -0.20f, 0.35f, 0.0f,
        0.20f, 0.35f, 0.0f,
        0.20f, 0.0f, 0.0f,
        0.20f, 0.35f, 0.0f,
        -0.20f, 0.0f, 0.0f,

        //Semicircle
        0.0f, 0.5f, 0.0f,
        0.35f*glm::cos(0*theta), 0.4f + 0.35f*glm::sin(0*theta), 0.0f,
        0.35f*glm::cos(1*theta), 0.4f + 0.35f*glm::sin(1*theta), 0.0f,

        0.0f, 0.5f, 0.0f,
        0.35f*glm::cos(theta), 0.4f + 0.35f*glm::sin(theta), 0.0f,
        0.35f*glm::cos(2*theta), 0.4f + 0.35f*glm::sin(2*theta), 0.0f,

        0.0f, 0.5f, 0.0f,
        0.35f*glm::cos(2*theta), 0.4f + 0.35f*glm::sin(2*theta), 0.0f,
        0.35f*glm::cos(3*theta), 0.4f + 0.35f*glm::sin(3*theta), 0.0f,

        0.0f, 0.5f, 0.0f,
        0.35f*glm::cos(3*theta), 0.4f + 0.35f*glm::sin(3*theta), 0.0f,
        0.35f*glm::cos(4*theta), 0.4f + 0.35f*glm::sin(4*theta), 0.0f,

        0.0f, 0.5f, 0.0f,
        0.35f*glm::cos(4*theta), 0.4f + 0.35f*glm::sin(4*theta), 0.0f,
        0.35f*glm::cos(5*theta), 0.4f + 0.35f*glm::sin(5*theta), 0.0f,

        0.0f, 0.5f, 0.0f,
        0.35f*glm::cos(5*theta), 0.4f + 0.35f*glm::sin(5*theta), 0.0f,
        0.35f*glm::cos(6*theta), 0.4f + 0.35f*glm::sin(6*theta), 0.0f,

        };

    static GLfloat color_buffer_data[] = {
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,

        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        0.96f, 0.14f, 0.14f, 1.0f,
        };

    for (int i = 0; i < 10 * 3 * 3; i++)
    {
        vertex_buffer_data[i] /= 2.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 10 * 3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Player::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    glm::mat4 rotate = glm::rotate((float)(this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Player::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Player::tick()
{
    //this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Player::bb()
{
    bounding_box_t bb;
    bb.x = (this->position).x;
    bb.y = (this->position).y;
    bb.height = 0.5f;
    bb.width = 0.35f;

    return bb;
}
