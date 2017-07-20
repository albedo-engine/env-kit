#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <data/cubemap.hpp>
#include <math/vector.hpp>
#include <processors/abstract-processor.hpp>

#include <shaders/formatted/irradiance_vert_glsl.hpp>
#include <shaders/formatted/irradiance_frag_glsl.hpp>
#include <shaders/formatted/tocubemap_vert_glsl.hpp>
#include <shaders/formatted/tocubemap_frag_glsl.hpp>
#include <shaders/shader.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class GPUProcessor : public AbstractProcessor
{
  public:
    GPUProcessor();

  public:
    void
    init();

    data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap, uint16_t nbSamples) override;

    data::Cubemap
    computeDiffuseIS(const data::Equirectangular& cubemap,
                     uint16_t nbSamples) override;

    void
    computeSpecularIS() override;

    void
    computeBRDFLUT() override;

    data::Cubemap
    toCubemap(const data::Equirectangular& map);

    data::Equirectangular
    toEquirectangular(const data::Cubemap& map);

  private:
    GLuint
    generateGLCubemap(GLsizei width, GLsizei height);

  private:
    static const glm::mat4  CAMERA_VIEWS[6];
    static const glm::mat4  CAM_PROJ;
    static const float      CUBE_VERTICES[288];

  private:
    shader::Shader  shaderEquiToCubemap_;
    shader::Shader  shaderIrradiance_;

    GLuint          fbo_;
    GLuint          rbo_;

    GLuint          cubeVAO_;
    GLuint          cubeVBO_;
};

} // namespace process

} // namespace tools

} // namespace albedo
