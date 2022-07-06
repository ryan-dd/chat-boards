#include "GUI.h"

#include <cstddef>
#include <cstdio>
#include <spdlog/spdlog.h>

#include <unordered_map>
#include <nngpp/nngpp.h>
#include <nngpp/protocol/sub0.h>
#include <nngpp/protocol/req0.h>

#include "Data.h"
#include "CerealSerializer.h"

void GUI::start()
{
  auto window = initGraphics();

  if(window == nullptr)
  {
    return;
  }

  // Get initial data from server
	nng::socket req_sock = nng::req::open();
	req_sock.dial( "tcp://localhost:8000" );
  req_sock.send("Hello");

  BoardMessages chatBoardMessages;
	nng::buffer req_buf = req_sock.recv();
  CerealSerializer::decodeCereal(chatBoardMessages, req_buf);

  // Initialize socket that will be updating the chat data
  nng::socket sub_socket = nng::sub::open();
  sub_socket.set_opt( NNG_OPT_SUB_SUBSCRIBE, {} );
  sub_socket.dial("tcp://localhost:8001"); 

  // Initialize gui state 
  // All states
  enum class BoardState{
    Lobby,
    Chat
  };

  BoardState state = BoardState::Lobby;
  std::string currentBoard{};

  void* subscriptionData;
  size_t size;

  // Board state
  bool focusInitializedAtBottom = false;
  constexpr size_t bufferSize = 500;
  char inputTextBuffer[bufferSize]{};

  while (!glfwWindowShouldClose(window))
  {
    // See if server published any data updates
    // Note: socket.recv() throws an exception if message is not received on non-block, which is way too expensive.
    // so we will use nng instead of nngpp here.
    int r = nng_recv(sub_socket.get(), &subscriptionData, &size, nng::flag::nonblock | nng::flag::alloc);
    if( r == static_cast<int>(nng::error::success) ) 
    {
      spdlog::info("Got sub message");
      CerealSerializer::decodeCereal(chatBoardMessages, {subscriptionData, size});
    }

    glfwPollEvents();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {800.f,600.f });

    if(state == BoardState::Lobby)
    {
      ImGui::Begin("Lobby");
      for(auto [key, value]: chatBoardMessages)
      {
        if (ImGui::Button(key.data()))
        {
          currentBoard = key;
          state = BoardState::Chat;
        }
      }
      ImGui::End();
    }
    else if(state == BoardState::Chat)
    {
      ImGui::Begin(currentBoard.data());

      if (ImGui::Button("Back"))
      {
        focusInitializedAtBottom = false;
        state = BoardState::Lobby;
      }

      for(auto& message: chatBoardMessages.at(currentBoard))
      {
        ImGui::Text("%s", message.data()); 
      }

      if(!focusInitializedAtBottom)
      {
        ImGui::SetScrollHere(0.999f);
        focusInitializedAtBottom = true;
      }

      ImGui::InputTextMultiline("##text1", inputTextBuffer, bufferSize, {300, 50});

      if(ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
      {
       ImGui::SetKeyboardFocusHere(0);
      }

      if (ImGui::Button("Send"))
      {
        NewMessage newMessage{currentBoard, inputTextBuffer};
        CerealSerializer::encodeCerealAndSend(req_sock, newMessage);
        auto buffer = req_sock.recv();
        CerealSerializer::decodeCereal(chatBoardMessages, buffer);
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

  shutdownGraphics(window);
}

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* GUI::initGraphics()
{
    // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
  {
    return nullptr;
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
#endif

  // Create window with graphics context
  constexpr int width = 1280;
  constexpr int height = 720;
  GLFWwindow *window = glfwCreateWindow(width, height, "Chat boards", nullptr, nullptr);
  if (window == nullptr)
  {
    return nullptr;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  bool err = glewInit() != GLEW_OK;

  if (err)
  {
    spdlog::error("Failed to initialize OpenGL loader!");
    return nullptr;
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
  return window;
}

void GUI::shutdownGraphics(GLFWwindow* window)
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}
