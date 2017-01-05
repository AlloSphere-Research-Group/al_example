// make an Agent data structure
//
// - vocabulary: encapsulation, agent, refactor,
// -

#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

#define N 20

struct Agent {
  gam::Sine<> sine;
  Vec3f position;
};

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;
  Agent agents[N];

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.scale(0.2);
    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      agents[i].position = Vec3f(sin(i), i, -i * i / 7);
    }
    initWindow();
    initAudio();
  }
  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      agents[i].position +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.01;
      agents[i].sine.freq(220.0 + agents[i].position.mag() * 5);
    }
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();
    for (int i = 0; i < N; ++i) {
      g.pushMatrix();
      g.color(HSV((float)i / N, 1, 1));
      g.translate(agents[i].position);
      g.draw(m);
      g.popMatrix();
    }
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i) {
        float distance = (nav().pos() - agents[i].position).mag();
        // constrain distance so no matter how we navigate each agent is not too
        // loud...
        if (distance < 0.5) distance = 0.5;
        // ... or too quiet
        if (distance > 7) distance = 7;
        s += agents[i].sine() / distance;
      }
      io.out(0) = s * 0.08;
      io.out(1) = s * 0.08;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main() { AlloApp().start(); }
