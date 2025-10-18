#ifndef RISKENGINE_H
#define RISKENGINE_H

#include "Portfolio.h"
#include "MarketData.h"
#include <map>
#include <vector>

struct PortfolioRiskResult {
    double total_pv;
    double total_delta;
    double total_gamma;
    double total_vega;
    double total_theta;
    double value_at_risk_95;
};

class RiskEngine {
public:
    PortfolioRiskResult calculatePortfolioRisk(const Portfolio& portfolio, const std::map<std::string, MarketData>& market_data_map);

private:
    double calculateVaR(const Portfolio& portfolio, const std::map<std::string, MarketData>& market_data_map, int simulations, double confidence_level);
};

#endif
