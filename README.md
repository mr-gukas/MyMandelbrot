# Drawing the Mandelbrot Set with SFML

This is a simple C++ program that uses SFML to draw the Mandelbrot set. The Mandelbrot set is a fractal that is created by iterating a complex function on a complex number and determining whether the result remains within a certain boundary. The resulting set is a complex boundary that exhibits self-similarity at different scales.

## Picture of the Mandelbrot set
![Picture](mandelbrot.jpeg)

## Dependencies

This program requires SFML to be installed on your system. You can download SFML from the official website: https://www.sfml-dev.org/download.php

## Build and run

First of all, download this repository:
```bash
git@github.com:mr-gukas/MyMandelbrot.git
cd MyMandelbrot
```
Then you can choose the branch you want:
```bash
git checkout sse    \\ for version with AVX2
git checkout no_sse \\ for version without AVX2
```
Compile the program using CMake: 
```bash
mkdir build 
cd build 
cmake ..
make
```
Run the program:
```bash 
./frac 
```

You can also choose on of compiler optimization flags. Write them in "CMakeLists.txt". For example::
```bash
add_compile_options(-Ofast)
```
## Camera motion

You can move around the fractal using the hot keys: 1) use classic WASD for for vertical and horizontal forward motion; 2) use J for zoom in and K for zoom out;

## Results

**FPS rate:**

| Optimization key | No optimizations | AVX2 float  | 
|------------------|------------------|-------------|
|       None       |        6         |      25     |
|       -Ofast     |        15        |      94     |

| ![NO_SSE](pictures/nosse.jpeg) |
|:--:|
| <b>Screenshot of the version that does not use SIMD</b>|

| ![NO_SSE](pictures/sse.jpeg) |
|:--:|
| <b>Screenshot of the version that uses SIMD</b>|

