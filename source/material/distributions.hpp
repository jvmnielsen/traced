//
// Created by Jacob Vesti Moeslund Nielsen on 2019-03-30.
//

#ifndef TRACED_DISTRIBUTIONS_HPP
#define TRACED_DISTRIBUTIONS_HPP

#include "../math/math_util.hpp"
#include "../math/vec3.hpp"
#include "../math/point2.hpp"
#include "../core/intersection.hpp"

/*
class TrowbridgeReitzDistribution {
public:
    static inline FLOAT RoughnessToAlpha(FLOAT roughness);

    TrowbridgeReitzDistribution(FLOAT alphax, FLOAT alphay) : alphax(alphax), alphay(alphay) {}

    FLOAT D(const Vec3f& wh, const Intersection& isect) const;
    Vec3f Sample_wh(const Vec3f &wo, const Point2f &u) const;

    FLOAT G(const Vec3f& wo, const Vec3f& wi) const {
        return 1 / (1 + Lambda(wo) + Lambda(wi));
    }

private:
    FLOAT Lambda(const Vec3f& w) const;
    const FLOAT alphax, alphay;
}; */

#endif //TRACED_DISTRIBUTIONS_HPP
