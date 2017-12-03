// MAT201B: Homework 1, Problem 1
// Author: Karl Yerkes
// Date: 2015-10-11
// This work is adapted from examples written by Karl Yerkes, Matt Wright, and
// Tim Wood.
//

#include "Gamma/Oscillator.h"
#include "al/core.hpp"

using namespace al;
using namespace std;

#define N 20

struct AlloApp : App {
  // Material material;
  Light light;
  Mesh m;
  gam::Sine<> sine[N];
  Vec3f position[N];
  float radius[N];

  bool shouldClick;
  bool shouldFlash[N];

  void onCreate() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      sine[i].freq(220.0 + i * i);
      position[i] = Vec3f(sin(i), i, -i * i / 7);
      radius[i] = rnd::uniform(.3, 1.0);

      shouldClick = false;
      shouldFlash[i] = false;
    }
  }

  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      position[i] +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.1;
      sine[i].freq(220.0 + position[i].mag() * 5);
    }

    for (int a = 0; a < N; ++a) {
      for (int b = a + 1; b < N; ++b) {
        if ((position[a] - position[b]).mag() <= (radius[a] + radius[b])) {
          // a and b are colliding...
          cout << a << " collided with " << b << endl;
          shouldClick = true;
          shouldFlash[a] = true;
          shouldFlash[b] = true;
        }
      }
    }
  }

  virtual void onDraw(Graphics& g) {
    g.clear(0);
    g.lighting(true);
    g.depthTesting(true);

    g.light(light);
    // g.material(material);

    for (int i = 0; i < N; ++i) {
      g.pushMatrix();
      if (shouldFlash[i]) {
        shouldFlash[i] = false;
        g.color(1, 1, 1);
      } else {
        g.color(HSV((float)i / N, 1, 1));
      }
      g.translate(position[i]);
      g.scale(radius[i]);
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

      if (shouldClick) {
        shouldClick = false;
        s = N;
      }

      io.out(0) = s / N;
      io.out(1) = s / N;
    }
  }

  virtual void onKeyDown(const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }
};

int main(int argc, char* argv[]) {
  AlloApp app;
  app.initAudio();
  app.start();
  return 0;
}
