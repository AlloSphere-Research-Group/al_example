// use Gamma to play a sine wave
//
// - show Gamma doxygen on Sine
// - #include "Gamma/Oscillator.h"
// - explain "gam::Sync::master().spu(audioIO().fps());"
// - use vocabulary: templates, generator, frequency, phase, samplerate
// - don't ask about the <> for now
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

    sine.freq(1400.0);

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
};

int main() { AlloApp().start(); }
