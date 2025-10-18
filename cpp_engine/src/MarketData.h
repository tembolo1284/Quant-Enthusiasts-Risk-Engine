#ifndef MARKETDATA_H
#define MARKETDATA_H

#include <string>

struct MarketData {
    std::string asset_id;
    double spot_price;
    double risk_free_rate;
    double volatility;
};

#endif
