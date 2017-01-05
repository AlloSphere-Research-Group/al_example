
// add color
//
// - explain why Material is necessary
// - explain HSV
// - show audio changes
//

#include "allocore/io/al_App.hpp"
#include "Gamma/Oscillator.h"
using namespace al;
using namespace std;

#define N 20

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;
  gam::Sine<> sine[N];
  Vec3f position[N];

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.scale(0.2);
    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      sine[i].freq(220.0 + i * i);
      position[i] = Vec3f(sin(i), i, -i * i / 7);
    }
    initWindow();
    initAudio();
  }

  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      position[i] +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.01;
      sine[i].freq(220.0 + position[i].mag() * 5);
    }
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();
    for (int i = 0; i < N; ++i) {
      g.pushMatrix();
      g.color(HSV((float)i / N, 1, 1));
      g.translate(position[i]);
      g.draw(m);
      g.popMatrix();
    }
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i) {
        float distance = (nav().pos() - position[i]).mag();
        // constrain distance so no matter how we navigate each agent is not too
        // loud...
        if (distance < 0.5) distance = 0.5;
        // ... or too quiet
        if (distance > 7) distance = 7;
        s += sine[i]() / distance;
      }
      io.out(0) = io.out(1) = s * 0.08;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main() { AlloApp().start(); }
