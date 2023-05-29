#include <iostream>
#include "runtimeEnv.h"

int main()
{
  static RuntimeEnv nachOS;
  nachOS.Run();
  return 0;
}
