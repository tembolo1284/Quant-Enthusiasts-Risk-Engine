#include "simple_test.h"
#include "../src/utils/BinomialTree.h"
#include "../src/utils/BlackScholes.h"
#include "../src/Instrument.h"
#include "../src/Portfolio.h"
#include "../src/MarketData.h"
#include <cmath>
#include <memory>

void test_american_call_pricing(TestSuite& suite) {
    suite.run_test("American call - ATM", [&]() {
        double price = BinomialTree::americanCallPrice(100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Should be positive and reasonable
        if (price <= 0.0 || price > 100.0) {
            throw std::runtime_error("American call price out of reasonable range");
        }
        
        // For non-dividend paying stocks, American call ≈ European call
        double european_price = BlackScholes::callPrice(100.0, 100.0, 0.05, 1.0, 0.2);
        suite.assert_equal(european_price, price, 0.5, "American call ≈ European call (no dividends)");
    });
    
    suite.run_test("American call - ITM", [&]() {
        double price = BinomialTree::americanCallPrice(110.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Should be at least intrinsic value
        double intrinsic = 10.0;
        if (price < intrinsic) {
            throw std::runtime_error("American call below intrinsic value");
        }
    });
    
    suite.run_test("American call - OTM", [&]() {
        double price = BinomialTree::americanCallPrice(90.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Should have some time value
        if (price <= 0.0) {
            throw std::runtime_error("OTM call should have positive value");
        }
    });
}

void test_american_put_pricing(TestSuite& suite) {
    suite.run_test("American put - ATM", [&]() {
        double price = BinomialTree::americanPutPrice(100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Should be positive
        if (price <= 0.0) {
            throw std::runtime_error("American put should have positive value");
        }
        
        suite.assert_equal(5.8, price, 1.0, "American put price reasonable");
    });
    
    suite.run_test("American put - ITM", [&]() {
        double price = BinomialTree::americanPutPrice(90.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Should be at least intrinsic value
        double intrinsic = 10.0;
        if (price < intrinsic) {
            throw std::runtime_error("American put below intrinsic value");
        }
    });
    
    suite.run_test("American put - Deep ITM shows early exercise premium", [&]() {
        // Deep in-the-money put should show significant early exercise value
        double american_price = BinomialTree::americanPutPrice(70.0, 100.0, 0.05, 1.0, 0.2, 100);
        double european_price = BlackScholes::putPrice(70.0, 100.0, 0.05, 1.0, 0.2);
        
        // American put should be worth MORE than European put
        if (american_price <= european_price) {
            throw std::runtime_error("American put should be >= European put");
        }
        
        // The difference should be noticeable for deep ITM
        double premium = american_price - european_price;
        if (premium < 0.1) {
            throw std::runtime_error("Early exercise premium too small for deep ITM put");
        }
    });
}

void test_early_exercise_premium(TestSuite& suite) {
    suite.run_test("American put > European put (early exercise)", [&]() {
        // For put options, American should be worth more due to early exercise
        double S = 80.0, K = 100.0, r = 0.05, T = 1.0, sigma = 0.3;
        
        double american_price = BinomialTree::americanPutPrice(S, K, r, T, sigma, 150);
        double european_price = BlackScholes::putPrice(S, K, r, T, sigma);
        
        // American >= European
        if (american_price < european_price - 0.01) {
            throw std::runtime_error("American put should be >= European put");
        }
        
        suite.assert_equal(european_price, american_price, 2.0, "American put premium");
    });
    
    suite.run_test("American call ≈ European call (no dividends)", [&]() {
        // For non-dividend stocks, American call shouldn't have early exercise
        double S = 100.0, K = 100.0, r = 0.05, T = 1.0, sigma = 0.2;
        
        double american_price = BinomialTree::americanCallPrice(S, K, r, T, sigma, 150);
        double european_price = BlackScholes::callPrice(S, K, r, T, sigma);
        
        // Should be very close
        suite.assert_equal(european_price, american_price, 0.3, "Call prices similar");
    });
}

void test_convergence(TestSuite& suite) {
    suite.run_test("Convergence with increasing steps", [&]() {
        double S = 100.0, K = 100.0, r = 0.05, T = 1.0, sigma = 0.2;
        
        double price_50 = BinomialTree::americanPutPrice(S, K, r, T, sigma, 50);
        double price_100 = BinomialTree::americanPutPrice(S, K, r, T, sigma, 100);
        double price_200 = BinomialTree::americanPutPrice(S, K, r, T, sigma, 200);
        
        // Prices should converge (differences should decrease)
        double diff_50_100 = std::abs(price_100 - price_50);
        double diff_100_200 = std::abs(price_200 - price_100);
        
        // Later difference should be smaller (convergence)
        if (diff_100_200 > diff_50_100) {
            throw std::runtime_error("Not converging with more steps");
        }
    });
    
    suite.run_test("High step count gives accurate result", [&]() {
        double S = 100.0, K = 100.0, r = 0.05, T = 1.0, sigma = 0.2;
        
        double american_price = BinomialTree::americanCallPrice(S, K, r, T, sigma, 200);
        double european_price = BlackScholes::callPrice(S, K, r, T, sigma);
        
        // With many steps, American call should be very close to European
        suite.assert_equal(european_price, american_price, 0.1, "High accuracy with 200 steps");
    });
}

void test_greeks(TestSuite& suite) {
    suite.run_test("Delta - call option range", [&]() {
        double delta = BinomialTree::americanDelta(true, 100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Delta should be between 0 and 1 for calls
        if (delta < 0.0 || delta > 1.0) {
            throw std::runtime_error("Call delta out of range [0,1]");
        }
        
        suite.assert_equal(0.6, delta, 0.2, "Call delta reasonable");
    });
    
    suite.run_test("Delta - put option range", [&]() {
        double delta = BinomialTree::americanDelta(false, 100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Delta should be between -1 and 0 for puts
        if (delta < -1.0 || delta > 0.0) {
            throw std::runtime_error("Put delta out of range [-1,0]");
        }
        
        suite.assert_equal(-0.4, delta, 0.2, "Put delta reasonable");
    });
    
    suite.run_test("Gamma is positive", [&]() {
        double gamma = BinomialTree::americanGamma(true, 100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        if (gamma <= 0.0) {
            throw std::runtime_error("Gamma should be positive");
        }
    });
    
    suite.run_test("Vega is positive", [&]() {
        double vega = BinomialTree::americanVega(true, 100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        if (vega <= 0.0) {
            throw std::runtime_error("Vega should be positive");
        }
    });
    
    suite.run_test("Theta is negative for long options", [&]() {
        double theta = BinomialTree::americanTheta(true, 100.0, 100.0, 0.05, 1.0, 0.2, 100);
        
        // Long options have negative theta (time decay)
        if (theta >= 0.0) {
            throw std::runtime_error("Theta should be negative for long option");
        }
    });
}

void test_edge_cases(TestSuite& suite) {
    suite.run_test("Zero time to expiry", [&]() {
        double call_price = BinomialTree::americanCallPrice(110.0, 100.0, 0.05, 0.0, 0.2, 100);
        double put_price = BinomialTree::americanPutPrice(90.0, 100.0, 0.05, 0.0, 0.2, 100);
        
        // Should return intrinsic values
        suite.assert_equal(10.0, call_price, 1e-6, "Call intrinsic at expiry");
        suite.assert_equal(10.0, put_price, 1e-6, "Put intrinsic at expiry");
    });
    
    suite.run_test("Zero volatility", [&]() {
        double call_price = BinomialTree::americanCallPrice(110.0, 100.0, 0.05, 1.0, 0.0, 100);
        
        // With zero vol, should be close to intrinsic
        suite.assert_equal(10.0, call_price, 1.0, "Zero vol gives intrinsic");
    });
    
    suite.run_test("Very short expiry", [&]() {
        double price = BinomialTree::americanPutPrice(100.0, 100.0, 0.05, 0.01, 0.2, 100);
        
        // Should have minimal time value
        if (price < 0.0 || price > 5.0) {
            throw std::runtime_error("Short expiry price unreasonable");
        }
    });
}

void test_american_option_class(TestSuite& suite) {
    suite.run_test("AmericanOption - call pricing", [&]() {
        AmericanOption call(OptionType::Call, 100.0, 1.0, "AAPL", 100);
        
        MarketData md;
        md.asset_id = "AAPL";
        md.spot_price = 100.0;
        md.risk_free_rate = 0.05;
        md.volatility = 0.2;
        
        double price = call.price(md);
        
        if (price <= 0.0) {
            throw std::runtime_error("American call price should be positive");
        }
        
        suite.assert_equal(10.0, price, 2.0, "Call price reasonable");
    });
    
    suite.run_test("AmericanOption - put pricing", [&]() {
        AmericanOption put(OptionType::Put, 100.0, 1.0, "AAPL", 100);
        
        MarketData md;
        md.asset_id = "AAPL";
        md.spot_price = 100.0;
        md.risk_free_rate = 0.05;
        md.volatility = 0.2;
        
        double price = put.price(md);
        
        if (price <= 0.0) {
            throw std::runtime_error("American put price should be positive");
        }
        
        suite.assert_equal(5.8, price, 1.0, "Put price reasonable");
    });
    
    suite.run_test("AmericanOption - Greeks calculation", [&]() {
        AmericanOption call(OptionType::Call, 100.0, 1.0, "AAPL", 100);
        
        MarketData md;
        md.asset_id = "AAPL";
        md.spot_price = 100.0;
        md.risk_free_rate = 0.05;
        md.volatility = 0.2;
        
        double delta = call.delta(md);
        double gamma = call.gamma(md);
        double vega = call.vega(md);
        double theta = call.theta(md);
        
        // Sanity checks
        if (delta < 0.0 || delta > 1.0) {
            throw std::runtime_error("Delta out of range");
        }
        if (gamma <= 0.0) {
            throw std::runtime_error("Gamma should be positive");
        }
        if (vega <= 0.0) {
            throw std::runtime_error("Vega should be positive");
        }
        if (theta >= 0.0) {
            throw std::runtime_error("Theta should be negative");
        }
    });
    
    suite.run_test("AmericanOption - getAssetId", [&]() {
        AmericanOption option(OptionType::Call, 100.0, 1.0, "MSFT", 100);
        
        if (option.getAssetId() != "MSFT") {
            throw std::runtime_error("Asset ID mismatch");
        }
    });
}

void test_portfolio_integration(TestSuite& suite) {
    suite.run_test("American options in portfolio", [&]() {
        Portfolio portfolio;
        
        // Add American call
        portfolio.addInstrument(
            std::make_unique<AmericanOption>(OptionType::Call, 100.0, 1.0, "AAPL", 100),
            10
        );
        
        // Add American put
        portfolio.addInstrument(
            std::make_unique<AmericanOption>(OptionType::Put, 100.0, 1.0, "AAPL", 100),
            5
        );
        
        const auto& instruments = portfolio.getInstruments();
        suite.assert_equal(2, static_cast<double>(instruments.size()), 1e-10, "Portfolio size");
    });
    
    suite.run_test("Mixed European and American options", [&]() {
        Portfolio portfolio;
        
        // Add European call
        portfolio.addInstrument(
            std::make_unique<EuropeanOption>(OptionType::Call, 100.0, 1.0, "AAPL"),
            5
        );
        
        // Add American put
        portfolio.addInstrument(
            std::make_unique<AmericanOption>(OptionType::Put, 100.0, 1.0, "AAPL", 100),
            5
        );
        
        MarketData md;
        md.asset_id = "AAPL";
        md.spot_price = 100.0;
        md.risk_free_rate = 0.05;
        md.volatility = 0.2;
        
        const auto& instruments = portfolio.getInstruments();
        
        // Both should price successfully
        double euro_price = instruments[0].first->price(md);
        double american_price = instruments[1].first->price(md);
        
        if (euro_price <= 0.0 || american_price <= 0.0) {
            throw std::runtime_error("Both options should have positive prices");
        }
    });
}

void test_different_step_counts(TestSuite& suite) {
    suite.run_test("Different step counts give similar results", [&]() {
        AmericanOption option1(OptionType::Put, 100.0, 1.0, "AAPL", 50);
        AmericanOption option2(OptionType::Put, 100.0, 1.0, "AAPL", 100);
        AmericanOption option3(OptionType::Put, 100.0, 1.0, "AAPL", 200);
        
        MarketData md;
        md.asset_id = "AAPL";
        md.spot_price = 100.0;
        md.risk_free_rate = 0.05;
        md.volatility = 0.2;
        
        double price1 = option1.price(md);
        double price2 = option2.price(md);
        double price3 = option3.price(md);
        
        // Prices should be similar (within 5%)
        double diff_1_2 = std::abs(price2 - price1) / price2;
        double diff_2_3 = std::abs(price3 - price2) / price3;
        
        if (diff_1_2 > 0.05) {
            throw std::runtime_error("Price difference too large between 50 and 100 steps");
        }
        if (diff_2_3 > 0.02) {
            throw std::runtime_error("Price difference too large between 100 and 200 steps");
        }
    });
}

int main() {
    TestSuite suite;
    
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  Binomial Tree & American Options Test Suite" << std::endl;
    std::cout << std::string(60, '=') << "\n" << std::endl;
    
    test_american_call_pricing(suite);
    test_american_put_pricing(suite);
    test_early_exercise_premium(suite);
    test_convergence(suite);
    test_greeks(suite);
    test_edge_cases(suite);
    test_american_option_class(suite);
    test_portfolio_integration(suite);
    test_different_step_counts(suite);
    
    suite.print_summary();
    
    return suite.all_passed() ? 0 : 1;
}
