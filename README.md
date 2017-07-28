# env-kit

This project is aims at generating, from a given map, its BRDF look-up texture, its irradiance map and the prefiltered environment.
You can use the project as a third-party library, directly integrated in your custom engine / project, or, if you want to use it offline, you can use the generated *albedo-prefilter* binary linked to the library.

You can choose whether to compute through the CPU or the GPU.

Supported input/output formats:
* .hdr

Supported input image type:
* [Latlong](http://gl.ict.usc.edu/HDRShop/tutorial/images/uffizi_latlong.jpg)
* [Cubecross](https://learnopengl.com/img/advanced/cubemaps_skybox.png)
* 6 faces (Splitted Cubemap)

Supported output image type:
* [Cubecross](https://learnopengl.com/img/advanced/cubemaps_skybox.png)
* 6 faces (Splitted Cubemap)

## Building

### Dependencies

According to how you compile the library or the albedo-prefilter binary, you will need the following dependencies:
* [TBB](https://www.threadingbuildingblocks.org/) (Only if you want CPU multithreading)
* [GLEW](http://glew.sourceforge.net/) (Only if you want to use the GPU)
* [GLFW](http://www.glfw.org/) (Only if you want to use the GPU)

### How to build

#### Builds only the library
```sh
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make albedo_envtools
```

#### Builds the binary tool using the library
```sh
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

#### Builds the library/binary without dependencies
If you do not want to use the GPU, or even the CPU cpu parallelism, you can build the library / binary accordingly.
You can set the value `-DALBEDO_TOOLS_MODE`to one of the following:
* NONE -> Only CPU monothreading is used and no dependency is needed
* ALL -> GPU and CPU multithreading
* TBB_ONLY -> Only CPU multithreading
* GPU_ONLY -> Only GPU

For instance, if you only plan to use the GPU:
```sh
$ cmake -DCMAKE_BUILD_TYPE=Release .. -DALBEDO_TOOLS_MODE=GPU_ONLY
$ make
```

## Binary Usage


## TODO

* [ ] CPUProcessor: Add bilinear interpolation to Cubemap generation
* [ ] GPUProcessor: Add specular computation
* [ ] CPUProcessor: Add specular computation
* [ ] Library: Track every memory leaks
* [ ] Library: Implements PIMPL
* [ ] Binary: Improve command line info
