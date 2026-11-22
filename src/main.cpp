#include <iostream>

/**
 *
 * @arg x
 * @return sum of x and y
 */
int add(int x, int y) {
  int s = x + y;
  std::cout << "SUM: " << s << '\n';
  return x + y;
}

void hello() { std::cout << "hello again" << std::endl; }

int main() {

  hello();
  add(5, 4);
  std::cout << "This code works" << std::endl;
  return 0;
}
