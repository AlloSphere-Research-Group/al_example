// Change sine wave frequency with the mouse
//
// - setting frequency
// - mouse.dx()
//

#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

struct AlloApp : App {
  Light light;
  Mesh m;

  gam::Sine<> sine;

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();

    sine.freq(440.0);

    initWindow();
    initAudio();
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
  }

  virtual void onSound(AudioIOData& io) {
    // tell gamma's oscillators what the sample rate is
    gam::Sync::master().spu(audioIO().fps());

    while (io()) {
      float s = sine();
      io.out(0) = s;
      io.out(1) = s;
    }
  }

  // Called when the mouse moves
  virtual void onMouseMove(const ViewpointWindow& w, const Mouse& m) {
    // normalize mouse position from -1.0 to 1.0
    float x = float(m.x()) / w.width() * 2.f - 1.f;
    float y = (float(m.y()) / w.height() * 2.f - 1.f) * -1.f;

    // move light with mouse
    light.pos(Vec3f(x, y, 1.f) * 10.f);

    // set frequency of sine wave based on mouse y
    sine.freq(500.f * (y + 1) + 100.f);

    // alternate way using mouse.dy() the change in y position
    // float newFreq = sine.freq() - m.dy();
    // sine.freq(newFreq);
  }

  // other mouse callbacks
  virtual void onMouseDown(const ViewpointWindow& w, const Mouse& m) {}
  virtual void onMouseUp(const ViewpointWindow& w, const Mouse& m) {}
  virtual void onMouseDrag(const ViewpointWindow& w, const Mouse& m) {}
};

int main() { AlloApp().start(); }
