#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "Instrument.h"
#include "Portfolio.h"
#include "RiskEngine.h"
#include "MarketData.h"
#include <memory>

namespace py = pybind11;

PYBIND11_MODULE(quant_risk_engine, m) {
    m.doc() = "Python bindings for the Quant Enthusiasts Risk Engine";

    py::enum_<OptionType>(m, "OptionType")
        .value("Call", OptionType::Call)
        .value("Put", OptionType::Put)
        .export_values();

    py::class_<MarketData>(m, "MarketData")
        .def(py::init<>())
        .def_readwrite("asset_id", &MarketData::asset_id)
        .def_readwrite("spot_price", &MarketData::spot_price)
        .def_readwrite("risk_free_rate", &MarketData::risk_free_rate)
        .def_readwrite("volatility", &MarketData::volatility);
    
    py::class_<Instrument, std::shared_ptr<Instrument>>(m, "Instrument");

    py::class_<EuropeanOption, Instrument, std::shared_ptr<EuropeanOption>>(m, "EuropeanOption")
        .def(py::init<OptionType, double, double, std::string>());

    py::class_<Portfolio>(m, "Portfolio")
        .def(py::init<>())
        .def("add_instrument", [](Portfolio &p, EuropeanOption &instr, int quantity) {
            auto owned_instr = std::make_unique<EuropeanOption>(instr);
            p.addInstrument(std::move(owned_instr), quantity);
        });

    py::class_<PortfolioRiskResult>(m, "PortfolioRiskResult")
        .def(py::init<>())
        .def_readwrite("total_pv", &PortfolioRiskResult::total_pv)
        .def_readwrite("total_delta", &PortfolioRiskResult::total_delta)
        .def_readwrite("total_gamma", &PortfolioRiskResult::total_gamma)
        .def_readwrite("total_vega", &PortfolioRiskResult::total_vega)
        .def_readwrite("total_theta", &PortfolioRiskResult::total_theta)
        .def_readwrite("value_at_risk_95", &PortfolioRiskResult::value_at_risk_95);

    py::class_<RiskEngine>(m, "RiskEngine")
        .def(py::init<>())
        .def("calculate_portfolio_risk", &RiskEngine::calculatePortfolioRisk);
}
