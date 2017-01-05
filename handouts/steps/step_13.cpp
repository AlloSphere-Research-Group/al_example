
// make 20 sine waves
//
// - explain for, #define, arrays and the / N
// - why does the sound pulse at 1 second intervals?
//

#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

#define N 20

struct AlloApp : App {
  Light light;
  Mesh m;

  gam::Sine<> sine[N];  // array [] .. the way to make many of something

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();

    for (int i = 0; i < N; ++i)
      sine[i].freq(220.0 + i * i);  // 220, 221, 224, ... 581

    initWindow();
    initAudio();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
    g.pushMatrix();
    g.translate(2, 1, 0);
    g.draw(m);
    g.popMatrix();
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());

    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i) s += sine[i]();
      io.out(0) = s / N;
      io.out(1) = s / N;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main() { AlloApp().start(); }
