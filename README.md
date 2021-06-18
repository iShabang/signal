# Signal
Minimal signal/slot library for C++

## Usage
```c++
#include <iostream>
#include <functional>
#include "Signal.h"

class Object {
 public:
  Object(int val) : value(val) {}
  void onEvent(int id) {
    std::cout << "Object: " << value << std::endl;
    std::cout << "Event called with id: " << id << std::endl;
  }
 private:
  int value;
};

void printSum(int x, int y) {
    std::cout << x + y << std::endl; 
}

void printDiff(int x, int y) {
    std::cout << x - y << std::endl;
}

int main() {
  sig::Signal<void,int,int> sig1; // Template parameters are the return type followed by arguments
                                  // for connected functions
  sig1.connect(printSum);  // Connnect std::function objects with the defined signature 
  sig1.connect(printDiff); // In this case signature is void(int,int)
  sig1(1,2); // Calls printSum and printDiff with arguments 1 and 2


  // Signals can be connected to member functions
  Object obj(5);
  Object obj1(20);
  sig::Signal<void, int> sig;
  // Use std::bind to bind the member instance to the method
  using namespace std::placeholders; //Used for _1, _2, etc
  auto con1 = sig.connect(std::bind(&Object::onEvent, &obj1, _1));
  auto con2 = sig.connect(std::bind(&Object::onEvent, &obj, _1));
  std::cout << "First signal emission" << std::endl;
  sig(10);
  con1->disconnect(); //Disconnect removes functions from the signal's slot list
  con2->disconnect();
  std::cout << "Second signal emission" << std::endl;
  sig(2); // No output. Functions are disconnected.


  // ScopedConnection will automatically disconnect when the connection goes out of scope
  if (true) {
    sig::ScopedConnection sc(sig.connect(std::bind(&Object::onEvent, &obj, _1)));
    sig(25);
  }
  sig(15); // No output here since connection was destroyed
  return 0;
}
```
