# Quant Enthusiasts Risk Engine

Quant Enthusiasts Risk Engine is a modular, scalable, and cross-language risk engine designed for the Quant Enthusiasts community. It provides a foundation for risk calculation, portfolio management, and real-time data analysis with C++, Python, and JavaScript components.

## Project Structure

```
Quant-Enthusiasts-Risk-Engine/
├── cpp_engine/
│   ├── src/
│   │   ├── utils/
│   │   │   ├── BlackScholes.cpp
│   │   │   └── BlackScholes.h
│   │   ├── Instrument.cpp
│   │   ├── Instrument.h
│   │   ├── MarketData.cpp
│   │   ├── MarketData.h
│   │   ├── Portfolio.cpp
│   │   ├── Portfolio.h
│   │   ├── RiskEngine.cpp
│   │   ├── RiskEngine.h
│   │   └── main.cpp
│   └── CMakeLists.txt
├── python_api/
│   ├── app.py
│   ├── pybind_wrapper.cpp
│   ├── requirements.txt
│   └── setup.py
└── js_dashboard/
    └── index.html
```

## Requirements

* C++17 compatible compiler
* Python 3.11+
* Node.js 20+
* CMake 3.25+

## Installation

Clone the repository:

```bash
git clone https://github.com/Quant-Enthusiasts/Quant-Enthusiasts-Risk-Engine.git
cd Quant-Enthusiasts-Risk-Engine
```

### Build C++ Engine

If you are using GCC or Clang, comment out the line `#include <corecrt_math_defines.h>` in the source file before building. This header is only needed on Windows with MSVC; other compilers already provide the required math constants.

```bash
mkdir -p cpp_engine/build
cd cpp_engine/build
cmake ..
cmake --build .
./risk-engine
```
#### Using the build script (Recommended)
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
./build.sh --test portfolio     # Portfolio management tests
./build.sh --test risk_engine   # Risk engine tests
```

### Python API

Create a virtual environment and install dependencies:

```bash
python -m venv venv
source venv/bin/activate    # Linux / Mac
venv\Scripts\activate       # Windows
pip install -r python_api/requirements.txt
python python_api/app.py
```

### JS Dashboard

Open `js_dashboard/index.html` in your browser or serve it with a local HTTP server:

```bash
cd js_dashboard
npx serve .
```

## Usage

1. C++ engine handles core risk calculations and portfolio management.
2. Python API exposes engine functionality for scripting, automation, and integration.
3. JS Dashboard provides visualization for portfolio risk metrics in real-time.

## Contribution

1. Fork the repository.
2. Create a branch for your feature or bug fix.
3. Commit changes with clear messages.
4. Push to your branch.
5. Open a pull request.

## License

MIT License

## Contact

Quant Enthusiasts Community: [https://discord.com/invite/z3S9Fguzw3](https://discord.com/invite/z3S9Fguzw3)
