
// minimal app with window
//
// - show the doxygen for initWindow() and start()
// -   initWindow can take arguments, but here we use the (good) defaults
// - point out key controls fullscreen toggle
// - start blocks until the app is shut down (Command-Q, Control-c, Alt-F4,
// etc.)
// - vocabulary: blocking call
//

#include "allocore/io/al_App.hpp"
using namespace al;
struct AlloApp : App {
  AlloApp() {
    std::cout << "Created AlloApp" << std::endl;
    initWindow();
    // initWindow(Window::Dim(800,600), "", 40);
  }
};

int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
