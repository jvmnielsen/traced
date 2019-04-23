//
// Created by Jacob Vesti Moeslund Nielsen on 2019-03-30.
//

#include "distributions.hpp"
#include <algorithm>

/*
inline FLOAT TrowbridgeReitzDistribution::RoughnessToAlpha(FLOAT roughness) {
    roughness = std::max(roughness, (FLOAT)1e-3);
    FLOAT x = std::log(roughness);
    return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
           0.000640711f * x * x * x * x;
}

auto cosine_between(const Vec3f& v, const Vec3f& w) -> FLOAT
{
    return dot(v, w) / (v.length() * w.length());
}

auto cos(const Vec3f& v, const Vec3f& w) -> FLOAT
{
    return dot(v, w);
}

auto sin(const Vec3f& v, const Vec3f& w) -> FLOAT
{
    return cross(v, w).length();
}

auto tan(const Vec3f& v, const Vec3f& w) -> FLOAT
{
    return cos(v, w) / sin(v, w);
}

auto sine_between(const Vec3f& v, const Vec3f& w) -> FLOAT
{
    return (cross(v, w) / (v.length() * w.length())).length();
}

auto tan_between(const Vec3f& v, const Vec3f& w) -> FLOAT
{
    return cross(v, w).length() / dot(v, w);
}

inline FLOAT CosPhi(const Vec3f& v, const Vec3f& w) {
    const auto sinTheta = sin(v, w);
    return (sinTheta == 0) ? 1 : std::clamp(w.x() / sinTheta, -1.0f, 1.0f);
}

inline FLOAT SinPhi(const Vec3f& v, const Vec3f& w) {
    const auto sinTheta = sin(v, w);
    return (sinTheta == 0) ? 0 : std::clamp(w.y() / sinTheta, -1.0f, 1.0f);
}

FLOAT TrowbridgeReitzDistribution::D(const Vec3f& wh, const Vec3f& normal) const {
    //const auto& normal = isect.shading_normal();
    FLOAT tan2Theta = tan(wh, normal) * tan(wh, normal); //Tan2Theta(wh); // Sin2Theta(w) / Cos2Theta(w);
    if (std::isinf(tan2Theta)) return 0.;
    const auto cosine = cos(wh, normal);
    const FLOAT cos4Theta = cosine * cosine * cosine * cosine;
    const auto  e =
            ((CosPhi(wh, normal) * CosPhi(wh, normal)) / (alphax * alphax) + sin(wh, normal) * sin(wh, normal) / (alphay * alphay)) *
            tan2Theta;
    return 1 / (Math::Constants::Pi * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

FLOAT TrowbridgeReitzDistribution::Lambda(const Vec3f& w, const Vec3f& normal) const {
    const auto absTanTheta = std::abs(tan(w, normal));
    if (std::isinf(absTanTheta)) return 0.;
    // Compute _alpha_ for direction _w_
    const auto  alpha =
            std::sqrt(CosPhi(w, normal) * CosPhi(w, normal) * alphax * alphax + SinPhi(w, normal) * SinPhi(w, normal) * alphay * alphay);
    const auto alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}

Vec3f TrowbridgeReitzDistribution::Sample_wh(const Vec3f& wo, const Vec3f& normal,
                                                const Point2f &u) const {
    Vec3f wh;
    if (!sampleVisibleArea) {
        Float cosTheta = 0, phi = (2 * Pi) * u[1];
        if (alphax == alphay) {
            Float tanTheta2 = alphax * alphax * u[0] / (1.0f - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        } else {
            phi =
                    std::atan(alphay / alphax * std::tan(2 * Pi * u[1] + .5f * Pi));
            if (u[1] > .5f) phi += Pi;
            Float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
            const Float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
            const Float alpha2 =
                    1 / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
            Float tanTheta2 = alpha2 * u[0] / (1 - u[0]);
            cosTheta = 1 / std::sqrt(1 + tanTheta2);
        }
        Float sinTheta =
                std::sqrt(std::max((Float)0., (Float)1. - cosTheta * cosTheta));
        wh = SphericalDirection(sinTheta, cosTheta, phi);
        if (!SameHemisphere(wo, wh)) wh = -wh;
    } else {
        bool flip = wo.z < 0;
        wh = TrowbridgeReitzSample(flip ? -wo : wo, alphax, alphay, u[0], u[1]);
        if (flip) wh = -wh;
    }
    return wh;
}

Float TrowbridgeReitzDistribution::Lambda(const Vector3f &w) const {
    Float absTanTheta = std::abs(TanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    // Compute _alpha_ for direction _w_
    Float alpha =
            std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
    Float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
    return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}
 */
