//
// Created by Saul on 3/23/2021.
//

#include "Vector2.hpp"
float Vector2::distance(Vector2 a, Vector2 b) {
    float dist = std::sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) * 1.0);
    return dist;
}

Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}
