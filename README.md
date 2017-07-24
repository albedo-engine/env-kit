# env-kit

This project is an open-source library aiming at generating, from a given map, its BRDF look-up texture, its irradiance map and the prefiltered environment.

## TODO

* [ ] GPUProcessor: Check irradiance map feeling wrong on GPU
* [ ] CPUProcessor: Add TBB parrallel_for
* [ ] GPUProcessor: Remove #indef and use std::integral_constant
* [ ] CPUProcessor: Add bilinear interpolation to Cubemap generation
* [ ] GPUProcessor: Add specular computation
* [ ] CPUProcessor: Add specular computation
