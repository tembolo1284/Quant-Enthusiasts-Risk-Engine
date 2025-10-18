from setuptools import setup, Extension
import pybind11

cpp_args = ['-std=c++17', '-Wall', '-Wextra', '-pedantic']

ext_modules = [
    Extension(
        'quant_risk_engine',
        sources=[
            'pybind_wrapper.cpp',
            '../cpp_engine/src/Instrument.cpp',
            '../cpp_engine/src/MarketData.cpp',
            '../cpp_engine/src/Portfolio.cpp',
            '../cpp_engine/src/RiskEngine.cpp',
            '../cpp_engine/src/utils/BlackScholes.cpp'
        ],
        include_dirs=[pybind11.get_include(), '../cpp_engine/src'],
        language='c++',
        extra_compile_args=cpp_args,
    ),
]

setup(
    name='quant_risk_engine',
    version='1.0',
    description='Python bindings for the Quant Enthusiasts Risk Engine',
    ext_modules=ext_modules,
)
