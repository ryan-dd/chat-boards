#ifndef GUI_H
#define GUI_H

#include <Data.h>

struct GLFWwindow;
namespace nng{struct socket_view;}

class GUI
{
public:
  static void start();
private:
  enum class BoardState{
    Lobby,
    Chat
  };

  static BoardMessages getInitialBoardMessages(nng::socket_view socket);
  static GLFWwindow* initGraphics();
  static void shutdownGraphics(GLFWwindow* window);
  static void handleGraphicsOnLoopStart();
  static void handleGraphicsOnLoopEnd(GLFWwindow* window);
};

#endif // GUI_H
