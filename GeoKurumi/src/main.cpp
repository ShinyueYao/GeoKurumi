#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imfilebrowser.h"

#include <filesystem>
#include <Windows.h>
#include <iostream>
#include <cmath>

#include "camera.h"
#include "window.h"
#include "model.h"
#include "mesh.h"
#include "shader.h"
#include "switcher.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);

const unsigned int SCR_WIDTH = 1440;
const unsigned int SCR_HEIGHT = 900;

glm::vec3 camera_position_init = glm::vec3(0.0f, 0.0f, 10.0f);

// camera
Camera camera(camera_position_init);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

string path="";
string filename = "";
switcher enable;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::vec3 Zaxis(0.0f, 0.0f, 1.0f);
glm::vec3 Yaxis(0.0f, 1.0f, 0.0f);
glm::vec3 Xaxis(1.0f, 0.0f, 0.0f);
glm::vec3 Shift(0.0f, 0.0f, 0.0f);
int main()
{
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    const char* glsl_version = "#version 330";
    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GeoKurumi", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    
    GLFWimage images[1];
    images[0].pixels = stbi_load("res/icon/kurumi_icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    //-------------------------------------
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if(enable.mouse_enable)glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    stbi_set_flip_vertically_on_load(true);
    //Z-buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader ourShader("res/shaders/light_shader.vs", "res/shaders/light_shader.fs"); // you can name your shader files however you like
    Model  ourModel(path,filename);
    
    ourShader.use();
    //ourShader.setInt("modeltexture", 0);
    
    //-----------------UI----------------------
    
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        //Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::styleColorsLight();

        ImGui::FileBrowser fileDialog;//File IO


        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Load Fonts
        ImFont* font = io.Fonts->AddFontFromFileTTF("res/font/font.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    

    ImVec4 clear_color = ImVec4(0.974f, 0.923f, 0.97f, 1.00f);
    // 设置光源位置和颜色
    glm::vec4 rot(0.0f);
    //render loop----------------------------------------------------------------------
    
    float prevRotationX = 0.0f;
    float prevRotationY = 0.0f;
    float prevRotationZ = 0.0f;
    
    while (!glfwWindowShouldClose(window))
    {
        //glfwSetCursor(window, NULL);
        glm::vec3 lightPos;
        lightPos.x = 20;
        lightPos.y = 20;
        lightPos.z = 5.0f;
        
        //--------------------------------glfw Window Creation-----------------------------------------------------

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);//esc

        int display_w, display_h;

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (!ourModel.isloaded)
        {
            ourModel.loadModel(path,filename);
        }
        //use shader
       
        ourShader.use();
        float radianX = glm::radians(rot.x);
        float radianY = glm::radians(rot.y);
        float radianZ = glm::radians(rot.z);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model ,radianX, Xaxis);
        model = glm::rotate(model, radianY, Yaxis);
        model = glm::rotate(model, radianZ, Zaxis);
        
        model = glm::translate(model, Shift); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setInt("blinn", enable.blinn);

        // render the loaded model

        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
        
        
        //-----------------------IMGUI Creatiion----------------------------------------------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        
         //添加菜单栏
        MyWindow::top_menu(fileDialog,path,filename, ourModel,enable);
        // 添加更多菜单项...
        if(enable.show_hint)
            MyWindow::hint(enable.show_hint, enable);
        if (enable.save)
            MyWindow::show_in(path,filename, enable);
        //2. show window 
        MyWindow::left_window(enable, clear_color, camera, io.Framerate, rot,Shift);
        if (glfwGetKey(window, GLFW_KEY_C) == true || enable.key_c == true) unsigned int modelTexture = loadTexture("res/textures/map.jpg");
        // Rendering
        ImGui::Render();
        //------------------------------------end of IMGUI Creation-----------------------------------------------

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
    bool Aexp = enable.mouse_enable && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && Aexp )
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && Aexp )
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && Aexp)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && Aexp)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && Aexp)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && Aexp)
        camera.ProcessKeyboard(UPWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && Aexp)
        camera.ProcessKeyboard(DOWNWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !enable.blinnKeyPressed)
    {
        enable.blinn = !enable.blinn;
        enable.blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        enable.blinnKeyPressed = false;
    }
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    bool Aexp = enable.mouse_enable && glfwGetKey(window, GLFW_KEY_LEFT_ALT) != GLFW_PRESS;
    if (enable.firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        enable.firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    if (Aexp && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
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
