
// create a sphere
//
// - why is the screen all white?
// - point out key controls for navigation and stereo
// - see "enum Primitive" al_Graphics.hpp
// - show doxygen for Mesh, addSphere(), draw() and onDraw()
// - addSphere changes Mesh primitive to triangles
// - use vocabulary: const, reference, calls, callback
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

struct AlloApp : App {
  Mesh m;
  AlloApp() {
    cout << "Created AlloApp" << endl;
    addSphere(m);
    initWindow();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) { g.draw(m); }
};

int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
