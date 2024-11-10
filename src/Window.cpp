#include "Core.h"

#include "Window.h"

namespace yon {

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::Create(int width, int height, std::string title) {
  auto success = glfwInit();
  ASSERT(success);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  m_context.Create(m_window);

  glfwSetWindowUserPointer(m_window, &m_data);

  glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos)
                                        {
                                          auto window_data =
                                            reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

                                          MouseMoveEvent event(xpos, ypos);
                                          window_data->m_callback(event);
                                        });

  glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
                                  {
                                    auto window_data =
                                      reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

                                    switch(action) {
                                      case GLFW_PRESS:
                                      {
                                        KeyPressEvent event(key);
                                        window_data->m_callback(event);
                                        break;
                                      }

                                      case GLFW_RELEASE:
                                      {
                                        KeyReleaseEvent event(key);
                                        window_data->m_callback(event);
                                        break;
                                      }

                                      case GLFW_REPEAT:
                                      {
                                        KeyHoldEvent event(key);
                                        window_data->m_callback(event);
                                        break;
                                      }

                                      default:
                                        break;
                                    }
                                  });

  glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
                                          {
                                            auto window_data =
                                              reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));
                                            WindowCloseEvent event;
                                            window_data->m_callback(event);
                                          });
}

void Window::OnUpdate() {
  m_context.SwapBuffers();
  glfwPollEvents();
}

}
