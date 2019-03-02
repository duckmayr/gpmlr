# gpmlr

[![Travis build status](https://travis-ci.org/duckmayr/gpmlr.svg?branch=master)](https://travis-ci.org/duckmayr/gpmlr)
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

**gpmlr** allows the [`GPML`](http://www.gaussianprocess.org/gpml/code/matlab/doc/) (Gaussian Processes for Machine Learning) toolbox Matlab function for Gaussian process inference and prediction to be called from R, using R syntax and data structures.

## Installation

`gpmlr` is not on CRAN.
You can install the development version from GitHub via

```r
remotes::install_github("duckmayr/gpmlr")
```

Installation and use on Windows systems is not yet supported.

Please also note that before installing `gplmr`, you must install Octave.
You can obtain Octave from their website, https://www.gnu.org/software/octave/.

If you are on a Debian based system such as Ubuntu, you can also install Octave using your package manager

```shell
sudo apt update
sudo apt install octave liboctave-dev
```

We do provide the `GPML` code so that anyone with Octave installed can use the R package out of the box.

## Using gpmlr

`gpmlr` only has one exported function, `gp()`.
It calls GPML's `gp()` MATLAB function.
Some basic documentation for our R interface is available via 

```r
help("gp")
```

You can also see an example of its use via

```r
example(gp)
```

However, we have not yet added extensive documentation on all the inference methods or mean, covariance, and likelihood functions available for use in `gp()` from `GPML`.
Documentation on these functions will be added in a vignette, but in the meantime is available in the Matlab files themselves under `inst/gpml`.

## Contributing

Before contributing, please consult the contributing guidelines in CONTRIBUTING.md.

## License

GPL (>= 2)

