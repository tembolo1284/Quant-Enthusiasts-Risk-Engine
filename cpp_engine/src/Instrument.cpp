#include "Instrument.h"
#include "utils/BlackScholes.h"

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
