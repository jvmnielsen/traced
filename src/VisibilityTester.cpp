//
// Created by Jacob Vesti Moeslund Nielsen on 19/10/2018.
//

#include "VisibilityTester.h"


bool VisibilityTester::IsVisible(const Point3f& p1, const Point3f& p2, const Scene& scene) const
{
    const Vec3f offset = p2 - p1;
    const float distance = offset.Length();
    Rayf ray{ p1, offset, distance };
    return !scene.IntersectsQuick(ray);
}
