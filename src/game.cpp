#include "header/game.hpp"
#include "header/aabb.hpp"
#include "header/entity.hpp"
#include "header/event_collider.hpp"
#include "header/gameController.hpp"
#include "header/level.hpp"
#include "header/light.hpp"
#include "header/material.hpp"
#include "header/passive_entity.hpp"
#include "header/physicscontroller.hpp"
#include "header/platform.hpp"
#include "header/deadly_platform.hpp"
#include "header/hud_element.hpp"
#include <cmath>
#include <cstddef>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

//////////////////////////////  State variables ////////////////////////////////
GameController &gc = GameController::getInstance();
PhysicsController &pc = PhysicsController::getInstance();
int initTime = 0;

bool mouseMoved = false;

bool choosingColour = false;

// Colour circle texture
HUD greenCircle;
HUD redCircle;
HUD blueCircle;

////////////////////////////  Function definitions  ////////////////////////////
int main(int argc, char *argv[]) {

  glutInit(&argc, argv);

  loadLevel();
  loadHUDTextures();

  // register entities in physics controller
  for (size_t i = 0; i < gc.getLevel().getEntities().size(); i++)
    pc.registerObject(&gc.getLevel().getEntities()[i], true);

  // register platforms in physics controller
  for (size_t i = 0; i < gc.getLevel().getPlatforms().size(); i++)
    pc.registerObject(&gc.getLevel().getPlatforms()[i]);

  for(size_t i = 0; i < gc.getLevel().getDeadlyPlatforms().size(); i++)
    pc.registerObject(&gc.getLevel().getDeadlyPlatforms()[i]);

  // Register event collider objects
  for (size_t i = 0; i < gc.getLevel().getEventColliders().size(); i++)
    pc.registerObject(&gc.getLevel().getEventColliders()[i]);

  ////////////////////////////// Initialise GLUT  //////////////////////////////
  initWindow();

  ////////////////////////////  Register functions  ////////////////////////////
  // display functions
  glutDisplayFunc(display);
  glutReshapeFunc(resize);

  // Control functions
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);

  // Mouse functions
  glutMouseFunc(mouse);
  glutMotionFunc(activeMouse);
  glutPassiveMotionFunc(passiveMouse);

  // Model update function
  initTime = glutGet(GLUT_ELAPSED_TIME); // Initialise initTime
  // glutIdleFunc(updateModel);

  // Schedule display refresh
  glutTimerFunc(10, callRedisplay, 0);

  glutMainLoop();
  return 0;
}

// Initialisation Functions
void initWindow() {
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
  glClearColor(0.0, 0.0, 0.0, 0);

  glutInitWindowSize(gc.getWidth(), gc.getHeight());
  glutCreateWindow("Colour Game"); // creates the window

  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 1, 200);
}

void loadLevel() {
  Level l1{{0, 0, 0}};
  l1.setInitColor(RED);

  ////////////////////////////// Create platforms //////////////////////////////
  Platform floor1{{"models/floor.obj", ""}, {0, 0, 0}, RED};
  DeadlyPlatform deadPlat1{{"models/deadlyplat.obj", ""}, {5, 1, -1}, RED};

  Platform floor2{{"models/floor.obj", ""}, {4, 1, 0}, BLUE};
  DeadlyPlatform deadPlat2{{"models/deadlyplat.obj", ""}, {9, 2, -1}, BLUE};

  Platform floor3{{"models/floor.obj", ""}, {8, 2, 0}, GREEN};
  Platform wall3{{"models/wall.obj", ""}, {10, 2, 0}, GREEN};

  Platform floor4{{"models/floor.obj", ""}, {12, 3, 0}, BLUE};

  l1.addPlatforms({floor1, floor2, floor3, floor4});
  l1.addPlatforms({wall3});

  l1.addDeadlyPlatforms({deadPlat1, deadPlat2});
  //////////////////////////////  Insert lights ////////////////////////////////
  Light lght0 = {
    {0, 1, 0, 1}, {0.0, 0.8, 0.2, 1}, {0.0, 0.8, 0.2, 1}, {0.1, 0.1, 0.1, 1}, GL_LIGHT0};

  Light lght1 = {
    {20, 10, 20, 1}, {0.0, 0.8, 0.2, 1}, {0.0, 0.8, 0.2, 1}, {1, 1, 1, 1}, GL_LIGHT1};

  l1.addLights({lght0, lght1});
  //////////////////////////// Add Event Colliders  ////////////////////////////

  // End game event
  EventCollider ev1{saveCheckpoint, AABB{{-2, 0, -2}, {2, 1, 2}, {4, 2, 0}, BLUE}};
  EventCollider ev2{saveCheckpoint, AABB{{-2, 0, -2}, {2, 1, 2}, {8, 3, 0}, GREEN}};
  EventCollider ev3{endGame, AABB{{-2, 0, -2}, {2, 1, 2}, {12, 4, 0}, BLUE}};

  l1.addEventColliders({ev1, ev2, ev3});

  // start level
  gc.startLevel(l1);
}
void loadHUDTextures(){
  // Load red circle
  greenCircle.load("textures/HUD/green_circle.png");
  redCircle.load("textures/HUD/red_circle.png");
  blueCircle.load("textures/HUD/blue_circle.png");
}

// Display Functions
void callRedisplay(int t) {
  glutPostRedisplay();
  updateModel();
  glutTimerFunc(10, callRedisplay, 0);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Render lights
  for (Light light : gc.getLevel().getLights())
    light.render();

  drawWorld();

  drawOverlay();

  glFlush();
}

void resize(int width, int height) {
  glViewport(0, 0, width, height);
  gc.setWidth(width);
  gc.setHeight(height);
}

// Perspective functions
void drawWorld() {

  //////////////////////  Set projection to Perspective ////////////////////////
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float fovRatio = ((float) gc.getWidth()) / ((float) gc.getHeight());
  gluPerspective(90, fovRatio, 1, 100);

  ////////////////////////////////  Draw World  ////////////////////////////////

  /** set material
   * Material will be the same for all objects
   * Based on the current game colour
   */
  switch (gc.getColor()) {
  case RED:
    redMat.set();
    break;
  case BLUE:
    blueMat.set();
    break;
  case GREEN:
    greenMat.set();
    break;
  }

  gc.getPlayer().render(gc.getColor());

  // Render Entities
  for (Entity entity : gc.getLevel().getEntities())
    entity.render(gc.getColor());

  // Render platforms
  for (Platform platform : gc.getLevel().getPlatforms())
    platform.render(gc.getColor());

  for (DeadlyPlatform deadlyPlat : gc.getLevel().getDeadlyPlatforms())
    deadlyPlat.render(gc.getColor());
}

// Ortho/HUD functions
void drawOverlay() {
  //////////////////////  Set projection to Orthographic  //////////////////////
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, gc.getWidth(), 0, gc.getHeight());
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //////////////////////////////// Draw overlay ////////////////////////////////
  // Disable light
  glDisable(GL_LIGHTING);

  // Draw overlay here
  if (choosingColour)
    drawColourSelectionMenu();

  // Enable light
  glEnable(GL_LIGHTING);
}
void drawColourSelectionMenu() {
  {
    float zoom;
    if (gc.getColor() == RED)
      zoom = 4;
    else
      zoom = 2;
    int x = gc.getWidth() / 4;
    x -= (redCircle.width * zoom) / 2;

    int y = gc.getHeight() / 3;
    y -= (redCircle.height * zoom) / 2;
    redCircle.draw(x, y, zoom);
  }

  {
    float zoom;
    if (gc.getColor() == BLUE)
      zoom = 4;
    else
      zoom = 2;
    int x = gc.getWidth() / 4;
    x *= 3;
    x -= (blueCircle.width * zoom) / 2;

    int y = gc.getHeight() / 3;
    y -= (blueCircle.height * zoom) / 2;
    blueCircle.draw(x, y, zoom);
  }

  {
    float zoom;
    if (gc.getColor() == GREEN)
      zoom = 4;
    else
      zoom = 2;
    int x = gc.getWidth() / 2;
    x -= (greenCircle.width * zoom) / 2;

    int y = gc.getHeight() / 3;
    y *= 2;
    y -= (greenCircle.height * zoom) / 2;
    greenCircle.draw(x, y, zoom);
  }
}

// Input Functions
// Keyboard
void keyboard(unsigned char key, int x, int y) {

  // handle input unrelated to player
  switch (key) {
  case 27:
    if (gc.getGameState()) {
      gc.setGameState(false);
      // open menu
    } else {
      // close menu
      gc.setGameState(true);
    }
    break;
  case 'q':
  case 'Q':
    exit(0);
  }
  gc.getPlayer().input(key, x, y);
}
void keyboardUp(unsigned char key, int x, int y) {

  gc.getPlayer().keyUp(key, x, y);
}
void specialKeyboard(int key, int x, int y) {}

// Mouse
void mouse(int button, int state, int x, int y){
  if (button == GLUT_LEFT_BUTTON){
    if (state == GLUT_DOWN){
      gc.setGameState(false);
      choosingColour = true;
    } else if (state == GLUT_UP) {
      gc.setGameState(true);
      choosingColour = false;
    }
  }
}
void activeMouse(int x, int y){
  if (choosingColour)
    changeColor(x, y);
}
void passiveMouse(int x, int y) {
  // if the game is not running, then do nothing.
  if (!gc.getGameState())
    return;

  // If the mouse was artificially moved, do nothing.
  if (mouseMoved) {
    mouseMoved = false;
    return;
  }

  // get change in mouse position
  float dx = ((float)gc.getWidth()) / 2 - x;
  float dy = ((float)gc.getHeight()) / 2 - y;
  dx *= 0.001;
  dy *= 0.001;
  gc.getPlayer().rotate({dx, dy}); // rotate the player entity

  // set mouse flag to true
  mouseMoved = true;
  // artificially move mouse to centre
  glutWarpPointer(gc.getWidth() / 2, gc.getHeight() / 2);
}

// Game related functions
void updateModel() {
  int curTime = glutGet(GLUT_ELAPSED_TIME);
  int dt = curTime - initTime;
  // if no time has passed, do nothing
  if (dt <= 0)
    return;

  // set initial time to now
  initTime = curTime;

  // update physics objects
  if (gc.getGameState()) // Only if the game is running
    pc.updateAll(dt);
}

void changeColor(int x, int y){

  auto rightCheck = [](int x) -> int {
    x -= gc.getWidth() / 2;
    return x * tan(1.0 / 6.0 * M_PI) + (gc.getHeight() / 2);
  };
  auto leftCheck = [](int x) -> int {
    x -= gc.getWidth() / 2;
    return x * tan(-(1.0/(6.0 * M_PI))) + (gc.getHeight() / 2);
  };

  y = gc.getHeight() - y;
  if (x > gc.getWidth() / 2) { // left side
    if (y > rightCheck(x))
      gc.setColour(GREEN);
    else
      gc.setColour(BLUE);
  } else { // right side
    if (y > leftCheck(x))
      gc.setColour(GREEN);
    else
      gc.setColour(RED);
  }
}

void saveCheckpoint() { gc.saveCheckpoint(); }
void endGame() { gc.endGame(); }
