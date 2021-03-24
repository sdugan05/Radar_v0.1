//
// Created by Saul on 3/23/2021.
//

#ifndef RADAR_V0_1_VECTOR2_HPP
#define RADAR_V0_1_VECTOR2_HPP

#include <iostream>
#include <complex>


class Vector2 {
public:
    float x,y;
    Vector2(float x, float y);

    static float distance(Vector2 a, Vector2 b);
};


#endif //RADAR_V0_1_VECTOR2_HPP
