#include "BinomialTree.h"
#include <cmath>
#include <vector>
#include <algorithm>

// Portable M_PI definition
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double BinomialTree::americanCallPrice(double S, double K, double r, double T, double sigma, int steps) {
    // Handle edge cases
    if (T <= 0) return std::max(0.0, S - K);
    if (sigma <= 0) return std::max(0.0, S - K);
    if (steps <= 0) steps = 100;
    
    // Calculate parameters for Cox-Ross-Rubinstein model
    double dt = T / steps;                          // Time step
    double u = std::exp(sigma * std::sqrt(dt));    // Up factor
    double d = 1.0 / u;                             // Down factor
    double p = (std::exp(r * dt) - d) / (u - d);   // Risk-neutral probability
    double discount = std::exp(-r * dt);            // Discount factor
    
    // Initialize asset prices at maturity (final nodes)
    std::vector<double> asset_prices(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        asset_prices[i] = S * std::pow(u, steps - i) * std::pow(d, i);
    }
    
    // Initialize option values at maturity
    std::vector<double> option_values(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        option_values[i] = std::max(0.0, asset_prices[i] - K);
    }
    
    // Backward induction through the tree
    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            // Calculate asset price at this node
            double S_current = S * std::pow(u, step - i) * std::pow(d, i);
            
            // Calculate continuation value (expected discounted value)
            double continuation_value = discount * (p * option_values[i] + (1 - p) * option_values[i + 1]);
            
            // Calculate early exercise value
            double exercise_value = std::max(0.0, S_current - K);
            
            // American option: take maximum of continuation and exercise
            option_values[i] = std::max(continuation_value, exercise_value);
        }
    }
    
    return option_values[0];
}

double BinomialTree::americanPutPrice(double S, double K, double r, double T, double sigma, int steps) {
    // Handle edge cases
    if (T <= 0) return std::max(0.0, K - S);
    if (sigma <= 0) return std::max(0.0, K - S);
    if (steps <= 0) steps = 100;
    
    // Calculate parameters
    double dt = T / steps;
    double u = std::exp(sigma * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp(r * dt) - d) / (u - d);
    double discount = std::exp(-r * dt);
    
    // Initialize asset prices at maturity
    std::vector<double> asset_prices(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        asset_prices[i] = S * std::pow(u, steps - i) * std::pow(d, i);
    }
    
    // Initialize option values at maturity
    std::vector<double> option_values(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        option_values[i] = std::max(0.0, K - asset_prices[i]);
    }
    
    // Backward induction through the tree
    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            // Calculate asset price at this node
            double S_current = S * std::pow(u, step - i) * std::pow(d, i);
            
            // Calculate continuation value
            double continuation_value = discount * (p * option_values[i] + (1 - p) * option_values[i + 1]);
            
            // Calculate early exercise value
            double exercise_value = std::max(0.0, K - S_current);
            
            // American option: take maximum of continuation and exercise
            option_values[i] = std::max(continuation_value, exercise_value);
        }
    }
    
    return option_values[0];
}

double BinomialTree::americanDelta(bool is_call, double S, double K, double r, double T, 
                                    double sigma, int steps, double bump) {
    // Calculate delta using finite difference: (V(S+h) - V(S-h)) / (2h)
    double price_up, price_down;
    
    if (is_call) {
        price_up = americanCallPrice(S + bump, K, r, T, sigma, steps);
        price_down = americanCallPrice(S - bump, K, r, T, sigma, steps);
    } else {
        price_up = americanPutPrice(S + bump, K, r, T, sigma, steps);
        price_down = americanPutPrice(S - bump, K, r, T, sigma, steps);
    }
    
    return (price_up - price_down) / (2.0 * bump);
}

double BinomialTree::americanGamma(bool is_call, double S, double K, double r, double T, 
                                    double sigma, int steps, double bump) {
    // Calculate gamma using finite difference: (V(S+h) - 2*V(S) + V(S-h)) / h^2
    double price_center, price_up, price_down;
    
    if (is_call) {
        price_center = americanCallPrice(S, K, r, T, sigma, steps);
        price_up = americanCallPrice(S + bump, K, r, T, sigma, steps);
        price_down = americanCallPrice(S - bump, K, r, T, sigma, steps);
    } else {
        price_center = americanPutPrice(S, K, r, T, sigma, steps);
        price_up = americanPutPrice(S + bump, K, r, T, sigma, steps);
        price_down = americanPutPrice(S - bump, K, r, T, sigma, steps);
    }
    
    return (price_up - 2.0 * price_center + price_down) / (bump * bump);
}

double BinomialTree::americanVega(bool is_call, double S, double K, double r, double T, 
                                   double sigma, int steps, double bump) {
    // Calculate vega using finite difference: (V(σ+h) - V(σ-h)) / (2h)
    double price_up, price_down;
    
    if (is_call) {
        price_up = americanCallPrice(S, K, r, T, sigma + bump, steps);
        price_down = americanCallPrice(S, K, r, T, sigma - bump, steps);
    } else {
        price_up = americanPutPrice(S, K, r, T, sigma + bump, steps);
        price_down = americanPutPrice(S, K, r, T, sigma - bump, steps);
    }
    
    return (price_up - price_down) / (2.0 * bump);
}

double BinomialTree::americanTheta(bool is_call, double S, double K, double r, double T, 
                                    double sigma, int steps, double bump) {
    // Handle edge case where T is too small
    if (T <= bump) return 0.0;
    
    // Calculate theta using finite difference: (V(T-h) - V(T)) / h
    // Note: theta is negative for long options (time decay)
    double price_current, price_later;
    
    if (is_call) {
        price_current = americanCallPrice(S, K, r, T, sigma, steps);
        price_later = americanCallPrice(S, K, r, T - bump, sigma, steps);
    } else {
        price_current = americanPutPrice(S, K, r, T, sigma, steps);
        price_later = americanPutPrice(S, K, r, T - bump, sigma, steps);
    }
    
    return (price_later - price_current) / bump;
}
