
// minimal c++ program
//
// - show how this results in a binary executable in build/bin
// - #include is the C preprocessor. it lets you add functionallity to your program.
// - including iostream gets us std::cout and std::endl
// - functions have name, return type, arguments, body/block in curly braces executed
// - main() is a function that special: it's called by the operating system to start your program
// - C++ has a stream operator (i.e., <<). cout is the name of the thing that prints to the console
// - std::endl includes a newline (like hiting the return key)
// - namespaces are a way of organizing code and avoiding name collisions
// - the "std" namespace holds the C++ standard library
// - "coloncolon" (::) operator aka "scope resolution operator"
//

#include <iostream>
int main() {
  std::cout << "Hello, world!" << std::endl;
}

