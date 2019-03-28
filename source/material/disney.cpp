#include "disney.hpp"

inline auto SchlickWeight(FLOAT cosTheta) -> FLOAT {
    const auto m = Math::Clamp(1.0 - cosTheta, 0.0, 1.0);
    return (m * m) * (m * m) * m;
}

inline auto FrSchlick(FLOAT R0, FLOAT cosTheta) -> FLOAT{
    return Math::Lerp(SchlickWeight(cosTheta), R0, 1.0f);
}

inline auto FrSchlick(const Color3f& R0, FLOAT cosTheta) -> Color3f {
    return Lerp(SchlickWeight(cosTheta), R0, Color3f{1.0});
}

// For a dielectric, R(0) = (eta - 1)^2 / (eta + 1)^2, assuming we're
// coming from air..
inline auto SchlickR0FromEta(FLOAT eta) -> FLOAT { return Math::Square(eta - 1) / Math::Square(eta + 1); }

/*
Spectrum DisneyDiffuse::f(const Vector3f &wo, const Vector3f &wi) const {
    Float Fo = SchlickWeight(AbsCosTheta(wo)),
        Fi = SchlickWeight(AbsCosTheta(wi));

    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing.
    // Burley 2015, eq (4).
    return R * InvPi * (1 - Fo / 2) * (1 - Fi / 2);
}

// -----------------------------------------

Spectrum DisneyFakeSS::f(const Vector3f &wo, const Vector3f &wi) const {
    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.);
    wh = Normalize(wh);
    Float cosThetaD = Dot(wi, wh);

    // Fss90 used to "flatten" retroreflection based on roughness
    Float Fss90 = cosThetaD * cosThetaD * roughness;
    Float Fo = SchlickWeight(AbsCosTheta(wo)),
        Fi = SchlickWeight(AbsCosTheta(wi));
    Float Fss = Lerp(Fo, 1.0, Fss90) * Lerp(Fi, 1.0, Fss90);
    // 1.25 scale is used to (roughly) preserve albedo
    Float ss =
        1.25f * (Fss * (1 / (AbsCosTheta(wo) + AbsCosTheta(wi)) - .5f) + .5f);

    return R * InvPi * ss;
}

// -----------------------------------------

Spectrum DisneyRetro::f(const Vector3f &wo, const Vector3f &wi) const {
    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.);
    wh = Normalize(wh);
    Float cosThetaD = Dot(wi, wh);

    Float Fo = SchlickWeight(AbsCosTheta(wo)),
        Fi = SchlickWeight(AbsCosTheta(wi));
    Float Rr = 2 * roughness * cosThetaD * cosThetaD;

    // Burley 2015, eq (4).
    return R * InvPi * Rr * (Fo + Fi + Fo * Fi * (Rr - 1));
}

// -----------------------------------------

Spectrum DisneySheen::f(const Vector3f &wo, const Vector3f &wi) const {
    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.);
    wh = Normalize(wh);
    Float cosThetaD = Dot(wi, wh);

    return R * SchlickWeight(cosThetaD);
}

// -----------------------------------------

inline Float GTR1(Float cosTheta, Float alpha) {
    Float alpha2 = alpha * alpha;
    return (alpha2 - 1) /
        (Pi * std::log(alpha2) * (1 + (alpha2 - 1) * cosTheta * cosTheta));
}

// Smith masking/shadowing term.
inline Float smithG_GGX(Float cosTheta, Float alpha) {
    Float alpha2 = alpha * alpha;
    Float cosTheta2 = cosTheta * cosTheta;
    return 1 / (cosTheta + sqrt(alpha2 + cosTheta2 - alpha2 * cosTheta2));
}

Spectrum DisneyClearcoat::f(const Vector3f &wo, const Vector3f &wi) const {
    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return Spectrum(0.);
    wh = Normalize(wh);

    // Clearcoat has ior = 1.5 hardcoded -> F0 = 0.04. It then uses the
    // GTR1 distribution, which has even fatter tails than Trowbridge-Reitz
    // (which is GTR2).
    Float Dr = GTR1(AbsCosTheta(wh), gloss);
    Float Fr = FrSchlick(.04, Dot(wo, wh));
    // The geometric term always based on alpha = 0.25.
    Float Gr =
        smithG_GGX(AbsCosTheta(wo), .25) * smithG_GGX(AbsCosTheta(wi), .25);

    return weight * Gr * Fr * Dr / 4;
}

Spectrum DisneyClearcoat::Sample_f(const Vector3f &wo, Vector3f *wi,
                                   const Point2f &u, Float *pdf,
                                   BxDFType *sampledType) const {
    // TODO: double check all this: there still seem to be some very
    // occasional fireflies with clearcoat; presumably there is a bug
    // somewhere.
    if (wo.z == 0) return 0.;

    Float alpha2 = gloss * gloss;
    Float cosTheta = std::sqrt(
        std::max(Float(0), (1 - std::pow(alpha2, 1 - u[0])) / (1 - alpha2)));
    Float sinTheta = std::sqrt(std::max((Float)0, 1 - cosTheta * cosTheta));
    Float phi = 2 * Pi * u[1];
    Vector3f wh = SphericalDirection(sinTheta, cosTheta, phi);
    if (!SameHemisphere(wo, wh)) wh = -wh;

    *wi = Reflect(wo, wh);
    if (!SameHemisphere(wo, *wi)) return Spectrum(0.f);

    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

Float DisneyClearcoat::Pdf(const Vector3f &wo, const Vector3f &wi) const {
    if (!SameHemisphere(wo, wi)) return 0;

    Vector3f wh = wi + wo;
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return 0;
    wh = Normalize(wh);

    // The sampling routine samples wh exactly from the GTR1 distribution.
    // Thus, the final value of the PDF is just the value of the
    // distribution for wh converted to a mesure with respect to the
    // surface normal.
    Float Dr = GTR1(AbsCosTheta(wh), gloss);
    return Dr * AbsCosTheta(wh) / (4 * Dot(wo, wh));
}

DisneyFresnel(const Spectrum &R0, Float metallic, Float eta)
    : R0(R0), metallic(metallic), eta(eta) {
}
Spectrum Evaluate(Float cosI) const {
    return Lerp(metallic, Spectrum(FrDielectric(cosI, 1, eta)),
                FrSchlick(R0, cosI));
}*/