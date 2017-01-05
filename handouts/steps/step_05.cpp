
// move the viewer back, outside the sphere
//
// - show doxygen for Nav and pos()
// - describe the coordinate system (looking down the -Z)
// - mention Quat and Pose
// - using namespace std
// - block comments
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;
struct AlloApp : App {
  Mesh ourMesh;
  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    addSphere(ourMesh);
    initWindow();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    g.draw(ourMesh);
  }
};
int main(int argc, char* argv[]) {
  AlloApp/* silly comment */app;
  app.start();
  return 0;
}
