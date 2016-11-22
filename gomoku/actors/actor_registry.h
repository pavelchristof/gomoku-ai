#ifndef GOMOKU_ACTORS_ACTOR_REGISTRY_H_
#define GOMOKU_ACTORS_ACTOR_REGISTRY_H_

#include <mutex>

#include "gomoku/core/actor.h"
#include "gomoku/actors/actor_spec.pb.h"

namespace gomoku {

// A factory that creates actors from a config. Should be thread safe.
using ActorFactory = std::function<std::unique_ptr<Actor>(const ActorConfig&)>;

// Maps type names to actor factories. Thread-safe.
class ActorRegistry {
 public:
  // Returns the global actor factory.
  static ActorRegistry* Global();

  // Creates a new instance of an actor.
  std::unique_ptr<Actor> Create(const std::string& type,
                                const ActorConfig& config);

  // Register a new actor. Fails if the type was already registered.
  bool Register(const std::string& type, ActorFactory&& factory);

  std::map<std::string, ActorFactory> Actors() { return actor_factories_; }

 private:
  std::mutex mutex_;
  std::map<std::string, ActorFactory> actor_factories_;
};

// A class that registers an actor on initialization.
class ActorRegistrar {
 public:
  ActorRegistrar(const std::string& type, ActorFactory&& factory);
};

// Registers an actor. For example:
//   REGISTER_ACTOR("TestActor", [] (const ActorConfig&) {
//     return std::shared_ptr<Actor>(new TestActor());
//   });
// The type names should be unique and in CamelCase.
#define REGISTER_ACTOR(type, factory) \
    static ::gomoku::ActorRegistrar actor_registrar ## __COUNTER__ = \
        ::gomoku::ActorRegistrar(type, factory)

}  // namespace gomoku

#endif  // GOMOKU_ACTORS_ACTOR_REGISTRY_H_
