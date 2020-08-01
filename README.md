<p align="center"> <img src="logo.png" alt="rgbtracer logo"> </p>

# rgbtracer
Toy Renderer / Path Tracer


## Building

Configuring the generator:

```cmake -G "Visual Studio 16 2019" -A x64 -B bin -S src```

You can also use "Visual Studio 15 2017", for example. Other generators can be found in CMake's generator section in its documentation.

Building it:

```cmake --build bin --config Release```