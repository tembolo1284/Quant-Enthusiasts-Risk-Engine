#include "BlackScholes.h"
#include <cmath>
// works on windows only. Commenting out for now and adding M_PI for cross platform compatibilty with linux and mac os
// #include <corecrt_math_defines.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
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

// implied vol using Newton-Raphson method
double BlackScholes::impliedVolatilityCall(double S, double K, double r, double T, 
                                            double market_price, double initial_guess, 
                                            double tolerance, int max_iterations) {
    // handle edge cases
    if (T <= 0) return 0.0;
    
    // check if market price is within valid bounds
    double intrinsic_value = std::max(0.0, S - K * std::exp(-r * T));
    if (market_price < intrinsic_value) {
        return -1.0; // invalid: market price below intrinsic value
    }
    if (market_price >= S) {
        return -1.0; // invalid: market price too high
    }
    
    double sigma = initial_guess;
    
    for (int i = 0; i < max_iterations; ++i) {
        double price = callPrice(S, K, r, T, sigma);
        double price_diff = price - market_price;
        
        // check for convergence
        if (std::abs(price_diff) < tolerance) {
            return sigma;
        }
        
        // calculate vega for Newton-Raphson step
        double option_vega = vega(S, K, r, T, sigma);
        
        // avoid division by very small vega
        if (std::abs(option_vega) < 1e-10) {
            return -1.0; // Failed to converge
        }
        
        // newton-raphson update: sigma_new = sigma_old - f(sigma) / f'(sigma)
        sigma = sigma - price_diff / option_vega;
        
        // keep sigma in reasonable bounds
        if (sigma < 0.001) sigma = 0.001;
        if (sigma > 5.0) sigma = 5.0;
    }
    
    // failed to converge
    return -1.0;
}

double BlackScholes::impliedVolatilityPut(double S, double K, double r, double T, 
                                           double market_price, double initial_guess,
                                           double tolerance, int max_iterations) {
    // handle edge cases
    if (T <= 0) return 0.0;
    
    // check if market price is within valid bounds
    double intrinsic_value = std::max(0.0, K * std::exp(-r * T) - S);
    if (market_price < intrinsic_value) {
        return -1.0; // invalid: market price below intrinsic value
    }
    if (market_price >= K * std::exp(-r * T)) {
        return -1.0; // invalid: market price too high
    }
    
    double sigma = initial_guess;
    
    for (int i = 0; i < max_iterations; ++i) {
        double price = putPrice(S, K, r, T, sigma);
        double price_diff = price - market_price;
        
        // check for convergence
        if (std::abs(price_diff) < tolerance) {
            return sigma;
        }
        
        // calculate vega for Newton-Raphson step
        double option_vega = vega(S, K, r, T, sigma);
        
        // avoid division by very small vega
        if (std::abs(option_vega) < 1e-10) {
            return -1.0; // Failed to converge
        }
        
        // newton-raphson update
        sigma = sigma - price_diff / option_vega;
        
        // keep sigma in reasonable bounds
        if (sigma < 0.001) sigma = 0.001;
        if (sigma > 5.0) sigma = 5.0;
    }
    
    // failed to converge
    return -1.0;
}
