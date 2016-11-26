#ifndef GOMOKU_CORE_METRICS_H_
#define GOMOKU_CORE_METRICS_H_

#include "gomoku/core/metrics.pb.h"

namespace gomoku {

// Adds a metric that computes the mean of all samples.
void AddMetricMean(Metrics* metrics, const std::string& name, float value);
void AddMetricMean(Metrics* metrics, const std::string& name, int key,
                   float value);

// Adds a new metric group and returns a pointer to it.
Metrics* AddMetricGroup(Metrics* metrics, const std::string& name);

}  // namespace gomoku

#endif  // GOMOKU_CORE_METRICS_H_
