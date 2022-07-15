#include "GUI.h"
#include "Data.h"
#include "CerealSerializer.h"

#include <spdlog/spdlog.h>

#include <nngpp/protocol/sub0.h>
#include <nngpp/protocol/req0.h>
#include <string>

void GUI::start()
{
  auto window = initGraphics();

  if(window == nullptr)
  {
    spdlog::error("Failed to initialize");
    return;
  }

  // Get initial data from server
  auto req_sock = nng::req::open();
  req_sock.dial( "tcp://localhost:8000" );

  std::stringstream ss;
  ss << initialHelloOpcode;
  auto toSend = ss.str();
  req_sock.send({toSend.data(), toSend.size()});

  auto req_buf = req_sock.recv();
  
  OpcodeType initialDataOpcode{};
  auto [dataPtr, dataSize] = Data::getMessageOpcode(initialDataOpcode, req_buf.data(), req_buf.size());
  BoardMessages chatBoardMessages;
  CerealSerializer::deserialize(chatBoardMessages, dataPtr, dataSize);

  // Initialize socket that will be updating the chat data
  auto sub_socket = nng::sub::open();
  sub_socket.set_opt( NNG_OPT_SUB_SUBSCRIBE, {} ); // Subscribe to everything
  sub_socket.dial("tcp://localhost:8001"); 

  // Initialize GUI state 
  enum class BoardState{
    Lobby,
    Chat
  };

  auto state = BoardState::Lobby;
  std::string currentBoard{};

  void* subscriptionData{};
  size_t size{};
  constexpr size_t bufferSize = 500;

  // Data for Lobby state
  char newBoardTextInputBuffer[bufferSize]{};
  
  // Data for Chat state
  bool focusInitializedAtBottom = false;
  char messageTextInputBuffer[bufferSize]{};

  while (!glfwWindowShouldClose(window))
  {
    // See if server published any data updates
    // Note: socket.recv() throws an exception if message is not received on non-block, which is very expensive.
    // so we will use nng instead of nngpp here.
    int r = nng_recv(sub_socket.get(), &subscriptionData, &size, nng::flag::nonblock | nng::flag::alloc);
    if( r == static_cast<int>(nng::error::success) ) 
    {
      OpcodeType opcode{};
      auto [dataPtr, dataSize] = Data::getMessageOpcode(opcode, subscriptionData, size);

      if(opcode == boardMessagesOpcode)
      {
        spdlog::info("Updated board messages");
        CerealSerializer::deserialize(chatBoardMessages, dataPtr, dataSize);
      }
    }

    handleGraphicsOnLoopStart();

    if(state == BoardState::Lobby)
    {
      ImGui::Begin("Lobby");
      if (ImGui::Button("New Board"))
      {
        NewBoard boardToSend{newBoardTextInputBuffer};

        if(boardToSend.size() != 0)
        {
          auto serializedMessage = CerealSerializer::serialize(boardToSend, newBoardOpcode);
          req_sock.send({serializedMessage.data(), serializedMessage.size()});
          req_sock.recv();
          memset(newBoardTextInputBuffer, 0, bufferSize); // Reset text input after sending message
        }  
      }

      ImGui::InputTextMultiline("##text2", newBoardTextInputBuffer, bufferSize, {300, 50});

      for(auto [boardName, _]: chatBoardMessages)
      {
        if (ImGui::Button(boardName.data()))
        {
          currentBoard = boardName;
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
        // Set the scroll to the bottom for access to the input text initially
        ImGui::SetScrollHere(0.999f);
        focusInitializedAtBottom = true;
      }

      ImGui::InputTextMultiline("##text1", messageTextInputBuffer, bufferSize, {300, 50});

      if(ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
      {
       ImGui::SetKeyboardFocusHere(0);
      }

      if (ImGui::Button("Send"))
      {
        NewMessage newMessage{currentBoard, messageTextInputBuffer};
        auto serializedMessage = CerealSerializer::serialize(newMessage, newMessageOpCode);
        req_sock.send({serializedMessage.data(), serializedMessage.size()});
        req_sock.recv();
        memset(messageTextInputBuffer, 0, bufferSize); // Reset text input after sending message
      }

      ImGui::End();
    }

    handleGraphicsOnLoopEnd(window);
  }

  shutdownGraphics(window);
}

void GUI::handleGraphicsOnLoopStart()
{
    glfwPollEvents();
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {800.f, 600.f});
}

void GUI::handleGraphicsOnLoopEnd(GLFWwindow* window)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    int display_w{}; 
    int display_h{};
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glfwSwapBuffers(window);
}

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* GUI::initGraphics()
{
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (glfwInit() == 0)
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

  int screen_width{};
  int screen_height{};
  glfwGetFramebufferSize(window, &screen_width, &screen_height);
  glViewport(0, 0, screen_width, screen_height);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
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

