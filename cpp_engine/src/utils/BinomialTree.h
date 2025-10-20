#ifndef BINOMIALTREE_H
#define BINOMIALTREE_H

namespace BinomialTree {
    // Cox-Ross-Rubinstein Binomial Tree for American Options
    
    /**
     * Price an American call option using binomial tree
     * @param S: Current spot price
     * @param K: Strike price
     * @param r: Risk-free rate
     * @param T: Time to expiry (years)
     * @param sigma: Volatility
     * @param steps: Number of time steps (default 100, more = more accurate)
     * @return Option price
     */
    double americanCallPrice(double S, double K, double r, double T, double sigma, int steps = 100);
    
    /**
     * Price an American put option using binomial tree
     * @param S: Current spot price
     * @param K: Strike price
     * @param r: Risk-free rate
     * @param T: Time to expiry (years)
     * @param sigma: Volatility
     * @param steps: Number of time steps (default 100, more = more accurate)
     * @return Option price
     */
    double americanPutPrice(double S, double K, double r, double T, double sigma, int steps = 100);
    
    /**
     * Calculate delta using finite difference
     * @param is_call: true for call, false for put
     * @param S: Current spot price
     * @param K: Strike price
     * @param r: Risk-free rate
     * @param T: Time to expiry (years)
     * @param sigma: Volatility
     * @param steps: Number of time steps
     * @param bump: Spot price bump for finite difference (default 0.01)
     * @return Delta
     */
    double americanDelta(bool is_call, double S, double K, double r, double T, double sigma, 
                         int steps = 100, double bump = 0.01);
    
    /**
     * Calculate gamma using finite difference
     * @param is_call: true for call, false for put
     * @param S: Current spot price
     * @param K: Strike price
     * @param r: Risk-free rate
     * @param T: Time to expiry (years)
     * @param sigma: Volatility
     * @param steps: Number of time steps
     * @param bump: Spot price bump for finite difference (default 0.01)
     * @return Gamma
     */
    double americanGamma(bool is_call, double S, double K, double r, double T, double sigma, 
                         int steps = 100, double bump = 0.01);
    
    /**
     * Calculate vega using finite difference
     * @param is_call: true for call, false for put
     * @param S: Current spot price
     * @param K: Strike price
     * @param r: Risk-free rate
     * @param T: Time to expiry (years)
     * @param sigma: Volatility
     * @param steps: Number of time steps
     * @param bump: Volatility bump for finite difference (default 0.01)
     * @return Vega
     */
    double americanVega(bool is_call, double S, double K, double r, double T, double sigma, 
                        int steps = 100, double bump = 0.01);
    
    /**
     * Calculate theta using finite difference
     * @param is_call: true for call, false for put
     * @param S: Current spot price
     * @param K: Strike price
     * @param r: Risk-free rate
     * @param T: Time to expiry (years)
     * @param sigma: Volatility
     * @param steps: Number of time steps
     * @param bump: Time bump for finite difference (default 1/365)
     * @return Theta (per day)
     */
    double americanTheta(bool is_call, double S, double K, double r, double T, double sigma, 
                         int steps = 100, double bump = 1.0/365.0);
}

#endif
