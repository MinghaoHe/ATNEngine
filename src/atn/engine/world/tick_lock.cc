/*
 * Copyright (C) by MinghaoHe - minghaohe@tencent.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "tick_lock.h"

namespace atn::engine::world {

void TickLock::WakePrimer() { cv_.notify_one(); }

void TickLock::PrimerSleep() {
  ::std::unique_lock<::std::mutex> lock(mutex_);
  cv_.wait(lock, [this] { return flag_.load(); });
  flag_ = false;
}

void TickLock::WakeWorld() { cv_.notify_one(); }

void TickLock::WorldSleep() {
  ::std::unique_lock<::std::mutex> lock(mutex_);
  cv_.wait(lock, [this] { return !flag_.load(); });
  flag_.store(true);
}

}  // namespace atn::engine::world
