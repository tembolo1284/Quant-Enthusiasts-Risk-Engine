#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H

namespace BlackScholes {
    double N(double z);
    double callPrice(double S, double K, double r, double T, double sigma);
    double putPrice(double S, double K, double r, double T, double sigma);
    double callDelta(double S, double K, double r, double T, double sigma);
    double putDelta(double S, double K, double r, double T, double sigma);
    double gamma(double S, double K, double r, double T, double sigma);
    double vega(double S, double K, double r, double T, double sigma);
    double callTheta(double S, double K, double r, double T, double sigma);
    double putTheta(double S, double K, double r, double T, double sigma);
}

#endif
