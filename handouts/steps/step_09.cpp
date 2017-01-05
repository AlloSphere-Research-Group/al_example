// add click sound on keypress
//
// - introduce the type: bool (also int, unsigned, float, double, )
// - explain the concept of a flag
// - lookup onKeyDown, ViewpointWindow, and Keyboard in the doxygen
// - breifly mention threading. we'll return to it later
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
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
  }

  virtual void onSound(AudioIOData& io) {
    while (io()) {  // for each sample... do..
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
};

int main() { AlloApp().start(); }
