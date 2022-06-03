//Relative path includes:
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "shader.h"
#include "camera.h"

#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Function Prototypes:
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char *path);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cameraControl = false;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
float uni_color[] = {0,0,0,0};

void setImGuiStyle(float highDPIscaleFactor)
{
    ImGuiStyle &style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();
    style.ScaleAllSizes(highDPIscaleFactor);
    //io.Fonts->AddFontFromFileTTF("verdana.ttf", 18.0f * highDPIscaleFactor, NULL, NULL);
    io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../../src/imgui/misc/fonts/ProggyClean.ttf", 13*highDPIscaleFactor);

    //ImGui::SetWindowFontScale(highDPIscaleFactor);
}

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "opengl_ex", NULL, NULL);
  if (window == NULL)
  {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //Mouse Input
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetMouseButtonCallback(window, mouse_callback);
  glfwSetCursorPosCallback(window, pos_callback);
  glfwSetScrollCallback(window, scroll_callback);

  //GLAD:
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  }
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  //TRANSPARENCY:
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //VERTICES:
  float vertices[] = {
      //POSITIONS            NORMALS             TEX COORDS
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };
  // positions all containers
  glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
  };
  // positions of the point lights
  glm::vec3 pointLightPositions[] = {
      glm::vec3( 0.7f,  0.2f,  2.0f),
      glm::vec3( 2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f, -3.0f)
  };
  // first, configure the cube's VAO (and VBO)
  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  //LIGHTING:
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  // we only need to bind to the VBO, the container's VBO's data already contains the data.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // set the vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  //SHADERS:
  Shader lighting("shaders/lit/lighting.vert", "shaders/lit/lighting.frag");
  Shader light_source("shaders/lit/lighting_source.vert", "shaders/lit/lighting_source.frag");

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMap  = loadTexture("container2.png");
  unsigned int specularMap = loadTexture("container2_specular.png");
  unsigned int emissionMap = loadTexture("matrix.jpg");

  //imgui
  const char* glsl_version = "#version 330 core";
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  setImGuiStyle(2);
  //bool show_demo_window = true;

  lighting.use();
  //glUniform1i(glGetUniformLocation(lighting.ID, "texture"), 0);
  //glUniform1i(glGetUniformLocation(lighting.ID, "texture"), 1);
  //lighting.setInt("texture", 0);
  lighting.setInt("material.diffuse", 0);
  lighting.setInt("material.specular", 1);
  lighting.setInt("material.emission", 2);

  //glm::mat4 view = glm::mat4(1.0f);
  // note that we're translating the scene in the reverse direction of where we want to move
  //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection;
  //projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  //projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
  lighting.setMat4("projection", projection);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow(&show_demo_window);
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Exit")) // It would be nice if this was a "X" like in the windows title bar set off to the far right
        {
          glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        // It would also be nice if there could be a button for minimizing the window and a button for maximizing the window

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Color"))
      {
        //int vertex_color_loc = glGetUniformLocation(lighting.ID, "uni_color");
        ImGui::ColorEdit4("Color", uni_color);
        //glUniform4f(vertex_color_loc, uni_color[0],uni_color[1],uni_color[2],uni_color[3]);
        lighting.setVec4("uni_color", uni_color[0],uni_color[1],uni_color[2],uni_color[3]);
        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    ImGui::Render();

    // render
    // ------

    //BACKGROUND COLOR:
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lighting.use();
    lighting.setVec3("viewPos", camera.Position);
    lighting.setFloat("material.shininess", 32.0f);

    // directional light
    lighting.setVec4("lightColor",uni_color[0],uni_color[1],uni_color[2],uni_color[3]);
    lighting.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lighting.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    lighting.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lighting.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lighting.setVec3("pointLights[0].position", pointLightPositions[0]);
    lighting.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lighting.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lighting.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lighting.setFloat("pointLights[0].constant", 1.0f);
    lighting.setFloat("pointLights[0].linear", 0.09f);
    lighting.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    lighting.setVec3("pointLights[1].position", pointLightPositions[1]);
    lighting.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lighting.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lighting.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lighting.setFloat("pointLights[1].constant", 1.0f);
    lighting.setFloat("pointLights[1].linear", 0.09f);
    lighting.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    lighting.setVec3("pointLights[2].position", pointLightPositions[2]);
    lighting.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    lighting.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    lighting.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    lighting.setFloat("pointLights[2].constant", 1.0f);
    lighting.setFloat("pointLights[2].linear", 0.09f);
    lighting.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    lighting.setVec3("pointLights[3].position", pointLightPositions[3]);
    lighting.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    lighting.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    lighting.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    lighting.setFloat("pointLights[3].constant", 1.0f);
    lighting.setFloat("pointLights[3].linear", 0.09f);
    lighting.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    lighting.setVec3("spotLight.position", camera.Position);
    lighting.setVec3("spotLight.direction", camera.Front);
    lighting.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lighting.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    lighting.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    lighting.setFloat("spotLight.constant", 1.0f);
    lighting.setFloat("spotLight.linear", 0.09f);
    lighting.setFloat("spotLight.quadratic", 0.032f);
    lighting.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lighting.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lighting.setMat4("projection", projection);
    lighting.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lighting.setMat4("model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    // bind emission map
    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, emissionMap);

    // render the cube
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        lighting.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    light_source.use();
    // we now draw as many light bulbs as we have point lights.
    glBindVertexArray(lightVAO);
    for (unsigned int i = 0; i < 4; i++)
    {
      light_source.setMat4("projection", projection);
      light_source.setMat4("view", view);
      light_source.setVec4("lightColor", uni_color[0],uni_color[1],uni_color[2],uni_color[3]);
      model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
      light_source.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    //End:
    glfwSwapBuffers(window);
    glfwPollEvents();
    }
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
  //glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.Sprint = true;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        camera.Sprint = false;
}
void pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if(cameraControl==true){
      float xoffset = xpos - lastX;
      float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

      lastX = xpos;
      lastY = ypos;

      camera.ProcessMouseMovement(xoffset, yoffset);
    }
}
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if(GLFW_PRESS == action){
      glfwGetCursorPos(window, &lastX, &lastY);
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      cameraControl = true;

    }
    else if(GLFW_RELEASE == action){
      cameraControl = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      //glfwGetCursorPos(window, &lastX, &lastY);
    }
  }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH=width;
    SCR_HEIGHT=height;
}
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
