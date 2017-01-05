
// make spheres smaller
//
// - explain why scale() effects all spheres
//

#include "allocore/io/al_App.hpp"
#include "Gamma/Oscillator.h"
using namespace al;
using namespace std;

#define N 20

struct AlloApp : App {
  Light light;
  Mesh m;
  gam::Sine<> sine[N];
  Vec3f position[N];

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);

    m.scale(0.2); // XXX

    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      sine[i].freq(220.0 + i * i);
      position[i] = Vec3f(sin(i), i, -i * i / 7);
    }
    initWindow();
    initAudio();
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    for (int i = 0; i < N; ++i) {
      g.pushMatrix();
      g.translate(position[i]);
      g.draw(m);
      g.popMatrix();
    }
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i)
        s += sine[i]();
      io.out(0) = io.out(1) = s / N;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main() { AlloApp().start(); }
