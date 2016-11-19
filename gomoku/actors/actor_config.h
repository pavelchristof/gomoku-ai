#ifndef GOMOKU_ACTORS_ACTOR_CONFIG_H_
#define GOMOKU_ACTORS_ACTOR_CONFIG_H_

#include "gomoku/actors/actor_spec.pb.h"

namespace gomoku {

// Finds a string with the given key in the configuration.
bool GetString(const ActorConfig& config, const std::string& key,
               std::string* value);

// Finds a float with the given key in the configuration.
bool GetFloat(const ActorConfig& config, const std::string& key, float* value);

// Finds an integer with the given key in the configuration.
bool GetInt(const ActorConfig& config, const std::string& key, int* value);

}  // namespace gomoku

#endif  // GOMOKU_ACTORS_ACTOR_CONFIG_H_
