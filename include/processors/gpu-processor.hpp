#pragma once

#include <iostream>
#include <type_traits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <data/cubemap.hpp>
#include <math/vector.hpp>
#include <processors/abstract-processor.hpp>

#include <shaders/formatted/irradiance_vert_glsl.hpp>
#include <shaders/formatted/irradiance_frag_glsl.hpp>
#include <shaders/formatted/tocubemap_vert_glsl.hpp>
#include <shaders/formatted/tocubemap_frag_glsl.hpp>
#include <shaders/formatted/test_glsl.hpp>
#include <shaders/formatted/test_frag_glsl.hpp>
#include <shaders/shader.hpp>

#include <utils/singleton.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

class GPUProcessor : public AbstractProcessor,
                     public Singleton<GPUProcessor>
{
  friend class Singleton<GPUProcessor>;

  public:
    void
    init() override;

    data::Cubemap
    computeDiffuseIS(const data::Cubemap& cubemap,
                     uint16_t nbSamples, int size) override;

    void
    computeSpecularIS() override;

    void
    computeBRDFLUT() override;

  public:
    inline void
    setWindow(GLFWwindow* window){ window_ = window; }

  protected:
    GPUProcessor();

  protected:
    data::Cubemap
    toCubemapImpl(const data::Latlong& map, int size) override;

    data::Latlong
    toEquirectangularImpl(const data::Cubemap& map) override;

  private:
    GLuint
    generateGLCubemap(GLsizei width, GLsizei height);

    data::Cubemap
    generateCubemapFromGLID(GLuint texID, int size);

    GLint
    getUniformId(GLint shaderId, const char* name);

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

    GLFWwindow*     window_;
};

} // namespace process

} // namespace tools

} // namespace albedo
