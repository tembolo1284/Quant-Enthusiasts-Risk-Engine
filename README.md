# Quant Enthusiasts Risk Engine

Quant Enthusiasts Risk Engine is a modular, scalable, and cross-language risk engine designed for the Quant Enthusiasts community. It provides a foundation for risk calculation, portfolio management, and real-time data analysis with C++, Python, and JavaScript components.

## Features

- **Multiple Pricing Models**: Black-Scholes, Binomial Tree, Merton Jump Diffusion
- **Option Types**: European and American options (Call/Put)
- **Portfolio Analytics**: Greeks (Delta, Gamma, Vega, Theta), PV, and VaR calculations
- **Monte Carlo Simulation**: Configurable simulations for Value at Risk (VaR)
- **Real-time Dashboard**: Interactive web interface for portfolio management
- **REST API**: Flask-based API with comprehensive validation
- **Cross-platform**: Works on Windows, Linux, and macOS

## Project Structure

```
Quant-Enthusiasts-Risk-Engine/
├── cpp_engine/
│   ├── src/
│   │   ├── utils/
│   │   │   ├── BlackScholes.cpp
│   │   │   ├── BlackScholes.h
│   │   │   ├── BinomialTree.cpp
│   │   │   ├── BinomialTree.h
│   │   │   ├── JumpDiffusion.cpp
│   │   │   ├── JumpDiffusion.h
│   │   │   ├── ImpliedVolatilitySurface.cpp
│   │   │   └── ImpliedVolatilitySurface.h
│   │   ├── Instrument.cpp
│   │   ├── Instrument.h
│   │   ├── MarketData.cpp
│   │   ├── MarketData.h
│   │   ├── Portfolio.cpp
│   │   ├── Portfolio.h
│   │   ├── RiskEngine.cpp
│   │   ├── RiskEngine.h
│   │   └── main.cpp
│   ├── CMakeLists.txt
│   └── build.sh
├── python_api/
│   ├── app.py
│   ├── pybind_wrapper.cpp
│   ├── requirements.txt
│   └── setup.py
└── js_dashboard/
    └── index.html
```

## Requirements

* C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
* Python 3.11+
* CMake 3.25+
* pybind11 2.6+
* Flask 2.0+

## Installation

Clone the repository:

```bash
git clone https://github.com/Quant-Enthusiasts/Quant-Enthusiasts-Risk-Engine.git
cd Quant-Enthusiasts-Risk-Engine
```

### Build C++ Engine

**Important**: If you are using GCC or Clang, comment out the line `#include <corecrt_math_defines.h>` in the source file before building. This header is only needed on Windows with MSVC; other compilers already provide the required math constants.

#### Manual Build

```bash
mkdir -p cpp_engine/build
cd cpp_engine/build
cmake ..
cmake --build .
./risk-engine
```

#### Using the Build Script (Recommended)

The easiest way to build and test the C++ engine:

```bash
cd cpp_engine
chmod +x build.sh
./build.sh --all    # Clean, build, and run all tests
```

Available build options:

```bash
./build.sh                      # Clean and build (default)
./build.sh --compiler clang     # Build with clang
./build.sh --test               # Build and run all tests
./build.sh --test blackscholes  # Run specific test
./build.sh --clean              # Clean build directory
./build.sh --help               # Show all options
```

### Testing

The C++ engine includes a comprehensive test suite with no external dependencies. Tests cover:

- **BlackScholes**: Option pricing and Greeks calculations
- **BinomialTree**: Binomial tree pricing for American/European options
- **JumpDiffusion**: Merton jump diffusion model
- **Portfolio**: Portfolio management and aggregation
- **RiskEngine**: Risk metrics including VaR

Run all tests:

```bash
cd cpp_engine
./build.sh --test
```

Run individual test suites:

```bash
./build.sh --test blackscholes  # Black-Scholes model tests
./build.sh --test binomial      # Binomial tree tests
./build.sh --test portfolio     # Portfolio management tests
./build.sh --test risk_engine   # Risk engine tests
```

### Python API Setup

Create a virtual environment and install dependencies:

```bash
cd python_api
python -m venv venv
source venv/bin/activate    # Linux / Mac
venv\Scripts\activate       # Windows
pip install -r requirements.txt
```

Build the Python bindings:

```bash
python setup.py build_ext --inplace
```

Run the Flask API server:

```bash
python app.py
```

The API will be available at `http://127.0.0.1:5000`

### JS Dashboard Setup

Open `js_dashboard/index.html` in your browser or serve it with a local HTTP server:

```bash
cd js_dashboard
npx serve .
```

Then navigate to `http://localhost:3000` in your browser.

**Note**: Ensure the Python API server is running before using the dashboard.

## Usage

### 1. C++ Engine (Standalone)

The C++ engine can be used directly for high-performance calculations:

```bash
cd cpp_engine/build
./risk-engine
```

This runs a comprehensive demonstration of all engine features including:
- Basic portfolio risk analysis
- Multiple pricing model comparisons
- American options pricing
- Market data management
- Complex multi-asset portfolios
- Error handling validation

### 2. Python API

#### Health Check

```bash
curl http://127.0.0.1:5000/health
```

#### Calculate Portfolio Risk

```bash
curl -X POST http://127.0.0.1:5000/calculate_risk \
  -H "Content-Type: application/json" \
  -d '{
    "portfolio": [
      {
        "type": "call",
        "strike": 100,
        "expiry": 1.0,
        "asset_id": "AAPL",
        "quantity": 100,
        "style": "european"
      }
    ],
    "market_data": {
      "AAPL": {
        "spot": 105,
        "rate": 0.05,
        "vol": 0.25
      }
    }
  }'
```

#### Price Single Option

```bash
curl -X POST http://127.0.0.1:5000/price_option \
  -H "Content-Type: application/json" \
  -d '{
    "type": "call",
    "strike": 100,
    "expiry": 1.0,
    "asset_id": "AAPL",
    "style": "european",
    "pricing_model": "blackscholes",
    "market_data": {
      "spot": 105,
      "rate": 0.05,
      "vol": 0.25
    }
  }'
```

### 3. Web Dashboard

1. Start the Python API server: `python python_api/app.py`
2. Open the dashboard: `js_dashboard/index.html`
3. Define market data for your assets
4. Build your portfolio by adding options
5. Calculate portfolio risk metrics

The dashboard provides:
- Real-time API health monitoring
- Interactive portfolio builder
- Dynamic market data management
- Visual risk metrics display
- Comprehensive error handling

## API Endpoints

### Health Check
- **GET** `/health` - Check API status and version

### Risk Calculation
- **POST** `/calculate_risk` - Calculate portfolio risk metrics
- **POST** `/price_option` - Price a single option

### Portfolio Analysis
- **POST** `/portfolio/net_position/<asset_id>` - Get net position for asset
- **POST** `/portfolio/summary` - Get portfolio summary statistics

## Pricing Models

### Black-Scholes
Classical option pricing model for European options.

### Binomial Tree
Discrete-time model supporting both European and American options with early exercise.

### Merton Jump Diffusion
Extension of Black-Scholes incorporating discontinuous price jumps.

## Configuration

### VaR Parameters (Python API)

```json
{
  "var_parameters": {
    "simulations": 100000,
    "confidence": 0.95,
    "time_horizon": 1.0,
    "seed": 42
  }
}
```

### Option Parameters

```json
{
  "type": "call",           // "call" or "put"
  "strike": 100.0,          // Strike price
  "expiry": 1.0,            // Time to expiry (years)
  "asset_id": "AAPL",       // Asset identifier
  "quantity": 100,          // Position size (positive=long, negative=short)
  "style": "european",      // "european" or "american"
  "pricing_model": "blackscholes",  // "blackscholes", "binomial", or "jumpdiffusion"
  "binomial_steps": 100,    // Number of steps for binomial tree (optional)
  "jump_parameters": {      // Parameters for jump diffusion (optional)
    "lambda": 2.0,
    "mean": -0.05,
    "vol": 0.15
  }
}
```

## Troubleshooting

### Build Issues

**Problem**: CMake cannot find compiler
```bash
cmake -DCMAKE_CXX_COMPILER=g++ ..
```

**Problem**: Math constants not defined (Linux/Mac)
- Comment out `#include <corecrt_math_defines.h>` in source files

### Python API Issues

**Problem**: Module 'quant_risk_engine' not found
```bash
cd python_api
python setup.py build_ext --inplace
```

**Problem**: API returns "API Offline"
- Check if Flask server is running: `python python_api/app.py`
- Verify port 5000 is not in use
- Check firewall settings

### Dashboard Issues

**Problem**: CORS errors in browser console
- Serve the dashboard using a web server instead of opening directly
- Use `npx serve .` or similar

**Problem**: API connection fails
- Ensure Flask API is running on `http://127.0.0.1:5000`
- Check browser console for specific error messages

## Performance Notes

- **C++ Engine**: Optimized for low-latency calculations, suitable for production use
- **Python API**: Good for integration and scripting; performance depends on portfolio size
- **VaR Calculations**: 100,000 simulations typically complete in <1 second
- **Portfolio Size**: Tested with portfolios up to 1,000 instruments

## Contribution

We welcome contributions from the community!

1. Fork the repository
2. Create a branch for your feature or bug fix (`git checkout -b feature/amazing-feature`)
3. Commit changes with clear messages (`git commit -m 'Add amazing feature'`)
4. Push to your branch (`git push origin feature/amazing-feature`)
5. Open a pull request

### Contribution Guidelines

- Follow existing code style and conventions
- Add tests for new features
- Update documentation as needed
- Ensure all tests pass before submitting PR

## License

MIT License

Copyright (c) 2025 Quant Enthusiasts

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Contact

- **Discord Community**: [https://discord.com/invite/z3S9Fguzw3](https://discord.com/invite/z3S9Fguzw3)
- **GitHub Issues**: [https://github.com/Quant-Enthusiasts/Quant-Enthusiasts-Risk-Engine/issues](https://github.com/Quant-Enthusiasts/Quant-Enthusiasts-Risk-Engine/issues)

## Acknowledgments

- Built by the Quant Enthusiasts community
- Powered by modern C++17, Python, and web technologies
- Inspired by industry-standard quantitative finance practices