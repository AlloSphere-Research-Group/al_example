// deathstar
//
// - change rendering primitive to POINTS
//

#include "allocore/io/al_App.hpp"
using namespace al;
struct AlloApp : App {
  Mesh m;
  AlloApp() {
    std::cout << "Created AlloApp" << std::endl;
    addSphere(m);
    m.primitive(Graphics::POINTS);
    initWindow();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) { g.draw(m); }
};
int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
