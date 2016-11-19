#include "gomoku/actors/actor_config.h"

namespace gomoku {

bool GetString(const ActorConfig& config, const std::string& key,
               std::string* value) {
  for (const auto& item : config.item()) {
    if (item.key() == key) {
      *value = item.string();
      return true;
    }
  }
  return false;
}

bool GetFloat(const ActorConfig& config, const std::string& key, float* value) {
  for (const auto& item : config.item()) {
    if (item.key() == key) {
      *value = item.floating();
      return true;
    }
  }
  return false;
}

bool GetInt(const ActorConfig& config, const std::string& key, int* value) {
  for (const auto& item : config.item()) {
    if (item.key() == key) {
      *value = item.integer();
      return true;
    }
  }
  return false;
}

}  // namespace gomoku
