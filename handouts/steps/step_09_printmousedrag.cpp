
// add click sound on keypress
//
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;
struct AlloApp : App {
  bool shouldClick;
  Light light;
  Mesh m;

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();
    initWindow();
    initAudio();

    shouldClick = true;

    // disable nav control mouse drag to look
    navControl().useMouse(false);
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
  }

  virtual void onSound(AudioIOData& io) {
    while (io()) {
      if (shouldClick) {
        shouldClick = false;
        io.out(0) = 1.0f;
        io.out(1) = 1.0f;
      } else {
        io.out(0) = 0.0f;
        io.out(1) = 0.0f;
      }
    }
  }

  virtual void onKeyDown(const ViewpointWindow& vw, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
      shouldClick = true;
    }
  }

  virtual void onMouseDrag(const ViewpointWindow& w, const Mouse& m) {
    cout << "mouse drag by [" << m.dx() << "," << m.dy() << "]" << endl;
  }
};

int main() { AlloApp().start(); }
