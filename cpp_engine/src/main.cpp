#include <iostream>
#include <vector>
#include "Portfolio.h"
#include "Instrument.h"
#include "MarketData.h"
#include "RiskEngine.h"

int main() {
    Portfolio my_portfolio;
    my_portfolio.addInstrument(std::make_unique<EuropeanOption>(OptionType::Call, 105.0, 0.5, "AAPL"), 100);
    my_portfolio.addInstrument(std::make_unique<EuropeanOption>(OptionType::Put, 95.0, 0.25, "GOOG"), -50);
    
    std::map<std::string, MarketData> market_data;
    market_data["AAPL"] = {"AAPL", 100.0, 0.05, 0.20};
    market_data["GOOG"] = {"GOOG", 102.0, 0.05, 0.25};
    
    RiskEngine engine;
    PortfolioRiskResult results = engine.calculatePortfolioRisk(my_portfolio, market_data);
    
    std::cout << "Portfolio Risk Analysis:" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "Total PV: " << results.total_pv << std::endl;
    std::cout << "Total Delta: " << results.total_delta << std::endl;
    std::cout << "Total Gamma: " << results.total_gamma << std::endl;
    std::cout << "Total Vega: " << results.total_vega << std::endl;
    std::cout << "Total Theta: " << results.total_theta << std::endl;
    std::cout << "95% VaR (1-day): " << results.value_at_risk_95 << std::endl;
    
    return 0;
}
