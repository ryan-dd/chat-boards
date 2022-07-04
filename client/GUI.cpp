#include "GUI.h"

#include <cstddef>
#include <cstdio>
#include <spdlog/spdlog.h>

#include "imgui.h"
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"

#include <GL/glew.h> // Initialize with glewInit()

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <nngpp/nngpp.h>
#include <nngpp/protocol/sub0.h>
#include <nngpp/protocol/req0.h>

#include "Data.h"
#include "CerealSerializer.h"

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

bool setupWindow(GLFWwindow* window, std::string windowTitle, int windowWidth, int windowHeight)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
  {
    spdlog::error("GLFW init error");
    return false;
  }

    // Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);       // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Create window with graphics context
  window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
  if (window == nullptr)
  {
    spdlog::error("glfwCreateWindowError");
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  bool err = glewInit() != GLEW_OK;

  if (err)
  {
    spdlog::error("Failed to initialize OpenGL loader!");
    return false;
  }

  int screen_width;
  int screen_height;
  glfwGetFramebufferSize(window, &screen_width, &screen_height);
  glViewport(0, 0, screen_width, screen_height);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  return true;
}

void GUI::start()
{
  // Get initial messages from server
	nng::socket req_sock = nng::req::open();
	req_sock.dial( "tcp://localhost:8000" );
  req_sock.send("Hello");

  BoardMessages chatBoardMessages;
	nng::buffer req_buf = req_sock.recv();
  CerealSerializer::decodeCereal(chatBoardMessages, req_buf);

  // Setup GUI
  constexpr int width = 1280;
  constexpr int height = 720;
  GLFWwindow* window;
  if(!setupWindow(window, "Chat Boards", width, height))
  {
    return;
  }

  bool initialized_at_bottom = false;
  size_t bufferSize = 500;
  char inputTextBuffer[bufferSize];
  memset(inputTextBuffer, 0, bufferSize);
  std::string state = "Boards";
  std::string currentBoard{};

  // Subscribe for updates
  /* nng::socket sub_socket = nng::sub::open(); */
  /* sub_socket.set_opt( NNG_OPT_SUB_SUBSCRIBE, {} ); */
  /* sub_socket.dial("tcp://localhost:8001"); */ 

  while (!glfwWindowShouldClose(window))
  {
    /* auto msg = sub_socket.recv(); TODO make nonblocking*/

    glfwPollEvents();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {800.f,600.f });

    if(state == "Boards")
    {
      ImGui::Begin("Boards");
      for(auto [key, value]: chatBoardMessages)
      {
        if (ImGui::Button(key.data()))
        {
          currentBoard = key;
          initialized_at_bottom = false;
          state = "Chat";
        }
      }
      ImGui::End();
    }
    else if(state == "Chat")
    {
      ImGui::Begin("Chat here!!");

      if (ImGui::Button("Back"))
      {
        state = "Boards";
      }

      for(auto& message: chatBoardMessages.at(currentBoard))
      {
        ImGui::Text("%s", message.data()); 
      }

      if(!initialized_at_bottom)
      {
        ImGui::SetScrollHere(0.999f);
        initialized_at_bottom = true;
      }

      ImGui::InputTextMultiline("##text1", inputTextBuffer, bufferSize, {300, 50});

      if (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
         ImGui::SetKeyboardFocusHere(0);

      if (ImGui::Button("Send"))
      {
        // TODO send to server
        chatBoardMessages.at(currentBoard).push_back(inputTextBuffer);
        memset(inputTextBuffer, 0, bufferSize);
      }

      ImGui::End();
    }

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

}