#include "RiskEngine.h"
#include <numeric>
#include <random>
#include <algorithm>
#include <vector>
#include <cmath>

PortfolioRiskResult RiskEngine::calculatePortfolioRisk(const Portfolio& portfolio, const std::map<std::string, MarketData>& market_data_map) {
    PortfolioRiskResult result = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    
    for (const auto& pair : portfolio.getInstruments()) {
        const auto& instrument = pair.first;
        int quantity = pair.second;
        std::string assetId = instrument->getAssetId();
        
        if (market_data_map.count(assetId)) {
            const MarketData& md = market_data_map.at(assetId);
            result.total_pv += instrument->price(md) * quantity;
            result.total_delta += instrument->delta(md) * quantity;
            result.total_gamma += instrument->gamma(md) * quantity;
            result.total_vega += instrument->vega(md) * quantity;
            result.total_theta += instrument->theta(md) * quantity;
        }
    }
    
    result.value_at_risk_95 = calculateVaR(portfolio, market_data_map, 10000, 0.95);
    
    return result;
}

double RiskEngine::calculateVaR(const Portfolio& portfolio, const std::map<std::string, MarketData>& market_data_map, int simulations, double confidence_level) {
    double initial_portfolio_value = 0.0;
    for (const auto& pair : portfolio.getInstruments()) {
        const auto& instrument = pair.first;
        int quantity = pair.second;
        const MarketData& md = market_data_map.at(instrument->getAssetId());
        initial_portfolio_value += instrument->price(md) * quantity;
    }

    if (initial_portfolio_value == 0) return 0.0;

    std::vector<double> pnl_distribution;
    pnl_distribution.reserve(simulations);

    std::mt19937 generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 0; i < simulations; ++i) {
        double simulated_portfolio_value = 0.0;
        
        for (const auto& pair : portfolio.getInstruments()) {
            const auto& instrument = pair.first;
            int quantity = pair.second;
            const MarketData& md = market_data_map.at(instrument->getAssetId());

            double dt = 1.0 / 252.0;
            double random_shock = distribution(generator);
            double simulated_spot = md.spot_price * std::exp((md.risk_free_rate - 0.5 * md.volatility * md.volatility) * dt + md.volatility * std::sqrt(dt) * random_shock);
            
            MarketData simulated_md = md;
            simulated_md.spot_price = simulated_spot;
            
            simulated_portfolio_value += instrument->price(simulated_md) * quantity;
        }
        
        pnl_distribution.push_back(simulated_portfolio_value - initial_portfolio_value);
    }
    
    std::sort(pnl_distribution.begin(), pnl_distribution.end());
    
    int index = static_cast<int>((1.0 - confidence_level) * simulations);
    return -pnl_distribution[index];
}
