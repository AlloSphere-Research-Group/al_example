// MAT201B: Homework 1
// Original authors: Karl Yerkes and Matt Wright (2012)
//

#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"

using namespace al;
using namespace std;

#define N 20

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;
  gam::Sine<> sine[N];
  Vec3f position[N];
  float radius[N];

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      sine[i].freq(220.0 + i * i);
      position[i] = Vec3f(sin(i), i, -i * i / 7);
      radius[i] = rnd::uniform(.3, 1.0);
    }
    initWindow();
    initAudio();
  }

  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      position[i] +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.1;
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
      g.scale(radius[i]);
      // g.scale(i);
      g.draw(m);
      g.popMatrix();
    }
  }

  virtual void onSound(AudioIOData& io) {
    gam::Sync::master().spu(audioIO().fps());
    while (io()) {
      float s = 0;
      for (int i = 0; i < N; ++i)
        s += sine[i]() / (nav().pos() - position[i]).mag();
      io.out(0) = io.out(1) = s / N;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
