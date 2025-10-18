#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "MarketData.h"
#include <string>

enum class OptionType { Call, Put };

class Instrument {
public:
    virtual ~Instrument() = default;
    virtual double price(const MarketData& md) const = 0;
    virtual double delta(const MarketData& md) const = 0;
    virtual double gamma(const MarketData& md) const = 0;
    virtual double vega(const MarketData& md) const = 0;
    virtual double theta(const MarketData& md) const = 0;
    virtual std::string getAssetId() const = 0;
};

class EuropeanOption : public Instrument {
public:
    EuropeanOption(OptionType type, double strike, double time_to_expiry, std::string asset_id);
    
    double price(const MarketData& md) const override;
    double delta(const MarketData& md) const override;
    double gamma(const MarketData& md) const override;
    double vega(const MarketData& md) const override;
    double theta(const MarketData& md) const override;
    std::string getAssetId() const override;

    OptionType option_type;
    double strike_price;
    double time_to_expiry_years;
    std::string underlying_asset_id;
};

#endif
