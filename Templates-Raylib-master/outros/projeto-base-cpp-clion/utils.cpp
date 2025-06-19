/**
 * @file utils.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Utility functions implementation.
 * 
 * @copyright Copyright (c) 2025
 */
#include "raylib.h"
#include "include/utils.h"

double toRadians( double degrees ) {
    return degrees * PI / 180.0;
}

double toDegrees( double radians ) {
    return radians * 180.0 / PI;
}