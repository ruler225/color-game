// Tentative name, used to store data that is used by all object, and fits in

#pragma once
#include <array>

using std::array;

// none the colours for the filters, `GREEN` is tentative
enum Color { RED, BLUE, GREEN };

using Vector3 = array<float, 3>; // common data alias

////////////////////////////  Function prototypes //////////////////////////////
// display functions
void display();
void drawWorld();
void drawOverlay();

void drawColourSelectionMenu();

void callRedisplay(int t);
void resize(int width, int height);

/////////////////  Input functions
// Keyboard
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);

// Mouse Functions
void mouse(int button, int state, int x, int y);
void activeMouse(int x, int y);
void passiveMouse(int x, int y);

// Initialisation functions
void initWindow();

void loadLevel();

void loadHUDTextures();

// Utility Functions
void changeColor(int x, int y);

// Game related functions
void updateModel();

void saveCheckpoint();
void endGame();
