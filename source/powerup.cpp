#include "powerup.h"
#include "main.h"
#define numv 20

Powerup::Powerup(float x, float y, char type)
{
    this->deleted = false;
    this->position = glm::vec3(x, y, 0);
    this->type = type;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    static GLfloat vertex_buffer_data_k[numv * 3 * 3];
    static GLfloat color_buffer_data_k[numv * 3 * 4]; 

    static GLfloat vertex_buffer_data_r[1 * 3 * 3];
    static GLfloat color_buffer_data_r[1 * 3 * 4];
    
    static GLfloat vertex_buffer_data_p[1 * 3 * 3];
    static GLfloat color_buffer_data_p[1 * 3 * 4];

    if(this->type == 'k'){
        for(int i = 0; i<numv; i++){
            float theta = 2 * glm::pi<float>() / numv;
            vertex_buffer_data_k[9*i] = 0.0f;
            vertex_buffer_data_k[9*i+1] = 0.0f;
            vertex_buffer_data_k[9*i+2] = 0.0f;

            vertex_buffer_data_k[9*i+3] = 0.2f * glm::cos(i*theta);
            vertex_buffer_data_k[9*i+4] = 0.2f * glm::sin(i*theta);
            vertex_buffer_data_k[9*i+5] = 0.0f;

            vertex_buffer_data_k[9*i+6] = 0.2f * glm::cos((i+1)*theta);
            vertex_buffer_data_k[9*i+7] = 0.2f * glm::sin((i+1)*theta);
            vertex_buffer_data_k[9*i+8] = 0.0f;
        }
        for(int i = 0; i<numv*3; i++){
            color_buffer_data_k[4*i] = 1.0f;
            color_buffer_data_k[4*i+1] = 0.0f;
            color_buffer_data_k[4*i+2] = 0.0f;
            color_buffer_data_k[4*i+3] = 1.0f;
        }
        this->object = create3DObject(GL_TRIANGLES, numv * 3, vertex_buffer_data_k, color_buffer_data_k, GL_FILL);
    }

    else if(this->type == 'r'){
        vertex_buffer_data_r[0] = 0.2f * glm::cos(11 * glm::pi<float>() / 6);
        vertex_buffer_data_r[1] = 0.2f * glm::sin(11 * glm::pi<float>() / 6);
        vertex_buffer_data_r[2] = 0;

        vertex_buffer_data_r[3] = 0.0f;
        vertex_buffer_data_r[4] = 0.2f;
        vertex_buffer_data_r[5] = 0;

        vertex_buffer_data_r[6] = 0.2f * glm::cos(7 * glm::pi<float>() / 6);
        vertex_buffer_data_r[7] = 0.2f * glm::sin(7 * glm::pi<float>() / 6);
        vertex_buffer_data_r[8] = 0;

        for(int i = 0; i < 3; i++){
            color_buffer_data_r[4*i] = 0.96f;
            color_buffer_data_r[4*i+1] = 0.87f;
            color_buffer_data_r[4*i+2] = 0.18f;
            color_buffer_data_r[4*i+3] = 1.0f;
        }

        this->object = create3DObject(GL_TRIANGLES, 1 * 3, vertex_buffer_data_r, color_buffer_data_r, GL_FILL);
    }

    else if(this->type == 'p'){
        GLfloat temp[] = {
            -0.1f, 0.1f, 0.0f,
            0.1f, 0.1f, 0.0f,
            -0.1f, -0.1f, 0.0f,
            0.1f, 0.1f, 0.0f,
            -0.1f, -0.1f, 0.0f,
            0.1f, -0.1f, 0.0f
        };

        /*
        for(int i = 0; i < 2 * 3; i++){
            color_buffer_data_p[3*i] = 0.0f;
            color_buffer_data_p[4*i+1] = 0.68f;
            color_buffer_data_p[4*i+2] = 0.30f;
            color_buffer_data_p[4*i+3] = 1.0f;
        }
        */

        GLfloat color_temp[] = {
            0.0f, 0.68f, 0.30f, 1.0f,
            0.0f, 0.68f, 0.30f, 1.0f,
            0.0f, 0.68f, 0.30f, 1.0f,
            0.0f, 0.68f, 0.30f, 1.0f,
            0.0f, 0.68f, 0.30f, 1.0f,
            0.0f, 0.68f, 0.30f, 1.0f  
        };
        this->object = create3DObject(GL_TRIANGLES, 2 * 3, temp, color_temp, GL_FILL);
    }

    else if(this->type == 'o'){
        GLfloat temp[] = {
            -0.1f, 0.1f, 0.0f,
            0.1f, 0.1f, 0.0f,
            -0.1f, -0.1f, 0.0f,
            0.1f, 0.1f, 0.0f,
            -0.1f, -0.1f, 0.0f,
            0.1f, -0.1f, 0.0f
        };

        GLfloat color_temp[] = {
            186./255., 105./255., 0., 1.,
            186./255., 105./255., 0., 1.,
            186./255., 105./255., 0., 1.,
            92./255., 52./255., 0., 1.,
            92./255., 52./255., 0., 1.,
            92./255., 52./255., 0., 1.
        };
        this->object = create3DObject(GL_TRIANGLES, 2 * 3, temp, color_temp, GL_FILL);
    }
    
}

void Powerup::draw(glm::mat4 VP)
{
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate(this->position); // glTranslatef
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Powerup::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Powerup::tick()
{
    //this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Powerup::bb()
{
    bounding_box_t bb;
    bb.x = (this->position).x;
    bb.y = (this->position).y;
    bb.height = 0.4f;
    bb.width = 0.4f;

    return bb;
}
