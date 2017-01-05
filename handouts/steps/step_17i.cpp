
// change position and frequency of agents using mouse
//
// - mapping ranges of values (i.e. mouse -> frequency)
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

  float freqOffset;
  float freqMultiplier;

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.scale(0.2);
    m.generateNormals();

    freqOffset = 220.f;
    freqMultiplier = 1.f;

    for (int i = 0; i < N; ++i) {
      sine[i].freq(freqOffset + i * i);
      position[i] = Vec3f(sin(i), i, -i * i / 7);
    }
    initWindow();
    initAudio();
  }

  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      position[i] +=
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.01;
      sine[i].freq(freqOffset + position[i].mag() * freqMultiplier);
    }
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
      for (int i = 0; i < N; ++i) s += sine[i]();
      io.out(0) = io.out(1) = s / N;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      cout << "Spacebar pressed" << endl;
    }
  }

  // Called when the mouse moves
  virtual void onMouseMove(const ViewpointWindow& w, const Mouse& m) {
    // normalize mouse position from -1.0 to 1.0
    float x = float(m.x()) / w.width() * 2.f - 1.f;
    float y = (float(m.y()) / w.height() * 2.f - 1.f) * -1.f;

    // move light with mouse
    light.pos(Vec3f(x, y, 1.f) * 10.f);

    // adjust frequency variables
    freqOffset = (y + 1) * 220.f;
    freqMultiplier = abs(20 * x);

    // set sphere positions based on mouse
    for (int i = 0; i < N; ++i) {
      position[i] =
          Vec3f(sin(i * y) + cos(i * x), sin(i * x) + cos(i * y), -i * i / 7);
      sine[i].freq(freqOffset + position[i].mag() * freqMultiplier);
    }
  }

  // other mouse callbacks
  virtual void onMouseDown(const ViewpointWindow& w, const Mouse& m) {}
  virtual void onMouseUp(const ViewpointWindow& w, const Mouse& m) {}
  virtual void onMouseDrag(const ViewpointWindow& w, const Mouse& m) {}
};

int main() { AlloApp().start(); }
