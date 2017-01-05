
// minimal allocore app
//
// - allocore/io/al_App.hpp gets us al::App
// - show the organization of the allocore include folder
// - show the doxygen for 'App'
// - App lives inside the AlloCore namespace "al"
// - use vocabulary: inherit, subclass, override, constructor, instantiate
//    - Constructor's name is the name of the class (C++ convention)
// - We saw that the constructor was run (because it printed) but then main
//   returned and the whole program shut down.
//

#include "allocore/io/al_App.hpp"
using namespace al;
using namespace std;

struct AlloApp : App {
  AlloApp() { cout << "Created AlloApp" << endl; }
};

int main() { AlloApp app; }
