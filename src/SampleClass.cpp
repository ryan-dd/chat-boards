#include "SampleClass.h"
#include <spdlog/spdlog.h>

void SampleClass::do_something()
{
  spdlog::info("Hello, {}!", "World");
}
