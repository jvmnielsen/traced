//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#ifndef RAYTRACER_VISIBILITYTESTER_H
#define RAYTRACER_VISIBILITYTESTER_H

#include "Scene.h"

class VisibilityTester
{
public:
    bool IsVisible(const Point3f& p1, const Point3f& p2, const Scene& scene) const;
};


#endif //RAYTRACER_VISIBILITYTESTER_H
