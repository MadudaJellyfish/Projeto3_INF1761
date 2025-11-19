
#ifdef _WIN32
#include <windows.h>
// Use glad to initialize OpenGL context on Windows
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#elif __APPLE__
#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#elif __linux__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "arcball.h"
#include "scene.h"
#include "state.h"
#include "camera3d.h"
#include "material.h"
#include "texture.h"
#include "transform.h"
#include "cube.h"
#include "quad.h"
#include "sphere.h"
#include "error.h"
#include "shader.h"
#include "light.h"
#include "light.h"
#include "polyoffset.h"

#include <iostream>
#include <cassert>

static float viewer_pos[3] = {2.0f, 3.5f, 4.0f};

static ScenePtr scene;
static Camera3DPtr camera;
static ArcballPtr arcball;
static LightPtr light;

glm::mat4 shadowMatrix(const glm::vec4& n, const glm::vec4& l) {
    float ndotl = glm::dot(glm::vec3(n), glm::vec3(l));
    return glm::mat4(
        glm::vec4(ndotl + n.w - l.x * n.x,   -l.y * n.x, -l.z * n.x, -n.x),
        glm::vec4(-l.x * n.y, ndotl + n.w - l.y * n.y, -l.z * n.y, -n.y),
        glm::vec4(-l.x * n.z, -l.y * n.z, ndotl + n.w - l.z * n.z, -n.z),
        glm::vec4(-l.x * n.w, -l.y * n.w, -l.z * n.w, ndotl)
    );
}

static void initialize (void)
{
  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);

  // create objects
  camera = Camera3D::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  //camera->SetOrtho(true);
  arcball = camera->CreateArcball();

  light = Light::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  //LightPtr light = Light::Make(0.0f,0.0f,0.0f,1.0f,"camera");
  
  AppearancePtr white = Material::Make(1.0f,1.0f,1.0f);
  AppearancePtr red = Material::Make(1.0f,0.5f,0.5f);
  AppearancePtr poff = PolygonOffset::Make(-10,-10);
  AppearancePtr paper = Texture::Make("decal","../images/paper.jpg");

/*   TransformPtr trf1 = Transform::Make();
  trf1->Scale(3.0f,0.3f,3.0f);
  trf1->Translate(0.0f,-1.0f,0.0f); */
  TransformPtr trf2 = Transform::Make();
  trf2->Scale(0.5f,0.5f,0.5f);
  trf2->Translate(0.0f,1.0f,0.0f);
  TransformPtr trf3 = Transform::Make();
  trf3->Translate(0.8f,0.0f,0.8f);
  trf3->Rotate(30.0f,0.0f,1.0f,0.0f);
  trf3->Rotate(90.0f,-1.0f,0.0f,0.0f);
  trf3->Scale(0.5f,0.7f,1.0f);

/*   Error::Check("before shps");
  ShapePtr cube = Cube::Make(); */
  Error::Check("before quad");
  ShapePtr quad = Quad::Make();
  Error::Check("before sphere");
  ShapePtr sphere = Sphere::Make();
  Error::Check("after shps");

  // create shader
  ShaderPtr shader = Shader::Make(light,"world");
  shader->AttachVertexShader("shaders/ilum_vert/vertex.glsl");
  shader->AttachFragmentShader("shaders/ilum_vert/fragment.glsl");
  shader->Link();

  // Define a different shader for texture mapping
  // An alternative would be to use only this shader with a "white" texture for untextured objects
  ShaderPtr shd_tex = Shader::Make(light,"world");
  shd_tex->AttachVertexShader("shaders/ilum_vert/vertex_texture.glsl");
  shd_tex->AttachFragmentShader("shaders/ilum_vert/fragment_texture.glsl");
  shd_tex->Link();

  // build scene
  NodePtr root = Node::Make(shader,
    {//Node::Make(trf1,{red},{cube}),
     Node::Make(shd_tex,trf3,{white,poff,paper},{quad}),
     Node::Make(trf2,{white},{sphere}), 
    }
  );
  scene = Scene::Make(root);
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  // enable depth test 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);  // cull back face
  Error::Check("before render");
  scene->Render(camera);
  Error::Check("after render");

/*   glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NEVER, 1, 0xFFFF);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);   */

  Error::Check("inicio stencil");

  glm::mat4 sm = shadowMatrix(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(viewer_pos[0],viewer_pos[1],viewer_pos[2], 1.0f));

  auto matrix = scene->GetRoot()->GetModelMatrix();
  auto mBackup = matrix;
  matrix = sm * matrix;

  auto shadowTrf1 = Transform::Make();
  shadowTrf1->MultMatrix(matrix);
  scene->GetRoot()->SetTransform(shadowTrf1);
  scene->Render(camera);
  auto oriBackup = Transform::Make();
  oriBackup->MultMatrix(mBackup);
  scene->GetRoot()->SetTransform(oriBackup);

  // separando o objeto sombra em outra cena para aplicar a transformacao de sombra
  Error::Check("antes cena sombra");
  
/*   glStencilFunc(GL_EQUAL, 0, 0xFFFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  glBlendFunc(GL_ONE, GL_ONE);
  glEnable(GL_BLEND);
  glDepthFunc(GL_EQUAL); */
  
  Error::Check("depois cena sombra");


  ShapePtr cube = Cube::Make();
  TransformPtr trf1 = Transform::Make();
  trf1->Scale(3.0f,0.3f,3.0f);
  trf1->Translate(0.0f,-1.1f,0.0f);
  AppearancePtr red = Material::Make(1.0f,0.5f,0.5f);
  ShaderPtr shader = Shader::Make(light,"world");
  shader->AttachVertexShader("shaders/ilum_vert/vertex.glsl");
  shader->AttachFragmentShader("shaders/ilum_vert/fragment.glsl");
  shader->Link();
  ScenePtr shadowScene = Scene::Make(Node::Make(shader, trf1,{red},{cube}));
  
  /* auto matrix2 = scene->GetRoot()->GetModelMatrix();
  auto shadowTrf = Transform::Make();
  shadowTrf->MultMatrix(matrix2);
  shadowScene->GetRoot()->SetTransform(shadowTrf); */
  shadowScene->Render(camera);

  Error::Check("final cena sombra");
  
  glDepthFunc(GL_LESS);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  Error::Check("final display");

}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

static void cursorpos (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->AccumulateMouseMotion(int(x),int(y));
}
static void cursorinit (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->InitMouseMotion(int(x),int(y));
  glfwSetCursorPosCallback(win, cursorpos);     // cursor position callback
}
static void mousebutton (GLFWwindow* win, int button, int action, int mods)
{
  if (action == GLFW_PRESS) {
    glfwSetCursorPosCallback(win, cursorinit);     // cursor position callback
  }
  else // GLFW_RELEASE 
    glfwSetCursorPosCallback(win, nullptr);      // callback disabled
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);       // required for mac os
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER,GLFW_TRUE);  // option for mac os
#endif

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(600,400,"Window title",nullptr,nullptr);
  assert(win);
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  glfwSetMouseButtonCallback(win, mousebutton); // mouse button callback
  
  glfwMakeContextCurrent(win);
#ifdef _WIN32
  if (!gladLoadGL(glfwGetProcAddress)) {
      printf("Failed to initialize GLAD OpenGL context\n");
      exit(1);
  }
#endif
#if defined(__linux__) && defined(__glad_h_)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD OpenGL context\n");
    exit(1);
   }
#endif
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  while(!glfwWindowShouldClose(win)) {
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

