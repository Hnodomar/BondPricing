# Bond Pricing

A simple Corporate and Treasury bond pricing library. The library provides an interface via Boost Python for usage in Python.

## Features

* Yield Curve Generation
* Yield and Price interpolation
* Dirty and Clean pricing
* Yield to Maturity (YTM) Safe-Newton approximations
* Flat and General Term Structure

## Usage

TO-DO

## Building The Bond Pricing Library

Note that this library uses Python3 in its linkage with Boost::Python, and a Boost Configuration of Python3 is required (by default, Boost builds with Python2).

To build Boost with Python3 from source, first edit user-config.jam in tools/build/src to contain:

```
using python : 3.8 : /usr/bin/python3 : /usr/include/python3.8 : /usr/lib ;
```
Then build Boost:
```
./bootstrap.sh --prefix=INSTALL_DIR
./b2 --with-python cxxflags="-std=c++11 -fPIC" variant=release stage
./b2 --with-python cxxflags="-std=c++11 -fPIC" variant=release install
```

## Dependencies

* Python 3: https://www.python.org/
* Boost: https://www.boost.org/
* Pytest (tests): https://docs.pytest.org/en/6.2.x/
* Cmake (build system): https://cmake.org/
