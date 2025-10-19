cmake_minimum_required(VERSION 3.10)
project(QuantRiskEngine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")

# Main executable
add_executable(risk_engine ${SOURCES})
target_include_directories(risk_engine PUBLIC ${SOURCE_DIR})

# Platform-specific compiler flags
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    # Help clang find standard library if needed
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
    endif()
endif()

# Enable testing
enable_testing()

# BlackScholes tests
add_executable(test_blackscholes 
    tests/test_blackscholes.cpp
    src/utils/BlackScholes.cpp
)
target_include_directories(test_blackscholes PRIVATE ${SOURCE_DIR})
add_test(NAME BlackScholesTests COMMAND test_blackscholes)

# Portfolio tests
add_executable(test_portfolio
    tests/test_portfolio.cpp
    src/Portfolio.cpp
    src/Instrument.cpp
    src/utils/BlackScholes.cpp
)
target_include_directories(test_portfolio PRIVATE ${SOURCE_DIR})
add_test(NAME PortfolioTests COMMAND test_portfolio)

# RiskEngine tests
add_executable(test_risk_engine
    tests/test_risk_engine.cpp
    src/RiskEngine.cpp
    src/Portfolio.cpp
    src/Instrument.cpp
    src/MarketData.cpp
    src/utils/BlackScholes.cpp
)
target_include_directories(test_risk_engine PRIVATE ${SOURCE_DIR})
add_test(NAME RiskEngineTests COMMAND test_risk_engine)

# Convenience target to run all tests
add_custom_target(run_all_tests
    COMMAND test_blackscholes
    COMMAND test_portfolio
    COMMAND test_risk_engine
    DEPENDS test_blackscholes test_portfolio test_risk_engine
    COMMENT "Running all tests..."
)
