#include "Instrument.h"
#include "utils/BlackScholes.h"
#include "utils/BinomialTree.h"

EuropeanOption::EuropeanOption(OptionType type, double strike, double time_to_expiry, std::string asset_id)
    : option_type(type), strike_price(strike), time_to_expiry_years(time_to_expiry), underlying_asset_id(asset_id) {}

double EuropeanOption::price(const MarketData& md) const {
    if (option_type == OptionType::Call) {
        return BlackScholes::callPrice(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
    } else {
        return BlackScholes::putPrice(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
    }
}

double EuropeanOption::delta(const MarketData& md) const {
    if (option_type == OptionType::Call) {
        return BlackScholes::callDelta(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
    } else {
        return BlackScholes::putDelta(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
    }
}

double EuropeanOption::gamma(const MarketData& md) const {
    return BlackScholes::gamma(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
}

double EuropeanOption::vega(const MarketData& md) const {
    return BlackScholes::vega(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
}

double EuropeanOption::theta(const MarketData& md) const {
    if (option_type == OptionType::Call) {
        return BlackScholes::callTheta(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
    } else {
        return BlackScholes::putTheta(md.spot_price, strike_price, md.risk_free_rate, time_to_expiry_years, md.volatility);
    }
}

std::string EuropeanOption::getAssetId() const {
    return underlying_asset_id;
}

#include "utils/BinomialTree.h"

AmericanOption::AmericanOption(OptionType type, double strike, double time_to_expiry, 
                               std::string asset_id, int tree_steps)
    : option_type(type), strike_price(strike), time_to_expiry_years(time_to_expiry), 
      underlying_asset_id(asset_id), binomial_steps(tree_steps) {}

double AmericanOption::price(const MarketData& md) const {
    bool is_call = (option_type == OptionType::Call);
    
    if (is_call) {
        return BinomialTree::americanCallPrice(md.spot_price, strike_price, md.risk_free_rate, 
                                               time_to_expiry_years, md.volatility, binomial_steps);
    } else {
        return BinomialTree::americanPutPrice(md.spot_price, strike_price, md.risk_free_rate, 
                                              time_to_expiry_years, md.volatility, binomial_steps);
    }
}

double AmericanOption::delta(const MarketData& md) const {
    bool is_call = (option_type == OptionType::Call);
    return BinomialTree::americanDelta(is_call, md.spot_price, strike_price, md.risk_free_rate, 
                                       time_to_expiry_years, md.volatility, binomial_steps);
}

double AmericanOption::gamma(const MarketData& md) const {
    bool is_call = (option_type == OptionType::Call);
    return BinomialTree::americanGamma(is_call, md.spot_price, strike_price, md.risk_free_rate, 
                                       time_to_expiry_years, md.volatility, binomial_steps);
}

double AmericanOption::vega(const MarketData& md) const {
    bool is_call = (option_type == OptionType::Call);
    return BinomialTree::americanVega(is_call, md.spot_price, strike_price, md.risk_free_rate, 
                                      time_to_expiry_years, md.volatility, binomial_steps);
}

double AmericanOption::theta(const MarketData& md) const {
    bool is_call = (option_type == OptionType::Call);
    return BinomialTree::americanTheta(is_call, md.spot_price, strike_price, md.risk_free_rate, 
                                       time_to_expiry_years, md.volatility, binomial_steps);
}

std::string AmericanOption::getAssetId() const {
    return underlying_asset_id;
}
