/**
 * @file main.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Main function. Base template for game development in C++ using
 * Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2025
 */
#include "GameWindow.h"

int main() {

    GameWindow gameWindow( 
        800,             // width
        450,             // height
        "Window Title",  // title
        60,              // target FPS
        true,            // antialiasing
        false,           // resizable
        false,           // full screen
        false,           // undecorated
        false,           // always on top
        false,           // always run
        false,           // load resources
        false            // init audio
    );

    return 0;

}