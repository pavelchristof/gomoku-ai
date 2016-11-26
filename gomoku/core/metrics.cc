#include "gomoku/core/metrics.h"
#include "tensorflow/core/lib/strings/strcat.h"

namespace gomoku {

void AddMetricMean(Metrics* metrics, const std::string& name, float value) {
  auto metric = metrics->add_metric();
  metric->set_name(name);
  metric->set_mean(value);
}

void AddMetricMean(Metrics* metrics, const std::string& name, int key,
                   float value) {
  auto metric = metrics->add_metric();
  metric->set_name(tensorflow::strings::StrCat(name, "/", key));
  metric->set_mean(value);
}

Metrics* AddMetricGroup(Metrics* metrics, const std::string& name) {
  auto metric = metrics->add_metric();
  metric->set_name(name);
  return metric->mutable_group();
}

}  // namespace gomoku
