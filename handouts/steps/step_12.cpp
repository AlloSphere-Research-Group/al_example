// make a second sine
//
// - does this scale?
// - how do we make lots of spheres and sines?
//

#include "allocore/io/al_App.hpp"
#include "Gamma/Oscillator.h"
using namespace al;
using namespace std;
struct AlloApp : App {
  Light light;
  Mesh m;

  gam::Sine<> sine, sine2;

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();

    sine.freq(440.0);
    sine2.freq(770.0);

    initWindow();
    initAudio();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);

    g.translate(2, 1, 0);
    g.draw(m);
  }
  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());

    while (io()) {
      io.out(0) = sine();   // left
      io.out(1) = sine2();  // right
      // io.out(0) = io.out(1) = 0.5f * sine() + 0.5f * sine2();
      // io.out(0) = io.out(1) = (sine() + sine2()) * 0.5f;
    }
  }
  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main() { AlloApp().start(); }
