#include "BlackScholes.h"
#include <cmath>
#include <corecrt_math_defines.h>
#include <algorithm>

double BlackScholes::N(double z) {
    return 0.5 * (1.0 + std::erf(z / std::sqrt(2.0)));
}

double BlackScholes::callPrice(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return std::max(0.0, S - K);
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    return S * N(d1) - K * std::exp(-r * T) * N(d2);
}

double BlackScholes::putPrice(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return std::max(0.0, K - S);
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    return K * std::exp(-r * T) * N(-d2) - S * N(-d1);
}

double BlackScholes::callDelta(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return (S > K) ? 1.0 : 0.0;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    return N(d1);
}

double BlackScholes::putDelta(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return (S < K) ? -1.0 : 0.0;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    return N(d1) - 1.0;
}

double BlackScholes::gamma(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return 0.0;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double nd1 = (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * d1 * d1);
    return nd1 / (S * sigma * std::sqrt(T));
}

double BlackScholes::vega(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return 0.0;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double nd1 = (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * d1 * d1);
    return S * nd1 * std::sqrt(T);
}

double BlackScholes::callTheta(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return 0.0;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    double nd1 = (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * d1 * d1);
    double term1 = -(S * nd1 * sigma) / (2 * std::sqrt(T));
    double term2 = r * K * std::exp(-r * T) * N(d2);
    return (term1 - term2) / 365.0;
}

double BlackScholes::putTheta(double S, double K, double r, double T, double sigma) {
    if (T <= 0 || sigma <= 0) return 0.0;
    double d1 = (std::log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * std::sqrt(T));
    double d2 = d1 - sigma * std::sqrt(T);
    double nd1 = (1.0 / std::sqrt(2.0 * M_PI)) * std::exp(-0.5 * d1 * d1);
    double term1 = -(S * nd1 * sigma) / (2 * std::sqrt(T));
    double term2 = r * K * std::exp(-r * T) * N(-d2);
    return (term1 + term2) / 365.0;
}
