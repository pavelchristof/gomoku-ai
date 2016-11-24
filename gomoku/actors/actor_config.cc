#include "gomoku/actors/actor_config.h"

namespace gomoku {

bool GetConfigString(const ActorConfig& config, const std::string& key,
                     std::string* value) {
  for (const auto& item : config.item()) {
    if (item.key() == key) {
      *value = item.string();
      return true;
    }
  }
  return false;
}

bool GetConfigFloat(const ActorConfig& config, const std::string& key,
                    float* value) {
  for (const auto& item : config.item()) {
    if (item.key() == key) {
      *value = item.floating();
      return true;
    }
  }
  return false;
}

bool GetConfigInt(const ActorConfig& config, const std::string& key,
                  int* value) {
  for (const auto& item : config.item()) {
    if (item.key() == key) {
      *value = item.integer();
      return true;
    }
  }
  return false;
}

void AddConfigItem(ActorConfig* config, const std::string& key,
                   const std::string& value) {
  auto item = config->add_item();
  item->set_key(key);
  item->set_string(value);
}

void AddConfigItem(ActorConfig* config, const std::string& key,
                   float value) {
  auto item = config->add_item();
  item->set_key(key);
  item->set_floating(value);
}

void AddConfigItem(ActorConfig* config, const std::string& key,
                   int value) {
  auto item = config->add_item();
  item->set_key(key);
  item->set_integer(value);
}

}  // namespace gomoku
