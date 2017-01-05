// MAT201B
// Fall 2015
// Author(s): Karl Yerkes
//
// Shows how to:
// - Measure a value, calculating mean, minimum, and maximum
//

#include "allocore/io/al_App.hpp"
#include <algorithm>
using namespace al;
using namespace std;

struct AlloApp : App {
  vector<double> dts;

  AlloApp() { initWindow(); }

  virtual void onAnimate(double dt) { dts.push_back(dt); }

  virtual void onKeyDown(const ViewpointWindow&, const Keyboard& k) {
    if (k.key() == ' ') {
      vector<double> local(dts);
      dts.clear();
      sort(local.begin(), local.end());
      cout << "min: " << local[0] << endl;
      cout << "max: " << local[local.size() - 1] << endl;
      double sum = 0;
      for (auto dt : local) sum += dt;
      cout << "mean: " << sum / local.size() << endl;
    }
  }
};

int main() { AlloApp().start(); }
