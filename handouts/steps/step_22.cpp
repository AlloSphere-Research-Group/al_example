// Giving each agent a song

#include "Gamma/Oscillator.h"
#include "allocore/io/al_App.hpp"
using namespace al;
#define N 1

#define MAX_SONG_LENGTH 10

struct Agent {
  Color color;
  gam::Sine<> sine;
  gam::Accum<> tmr;  // an accumulator that will keep track of note time
  Vec3f position;

  // My song
  int song_length;
  int current_note;
  float frequencies[MAX_SONG_LENGTH];  // Hertz
  float durations[MAX_SONG_LENGTH];    // seconds

  Agent() {
    song_length = 1 + rnd::uniform<int>(MAX_SONG_LENGTH);
    std::cout << "Calling Agent constructor; song_length is " << song_length
              << std::endl;

    // Make up our song

    // Random starting note between 150 and 300 Hz
    frequencies[0] = rnd::uniform(150, 300);
    durations[0] = 0.5;

    float multiplier = rnd::uniform(1.01, 1.5);
    float offset = rnd::uniformS(25);

    for (int i = 1; i < song_length; i++) {
      frequencies[i] = frequencies[i - 1] * multiplier + offset;
      durations[i] = pow(2.0, rnd::uniform<int>(5) - 4);
    }

    std::cout << "Here is my entire melody: " << std::endl;
    for (int i = 0; i < song_length; i++) {
      std::cout << "  Duration " << durations[i] << ", frequency "
                << frequencies[i] << std::endl;
    }

    // Initialize song player
    current_note = 0;
    sine.freq(frequencies[current_note]);
    tmr.period(durations[current_note]);
  }

  void onAnimate(double dt) {
    position += Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * 0.01;
  }

  void onDraw(Graphics& g, const Viewpoint& v, Mesh& m) {
    g.pushMatrix();
    g.color(color);
    g.translate(position);
    g.draw(m);
    g.popMatrix();
  }

  float onSound() {
    if (tmr()) {
      // Current note just ended so move to next one
      current_note++;
      if (current_note >= song_length) current_note = 0;
      sine.freq(frequencies[current_note]);
      tmr.reset();
      tmr.period(durations[current_note]);
    }

    return sine();
  }
};

struct AlloApp : App {
  Material material;
  Light light;
  Mesh m;
  Agent agents[N];

  AlloApp() {
    std::cout << "Created AlloApp" << std::endl;
    nav().pos(0, 0, 10);
    light.pos(0, 0, 10);
    addSphere(m);
    m.primitive(Graphics::TRIANGLES);
    m.scale(0.2);
    m.generateNormals();
    for (int i = 0; i < N; ++i) {
      agents[i].position = Vec3f(sin(i), i, -i * i / 7);
      agents[i].color = HSV((float)i / N, 1, 1);
    }
    initWindow();
    initAudio();
  }
  virtual void onAnimate(double dt) {
    for (int i = 0; i < N; ++i) {
      agents[i].onAnimate(dt);
    }
  }
  virtual void onDraw(Graphics& g, const Viewpoint& v) {
    material();
    light();
    for (int i = 0; i < N; ++i) {
      agents[i].onDraw(g, v, m);
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
        s += agents[i].onSound() / distance;
      }
      io.out(0) = io.out(1) = s * 0.08;
    }
  }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      std::cout << "Spacebar pressed" << std::endl;
    }
  }
};

int main(int argc, char* argv[]) {
  AlloApp app;
  app.start();
  return 0;
}
