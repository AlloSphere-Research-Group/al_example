
// add mouse interaction to change light position
//
// - show mouse callbacks
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
    m.generateNormals();
    initWindow();
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
  }

  // onMouseMove is when the mouse moves
  //
  virtual void onMouseMove(const ViewpointWindow& w, const Mouse& m) {
    // print mouse coordinates out
    // notice that the origin (0,0) is in the
    // top left corner of the window
    cout << m.x() << " " << m.y() << endl;

    // normalize mouse position from -1.0 to 1.0
    float x = float(m.x()) / w.width() * 2.f - 1.f;
    float y = (float(m.y()) / w.height() * 2.f - 1.f) * -1.f;

    // move light with mouse
    light.pos(Vec3f(x, y, 1.f) * 10.f);
  }

  // other mouse callbacks
  //
  virtual void onMouseDown(const ViewpointWindow& w, const Mouse& m) {}
  virtual void onMouseUp(const ViewpointWindow& w, const Mouse& m) {}
  virtual void onMouseDrag(const ViewpointWindow& w, const Mouse& m) {}
};
int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
