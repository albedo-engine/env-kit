#include <processors/gpu-processor.hpp>

namespace albedo
{

namespace tools
{

namespace process
{

const glm::mat4 GPUProcessor::CAMERA_VIEWS[] =
{
  glm::lookAt(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3( 1.0f,  0.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f)
  ),
  glm::lookAt(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(0.0f, -1.0f,  0.0f)
  ),
  glm::lookAt(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3( 0.0f,  1.0f,  0.0f),
    glm::vec3(0.0f,  0.0f,  1.0f)
  ),
  glm::lookAt(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3( 0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f,  0.0f, -1.0f)
  ),
  glm::lookAt(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3( 0.0f,  0.0f,  1.0f),
    glm::vec3(0.0f, -1.0f,  0.0f)
  ),
  glm::lookAt(
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3( 0.0f,  0.0f, -1.0f),
    glm::vec3(0.0f, -1.0f,  0.0f)
  )
};

const float GPUProcessor::CUBE_VERTICES[] =
{
  // back face
  -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
  -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
  -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
  // front face
  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
  -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
  // left face
  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
  -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
  -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
  -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
  -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
  // right face
  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
  // bottom face
  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
  -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
  -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
  // top face
  -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
  -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
  -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
};

const glm::mat4 GPUProcessor::CAM_PROJ(
  glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 2.0f)
);


GPUProcessor::GPUProcessor()
            : shaderEquiToCubemap_{shader_source_tocubemap_vert_glsl,
               shader_source_tocubemap_frag_glsl}
            , shaderIrradiance_{shader_source_irradiance_vert_glsl,
                                shader_source_irradiance_frag_glsl}
{ }

void
GPUProcessor::init()
{
  if (!shaderEquiToCubemap_.compile())
  {
    std::cerr << "Error compiling Equirectangular shader:" << std::endl;
    shaderEquiToCubemap_.printError();
  }

  if (!shaderIrradiance_.compile())
  {
    std::cerr << "Error compiling Irradiance shader:" << std::endl;
    shaderIrradiance_.printError();
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Creates FBO and RBO used to holds the equiplanar map
  // to cubemap and the cubemap irradiance.
  glGenFramebuffers(1, &fbo_);
  glGenRenderbuffers(1, &rbo_);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                            GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER,
                            rbo_);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  // Creates Cube VAO & VBO.
  // This cube is mainly used to display cubemap or
  // to render cubemap from an equirectangular form.
  glGenVertexArrays(1, &cubeVAO_);
  glGenBuffers(1, &cubeVBO_);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO_);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO_);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

data::Cubemap
GPUProcessor::computeDiffuseIS(const data::Cubemap& cubemap,
                               uint16_t nbSamples, int size)
{
  // Generates a Cubemap GPU-side from a
  // CPU-side Cubemap.
  const auto& mipmaps = cubemap.getMipmaps();
  auto& data = mipmaps[0];
  int cubeSize = cubemap.getWidth();

  GLuint cubemapID = 0;
  glGenTextures(1, &cubemapID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
  for(GLuint i = 0; i < data.size(); i++)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                 GL_RGB16F, cubeSize, cubeSize, 0, GL_RGB, GL_FLOAT, data[i]);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  // Creates the VRAM-stored cubemap containing
  // the irradiance for each pixel.
  unsigned int irradianceMapId = this->generateGLCubemap(size, size);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);

  // Binds the shader computing the irradiance map.
  GLint shaderId = shaderIrradiance_.id();
  shaderIrradiance_.use();

  // Binds equirectangular texture in
  // the first texture unit.
  GLint uProjId = this->getUniformId(shaderId, "uProjection");
  GLint uMapId = this->getUniformId(shaderId, "uMap");

  // Binds the cubemap texture in
  // the first texture unit.
  glViewport(0, 0, size, size);
  glUniformMatrix4fv(uProjId, 1, GL_FALSE, &CAM_PROJ[0][0]);
  glUniform1i(uMapId, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

  for (unsigned int i = 0; i < 6; ++i)
  {
    GLint uViewId = this->getUniformId(shaderId, "uView");
    GLint uNbSamples = this->getUniformId(shaderId, "uNbSamples");

    glUniformMatrix4fv(uViewId, 1, GL_FALSE, &CAMERA_VIEWS[i][0][0]);
    glUniform1f(uNbSamples, (float)nbSamples);

    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           irradianceMapId, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(cubeVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  auto result = this->generateCubemapFromGLID(irradianceMapId, size);

  glDeleteTextures(1, &cubemapID);
  glDeleteTextures(1, &irradianceMapId);

  return result;
}

void
GPUProcessor::computeSpecularIS()
{
  throw "Not implemented.";
}

void
GPUProcessor::computeBRDFLUT()
{
  throw "Not implemented.";
}

data::Cubemap
GPUProcessor::toCubemapImpl(const data::Latlong& map, int size)
{
  const auto& mipmaps = map.getMipmaps();
  float* equiplanarData = mipmaps[0];
  // Creates GPU texture storing the
  // HDR equiplanar texture in VRAM.
  GLuint equiplanarTextId = 0;
  glGenTextures(1, &equiplanarTextId);
  glBindTexture(GL_TEXTURE_2D, equiplanarTextId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, map.getWidth(),
               map.getHeight(), 0,GL_RGB, GL_FLOAT, equiplanarData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint cubemapId = this->generateGLCubemap(size, size);

  // Binds the shader writing from equiplanar to a cubemap.
  // Sends required uniforms, such as equirectangular map,
  // perspective matrix, current camera view, ...
  GLuint shaderId = shaderEquiToCubemap_.id();
  shaderEquiToCubemap_.use();

  // Binds equirectangular texture in
  // the first texture unit.
  GLint uProjId = this->getUniformId(shaderId, "uProjection");
  GLint uMapId = this->getUniformId(shaderId, "uMap");

  glUniformMatrix4fv(uProjId, 1, GL_FALSE, &CAM_PROJ[0][0]);
  glUniform1i(uMapId, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, equiplanarTextId);

  glViewport(0, 0, size, size);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
  for (unsigned int i = 0; i < 6; ++i)
  {
    GLint uViewId = glGetUniformLocation(shaderId, "uView");
    glUniformMatrix4fv(uViewId, 1, GL_FALSE, &CAMERA_VIEWS[i][0][0]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                           cubemapId, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(cubeVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Gets back data from the VRAM into a STL vector.
  std::vector<float*> faces;
  for (auto i = 0; i < 6; i++)
  {
    auto* data = new float[size * size * 3];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                  GL_RGB, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    faces.push_back(data);
  }

  data::Cubemap result = this->generateCubemapFromGLID(cubemapId, size);

  glDeleteTextures(1, &cubemapId);
  glDeleteTextures(1, &equiplanarTextId);

  return result;
}

data::Latlong
GPUProcessor::toEquirectangularImpl(const data::Cubemap& map)
{
  throw "Not implemented.";
}

GLuint
GPUProcessor::generateGLCubemap(GLsizei width, GLsizei height)
{
  // Creates the Cubemap stored on the GPU.
  unsigned int mapId;
  glGenTextures(1, &mapId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mapId);
  for (unsigned int i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                 GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return mapId;
}

data::Cubemap
GPUProcessor::generateCubemapFromGLID(GLuint texID, int size)
{
  std::vector<float*> faces;
  for (auto i = 0; i < 6; i++)
  {
    auto* data = new float[size * size * 3];
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                  GL_RGB, GL_FLOAT, data);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    faces.push_back(data);
  }
  return data::Cubemap(faces, size, 3);
}

GLint
GPUProcessor::getUniformId(GLint shaderId, const char *name)
{
  GLint id = glGetUniformLocation(shaderId, name);
  if (id < 0)
  {
    std::cerr << "GPUProcessor: attribute '" << name
              << "' not found. Processing may not output a good result."
              << std::endl;
  }

  return id;
}

} // process

} // tools

} // albedo
