/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once
#ifndef INCLUDE_ATN_ENGINE_WORLD_TICK_LOCK
#define INCLUDE_ATN_ENGINE_WORLD_TICK_LOCK

#include <atomic>
#include <mutex>

namespace atn::engine::world {

class TickLock final {
 public:
  void WakePrimer();
  void PrimerSleep();
  void WakeWorld();
  void WorldSleep();

 private:
  ::std::atomic_bool flag_{true};
  ::std::mutex mutex_;
  ::std::condition_variable cv_;
};

}  // namespace atn::engine::world

#endif  // !INCLUDE_ATN_ENGINE_WORLD_TICK_LOCK
