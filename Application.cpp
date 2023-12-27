#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h" >
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include <stdio.h>


static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL error] (" << error << ") " << std::endl;
        return false;
    }
    return true;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //if we specify as core_profile then we have to use vertex array objects

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // changing rate

    if (glewInit() != GLEW_OK)
        std::cout << "error_glewinit" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    //float positions[] = {
    //    -0.5f, -0.5f, 0.0f, 0.0f, // 0
    //    0.5f , -0.5f, 1.0f, 0.0f, // 1
    //    0.5f , 0.5f, 1.0f, 1.0f, // 2
    //    -0.5f, 0.5f, 0.0f, 1.0f   // 3
    //};


    //float positions[] = {
    //    100.0f, 100.0f, 0.0f, 0.0f, // 0
    //    200.0f , 100.0f, 1.0f, 0.0f, // 1
    //    200.0f , 200.0f, 1.0f, 1.0f, // 2
    //    100.0f, 200.0f, 0.0f, 1.0f   // 3
    //};
    float positions[] = {
    -50.0f, -50.0f, 0.0f, 0.0f, // 0
    50.0f , -50.0f, 1.0f, 0.0f, // 1
    50.0f , 50.0f, 1.0f, 1.0f, // 2
    -50.0f, 50.0f, 0.0f, 1.0f,   // 3

    };


    //any index buffer must be of type unsigned int
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    // ------------------------- binding vertices to buffer -------------------
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    //unsigned int buffer;
    //glGenBuffers(1, &buffer); // (number of buffers desired, the id of the buffer will be stored here by the function)
    ////After getting a buffer ID, we need to select a buffer. 
    //// A buffer is selected by Bind buffer as such
    //glBindBuffer(GL_ARRAY_BUFFER, buffer); // we simply say that our buffer is an array
    //glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    //buffer layout
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)0); // this line link buffer with vao
    //glEnableVertexAttribArray(0);

    // ---------------------- index buffers -----------------------------------

    IndexBuffer ib(indices, 6);

    //unsigned int ibo; // index buffer object
    //glGenBuffers(1, &ibo); // (number of buffers desired, the id of the buffer will be stored here by the function)
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // we simply say that our buffer is an array
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    

    //bir vertex yapısında 3 farklı attribute tanımlanabilir
    // position of the vertex, texture coordinates, normal vector
    // ama bunu positions[6] şeklinde yapıp direk opengl e veremeyiz. 
    // Bu 3d veya 2d koordinatlara sahip olabilir ya da normal vector bilgisine sahip olmayabilir.
    // Tüm bunları glVertexAttribPointer ile söylememiz lazım
    // stride: size of each vertex 
    //      -> mesela position 3 component (float) bir vector olsun -> 3 * 4 = 12
    //      -> texture coordinates 2 component vector olsun -> 2 * sizeof(float) = 2 * 4 = 8
    //      -> normal 3 component vector olsun -> 3 * 4 = 12
    //      -> total = 12 + 8 + 12 = 32 bytes is our stride 
    // pointer: points to the beginning of that vertex
    //(starting position, 2 floats per vertex is provided (2D or 3D...),stride, pointer to the vertex)




    //#version 330 -> glsl (opengl shader language)
    // core -> it wont let us use any deprecated function


    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f); // it will get culled if it is outside of the last 2 parameters boundaries (z)
    /*glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));*/
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


    Shader shader("D:/dev/Visual Studio Community 2022/OpenGL/OpenGL/res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 1.0f, 0.3f, 0.8f, 1.0f);

    

    //Texture texture("D:/dev/Visual Studio Community 2022/OpenGL/OpenGL/res/textures/christmas_tree2.png");
    Texture texture("D:/dev/Visual Studio Community 2022/OpenGL/OpenGL/res/textures/sky.jpg");
    int slot = 0;
    texture.Bind(slot);
    shader.SetUniform1i("u_Texture",slot);
 
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");

    //ImGui variables: 
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    float r = 0.0f;
    float increment = 0.05f;

    glm::vec4 Color(0.0, 0.3f, 0.8f, 1.0f);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(errorOccurredGL, NULL);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        /*glClear(GL_COLOR_BUFFER_BIT);*/
        //renderer.Clear();
        renderer.Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Draw arrays in çizeceği buffer bilgisi bindladığımız bufferdan çekiliyor
        // vertices kullanacak olsaydık bu şekilde çizdiriyoruz ama index buffer ile çizdirmek daha avantajlı
        //glDrawArrays(GL_TRIANGLES, 0, 6); // read the usage from docs.GL

        //uniforms are set per draw, so if you drawy both of your triangles at the same draw call, 
        //you cannot change their uniforms in between

        shader.Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;

            shader.SetUniformMat4f("u_MVP", mvp);
            /*shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);*/
            shader.SetUniform4f("u_Color", Color.r, Color.g, Color.b, 1.0f);
            renderer.Draw(va, ib, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;

            shader.SetUniformMat4f("u_MVP", mvp);
            /*shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);*/
            shader.SetUniform4f("u_Color", Color.r, Color.g, Color.b, 1.0f);
            renderer.Draw(va, ib, shader);
        }

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            {
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            }
            ImGui::ColorEdit3("clear color", (float*)&Color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}