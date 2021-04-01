#include "enemy.h"
#include "main.h"
#include <queue>
#include <utility>

#define INF 1e9;

Enemy::Enemy(float x, float y, color_t color)
{
    this->deleted = false;
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.04;
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
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,

        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,

        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,

        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,

        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,

        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        0.17f, 0.60f, 0.95f, 1.0f,
        };

    for (int i = 0; i < 10 * 3 * 3; i++)
    {
        vertex_buffer_data[i] /= 2.0f;
    }

    this->object = create3DObject(GL_TRIANGLES, 10 * 3, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Enemy::draw(glm::mat4 VP)
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

void Enemy::set_position(float x, float y)
{
    this->position = glm::vec3(x, y, 0);
}

void Enemy::tick()
{
    //this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Enemy::bb()
{
    bounding_box_t bb;
    bb.x = (this->position).x;
    bb.y = (this->position).y;
    bb.height = 0.5f;
    bb.width = 0.35f;

    return bb;
}

glm::vec2 Enemy::findPlayer(char maze[SIZE][SIZE], int dist[SIZE][SIZE][SIZE][SIZE], float player_x, float player_y)
{
    // Returns direction in which enemy should move

    int player_j = (int)player_x + (int)(0.5 + player_x - (float)((int)player_x));
    int player_i = (int)player_y + (int)(0.5 + player_y - (float)((int)player_y));

    int self_j = (int)(this->position).x + (int)(0.5 + (this->position).x - (float)((int)(this->position).x));
    int self_i = (int)(this->position).y + (int)(0.5 + (this->position).y - (float)((int)(this->position).y));

    if(self_i == player_i && self_j == player_j)
    {
        //Same box
        return glm::normalize(glm::vec2(player_x - (this->position).x, player_y - (this->position).y));
    }

    int dir_i, dir_j;
    dir_i = -1; dir_j = 0;

    if(dist[player_i][player_j][self_i + 1][self_j] < dist[player_i][player_j][self_i + dir_i][self_j + dir_j]){
        dir_i = 1;
        dir_j = 0;
    }
    if(dist[player_i][player_j][self_i][self_j + 1] < dist[player_i][player_j][self_i + dir_i][self_j + dir_j]){
        dir_i = 0;
        dir_j = 1;
    }
    if(dist[player_i][player_j][self_i][self_j - 1] < dist[player_i][player_j][self_i + dir_i][self_j + dir_j]){
        dir_i = 0;
        dir_j = -1;
    }
    
    return glm::vec2((float)dir_j, (float)dir_i);
}
