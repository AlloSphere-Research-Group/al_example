
// add sound
//
// - show doxygen on initAudio, onSound,
// - introduce the generator pattern: io() and AudioIOData
// - explain the waveform (pulse) and pitch of the sound
// - show adjusting audio buffer size with initAudio
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

struct AlloApp : App {
  Light light;
  Mesh m;

  AlloApp() {
    cout << "Created AlloApp" << endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.generateNormals();
    initWindow(Window::Dim(200, 300), "this is a title", 10);
    initAudio();
  }

  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    light();
    g.draw(m);
  }

  virtual void onSound(AudioIOData& io) {
    // this will make a pulse wave with a frequency of 44100 / 128 Hz
    io();
    io.out(0) = 1.0f;
    io.out(1) = 1.0f;
    while (io()) {
      io.out(0) = 0.0f;
      io.out(1) = 0.0f;
    }
  }
};

int main() { AlloApp().start(); }

