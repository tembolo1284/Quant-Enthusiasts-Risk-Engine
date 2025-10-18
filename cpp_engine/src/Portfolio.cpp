#include "Portfolio.h"

void Portfolio::addInstrument(std::unique_ptr<Instrument> instrument, int quantity) {
    instruments.emplace_back(std::move(instrument), quantity);
}

const std::vector<std::pair<std::unique_ptr<Instrument>, int>>& Portfolio::getInstruments() const {
    return instruments;
}
