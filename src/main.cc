/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include "atn/engine.h"

int main(int argc, char* argv[]) {
  try {
    ::atn::Engine engine(argc, argv);
    engine.Initialize();
    engine.CreateWorld();

    return engine.Exec();
  } catch (::std::exception &e) {
    ::std::cerr << "error: " << e.what();
  }

  return 0;
}
