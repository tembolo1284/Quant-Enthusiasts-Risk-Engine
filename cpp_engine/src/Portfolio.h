#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Instrument.h"
#include <vector>
#include <memory>

class Portfolio {
public:
    void addInstrument(std::unique_ptr<Instrument> instrument, int quantity);
    const std::vector<std::pair<std::unique_ptr<Instrument>, int>>& getInstruments() const;
private:
    std::vector<std::pair<std::unique_ptr<Instrument>, int>> instruments;
};

#endif
