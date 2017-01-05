
// fix the light by generating normals and positioning the light
//
// - explain a little about light, shading and normals
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;
struct AlloApp : App {
  Light light;
  Mesh m;

  // the constructor is called just once at setup
  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();

    initWindow();
  }

  // onDraw is called *about* 40 times a second by default (see initWindow)
  //
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    // this would move the light with the nav()
    //
    //light.pos(nav().pos());

    // this would print out the nav() position on every frame
    //
    // cout << nav().pos() << endl;

    light();
    g.draw(m);
  }
};
int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}

