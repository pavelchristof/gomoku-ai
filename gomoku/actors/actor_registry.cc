#include "gomoku/actors/actor_registry.h"

#include "tensorflow/core/platform/logging.h"

namespace gomoku {

ActorRegistry* ActorRegistry::Global() {
  static ActorRegistry actor_registry;
  return &actor_registry;
}

std::unique_ptr<Actor> ActorRegistry::Create(const std::string& name,
                                             const ActorConfig& config) {
  std::unique_lock<std::mutex> lock(mutex_);
  auto iterator = actor_factories_.find(name);
  CHECK(iterator != actor_factories_.end()) << "Unknown actor name " << name;
  return iterator->second(config);
}

// Register a new actor. Fails if the name was already used.
bool ActorRegistry::Register(const std::string& name, ActorFactory&& factory) {
  std::unique_lock<std::mutex> lock(mutex_);
  return actor_factories_.insert(
      std::make_pair(name, std::move(factory))).second;
}

ActorRegistrar::ActorRegistrar(const std::string& name,
                               ActorFactory&& factory) {
  CHECK(ActorRegistry::Global()->Register(name, std::move(factory)));
}

}  // namespace gomoku
