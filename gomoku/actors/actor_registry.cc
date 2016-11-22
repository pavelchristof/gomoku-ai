#include "gomoku/actors/actor_registry.h"

#include "tensorflow/core/platform/logging.h"

namespace gomoku {

ActorRegistry* ActorRegistry::Global() {
  static ActorRegistry actor_registry;
  return &actor_registry;
}

std::unique_ptr<Actor> ActorRegistry::Create(const std::string& type,
                                             const ActorConfig& config) {
  std::unique_lock<std::mutex> lock(mutex_);
  auto iterator = actor_factories_.find(type);
  CHECK(iterator != actor_factories_.end()) << "Unknown actor type " << type;
  return iterator->second(config);
}

bool ActorRegistry::Register(const std::string& type, ActorFactory&& factory) {
  std::unique_lock<std::mutex> lock(mutex_);
  return actor_factories_.insert(
      std::make_pair(type, std::move(factory))).second;
}

ActorRegistrar::ActorRegistrar(const std::string& type,
                               ActorFactory&& factory) {
  CHECK(ActorRegistry::Global()->Register(type, std::move(factory)));
}

}  // namespace gomoku
