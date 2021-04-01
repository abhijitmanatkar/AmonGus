#include "main.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"
#include "powerup.h"
#include "wall.h"
#include "grid.h"
#include <vector>
#include <map>

#define GLT_IMPLEMENTATION
#include "gltext.h"

#define NUM_POWERUPS 10

using namespace std;

GLMatrices Matrices;
GLuint programID, textShader;
GLFWwindow *window;

char maze[SIZE][SIZE];
int dist[SIZE][SIZE][SIZE][SIZE];

/**************************
* Customizable functions *
**************************/

Player player;
Enemy enemy;
Powerup kill;
Powerup release;
Powerup powerups[NUM_POWERUPS];
Powerup obstacles[NUM_POWERUPS];
vector<Wall> walls;
Wall wall, exitwall;

bool powerupsReleased = false;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

// Camera stuff

glm::vec3 eye(SIZE/2, SIZE/2, 5);
glm::vec3 dir(0,0,-5);
glm::vec3 target(0, 0, 0);
glm::vec3 up(0, 1, 0);

GLTtext *healthText, *pointsText, *tasksText, *lightText, *timeText, *endText;

bool task1done = false;
bool task2done = false;
bool lightOn = true;

time_t startTime;
time_t currTime;

float points = 0;
int numPowerUpsTaken = 0;

int numTasksDone(){
    if(task1done && task2done) 
        return 2;
    if(task1done || task2done)
        return 1;
    return 0;
}

float somedel = 4; // Controls level of zoom, 4 = normal

bool gameover = false;
bool youWin = false;

void draw()
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, eye + dir, up); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP; // MVP = Projection * View * Model

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(800));
    GLfloat view[] = {
        1., 0., 0., 0.,
        0., 1., 0., 0.,
        0., 0., 1., 0.,
        0., 0., 0., 1.
    };

    if(gameover){
        gltBeginDraw();
        if(youWin){
            gltSetText(endText, "You won!");
            gltColor(0.0f, 1.0f, 0.0f, 1.0f);
        }
        else {
            gltSetText(endText, "Game Over");
            gltColor(1.0f, 0.0f, 0.0f, 1.0f);
        }
        gltDrawText3D(endText, 200, 400, 1, 5, view, (GLfloat*)&projection);
        string temp = "Points:";
        gltSetText(pointsText, (temp + std::to_string(points)).c_str());
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText3D(pointsText, 250, 300, 1, 2, view, (GLfloat*)&projection);
        gltEndDraw();
        return;
    }

    for(Wall wall: walls)
        wall.draw(VP);

    // Scene render
    if(!release.deleted) release.draw(VP);
    if(!kill.deleted) kill.draw(VP);

    if(powerupsReleased){
        for(int i = 0; i<NUM_POWERUPS; i++){
            if(!powerups[i].deleted){
                powerups[i].draw(VP);
            }
            if(!obstacles[i].deleted){
                obstacles[i].draw(VP);
            }
        }
    }

    
    if(!enemy.deleted)enemy.draw(VP);
    player.draw(VP);

    gltBeginDraw();

    string temp = "Health:";
    gltSetText(healthText, (temp + std::to_string(player.health)).c_str());

    temp = "Points:";
    gltSetText(pointsText, (temp + std::to_string(points)).c_str());

    temp = "Tasks:";
    string temp2 = "/2";
    gltSetText(tasksText, (temp + std::to_string(numTasksDone()) + temp2).c_str());

    if(lightOn) gltSetText(lightText, "Lights:ON");
    else gltSetText(lightText, "Lights:OFF");

    temp = "Time:";
    time(&currTime);
    gltSetText(timeText, (temp + std::to_string(100 - currTime + startTime)).c_str());

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    gltDrawText3D(healthText, 0, 770, 1, 2, view, (GLfloat*)&projection);
    gltDrawText3D(pointsText, 0, 740, 1, 2, view, (GLfloat*)&projection);
    gltDrawText3D(tasksText, 0, 710, 1, 2, view, (GLfloat*)&projection);
    gltDrawText3D(lightText, 0, 680, 1, 2, view, (GLfloat*)&projection);
    gltDrawText3D(timeText, 0, 650, 1, 2, view, (GLfloat*)&projection);

    gltEndDraw();
    
}

void tick_input(GLFWwindow *window)
{
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    float speed = 0.05;
    glm::vec2 delta(0, 0);


    if (left)
    {
        delta.x -= speed;
    }
    if (right)
    {
        delta.x += speed;
    }
    if (up)
    {
        delta.y += speed;
    }
    if (down)
    {
        delta.y -= speed;
    }


    player.set_position(player.position.x + delta.x, player.position.y + delta.y);
    bool colliding = false;
    for(Wall wall: walls){
        if (detect_collision(player.bb(),wall.bb())){
            colliding = true;
            if(wall.type == 'e' && numTasksDone() == 2){
                gameover = true;
                youWin = true;
                /*
                gltDeleteText(healthText);
                gltDeleteText(pointsText);
                gltDeleteText(timeText);
                gltDeleteText(lightText);
                gltDeleteText(tasksText);
                // Destroy glText
                gltTerminate();

                quit(window);
                */
            }
            break;
        }
    }
    if(colliding)
    {
        //cout<<"Colliding"<<endl;
        player.set_position(player.position.x - delta.x, player.position.y - delta.y);
    }

    if(powerupsReleased){
        for(int i = 0; i<NUM_POWERUPS; i++){
            if(!powerups[i].deleted){
                if (detect_collision(player.bb(),powerups[i].bb())){
                    powerups[i].deleted = true;
                    points += 10;
                    numPowerUpsTaken++;
                    if(numPowerUpsTaken == 10){
                        task2done = true;
                    }
                }
            }
        }
    }

    if(powerupsReleased){
        for(int i = 0; i<NUM_POWERUPS; i++){
            if(!obstacles[i].deleted){
                if (detect_collision(player.bb(),obstacles[i].bb())){
                    obstacles[i].deleted = true;
                    player.health -= 1;
                }
            }
        }
    }

    if(!kill.deleted){
        // Kill enemy powerup
        if(detect_collision(player.bb(), kill.bb())){
            kill.deleted = true;
            enemy.deleted = true;
            task1done = true;
        }
    }

    if(!release.deleted){
        if(detect_collision(player.bb(), release.bb())){
            release.deleted = true;
            powerupsReleased = true;
        }
    }

    // Enemy movement
    if(!enemy.deleted){
        glm::vec2 enemyDelta = enemy.findPlayer(maze, dist, player.position.x, player.position.y);
        enemy.set_position(enemy.position.x + enemy.speed * enemyDelta.x, enemy.position.y + enemy.speed * enemyDelta.y);
        if(detect_collision(player.bb(), enemy.bb())){
            player.health -= 0.05;
        }
    }

    // Move camera with player
    int del = somedel - 2;
    if(player.position.x - eye.x > del)
        eye.x += speed;
    else if(player.position.x - eye.x < -1*del)
        eye.x -= speed;
    
    if(player.position.y - eye.y > del)
        eye.y += speed;
    else if(player.position.y - eye.y < -1*del)
        eye.y -= speed;
}

void tick_elements()
{
    player.tick();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    
    // Maze stuff
    init_maze(maze);
    partition(maze, 0, SIZE-1, 0, SIZE-1);
    min_distances(maze, dist);

    int rnd = rand() % (4 * (SIZE-2));
    int loc = rnd / (SIZE-2);
    if(loc == 0){
        maze[rnd % (SIZE-2)][0] = 'E';
    }
    else if(loc == 1){
        maze[0][rnd % (SIZE-2)] = 'E';
    }
    else if(loc == 2){
        maze[rnd % (SIZE-2)][SIZE-1] = 'E';
    }
    else if(loc == 3){
        maze[SIZE-1][rnd % (SIZE-2)] = 'E';
    }

    for(int i = 0; i<SIZE; i++){
        for(int j = 0; j<SIZE; j++){
            if(maze[i][j] == 'X'){
                wall = Wall(j,i,'x');
                walls.push_back(wall);
            }
            else if(maze[i][j] == 'E'){
                wall = Wall(j,i,'e');
                walls.push_back(wall);
                exitwall = wall;
            }
        }
    }

    // Random co-ordinates for player
    int i = rand() % SIZE;
    int j = rand() % SIZE;
    while(maze[i][j] != ' '){
        i = rand() % SIZE;
        j = rand() % SIZE;
    }
    player = Player(j, i , COLOR_RED);

    // Random co-ordinates for enemy
    i = rand() % SIZE;
    j = rand() % SIZE;
    while(maze[i][j] != ' '){
        i = rand() % SIZE;
        j = rand() % SIZE;
    }
    enemy = Enemy(j, i , COLOR_RED);

    // Kill powerup
    i = rand() % SIZE;
    j = rand() % SIZE;
    while(maze[i][j] != ' '){
        i = rand() % SIZE;
        j = rand() % SIZE;
    }
    kill = Powerup(j, i, 'k');

    // Release powerup 
    i = rand() % SIZE;
    j = rand() % SIZE;
    while(maze[i][j] != ' '){
        i = rand() % SIZE;
        j = rand() % SIZE;
    }
    release = Powerup(j, i, 'r');

    // Powerups
    for(int k = 0; k<NUM_POWERUPS; k++){
        i = rand() % SIZE;
        j = rand() % SIZE;
        while(maze[i][j] != ' '){
            i = rand() % SIZE;
            j = rand() % SIZE;
        }
        powerups[k] = Powerup(j, i, 'p');
    }

    // Obstacles
    for(int k = 0; k<NUM_POWERUPS; k++){
        i = rand() % SIZE;
        j = rand() % SIZE;
        while(maze[i][j] != ' '){
            i = rand() % SIZE;
            j = rand() % SIZE;
        }
        obstacles[k] = Powerup(j, i, 'o');
    }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv)
{
    srand(time(0));
    int width = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL(window, width, height);

    time(&startTime);

    //glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Text rendering stuff
    gltInit();

    // Creating text
    healthText = gltCreateText();
    pointsText = gltCreateText();
    tasksText = gltCreateText();
    lightText = gltCreateText();
    timeText = gltCreateText();
    endText = gltCreateText();

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            // 60 fps
            // OpenGL Draw commands
            draw();

            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        if(100 - currTime + startTime < 0) gameover = true;
        if(player.health <= 0) gameover = true;

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    gltDeleteText(healthText);
    gltDeleteText(pointsText);
    gltDeleteText(timeText);
    gltDeleteText(lightText);
    gltDeleteText(tasksText);
    gltDeleteText(endText);
    // Destroy glText
    gltTerminate();

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b)
{
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen()
{
    float top = screen_center_y + somedel / screen_zoom;
    float bottom = screen_center_y - somedel / screen_zoom;
    float left = screen_center_x - somedel / screen_zoom;
    float right = screen_center_x + somedel / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
