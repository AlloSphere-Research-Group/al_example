
// turn on the lights
//
// - show the doxygen for Light and pos()
// - use vocabulary: operator
// - are the lights working?
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;
struct AlloApp : App {
  Light light;
  Mesh m;
  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    initWindow();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
  }
};
int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}

