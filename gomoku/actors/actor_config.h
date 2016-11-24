#ifndef GOMOKU_ACTORS_ACTOR_CONFIG_H_
#define GOMOKU_ACTORS_ACTOR_CONFIG_H_

#include "gomoku/actors/actor_spec.pb.h"

namespace gomoku {

// Finds a string with the given key in the configuration.
bool GetConfigString(const ActorConfig& config, const std::string& key,
                     std::string* value);

// Finds a float with the given key in the configuration.
bool GetConfigFloat(const ActorConfig& config, const std::string& key,
                    float* value);

// Finds an integer with the given key in the configuration.
bool GetConfigInt(const ActorConfig& config, const std::string& key,
                  int* value);

// Adds a string item to the given configuration.
void AddConfigItem(ActorConfig* config, const std::string& key,
                   const std::string& value);

// Adds a float item to the given configuration.
void AddConfigItem(ActorConfig* config, const std::string& key,
                   float value);

// Adds an int item to the given configuration.
void AddConfigItem(ActorConfig* config, const std::string& key,
                   int value);

}  // namespace gomoku

#endif  // GOMOKU_ACTORS_ACTOR_CONFIG_H_
