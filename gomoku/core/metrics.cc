#include "gomoku/core/metrics.h"
#include "tensorflow/core/lib/strings/strcat.h"
#include "tensorflow/core/platform/logging.h"

#include <sstream>

namespace gomoku {

void AddMetricMean(Metrics* metrics, const std::string& name, float value) {
  auto metric = metrics->add_metric();
  metric->set_name(name);
  auto mean = metric->mutable_mean();
  mean->set_value(value);
  mean->set_count(1);
}

void AddMetricMean(Metrics* metrics, const std::string& name, int key,
                   float value) {
  AddMetricMean(metrics, tensorflow::strings::StrCat(name, "/", key), value);
}

Metrics* AddMetricGroup(Metrics* metrics, const std::string& name) {
  auto metric = metrics->add_metric();
  metric->set_name(name);
  return metric->mutable_group();
}

std::vector<Metric> FlattenMetrics(const Metrics& metrics) {
  std::vector<Metric> flat_metrics;
  for (const auto& metric : metrics.metric()) {
    if (metric.value_case() == Metric::kGroup) {
      std::vector<Metric> flat_group = FlattenMetrics(metric.group());
      flat_metrics.insert(flat_metrics.end(), flat_group.begin(),
                          flat_group.end());
    } else {
      flat_metrics.push_back(metric);
    }
  }
  return flat_metrics;
}

namespace {

template <typename Visitor>
void IterateFlatMetrics(const Metrics& metrics, const std::string& path,
                        Visitor visitor) {
  for (const auto& metric : metrics.metric()) {
    auto child_path = tensorflow::strings::StrCat(path, "/", metric.name());
    if (metric.value_case() == Metric::kGroup) {
      IterateFlatMetrics(metric.group(), child_path, visitor);
    } else {
      visitor(child_path, metric);
    }
  }
}

template <typename Visitor>
void IterateFlatMetrics(const Metrics& metrics, Visitor visitor) {
  IterateFlatMetrics(metrics, "", visitor);
}

Metric ConcatMetrics(const Metric& left, const Metric& right) {
  Metric result;
  if (left.value_case() == Metric::kMean &&
      right.value_case() == Metric::kMean) {
    auto mean = result.mutable_mean();
    mean->set_count(left.mean().count() + right.mean().count());
    float total_count = mean->count();
    float left_coeff = left.mean().count() / total_count;
    float right_coeff = right.mean().count() / total_count;
    mean->set_value(left_coeff * left.mean().value() +
                    right_coeff * right.mean().value());
  } else {
    LOG(ERROR) << "Incompatible metrics: " << left.DebugString()
        << ", " << right.DebugString();
  }
  return result;
}

}  // namespace

void MetricCollector::Append(const Metrics& metrics) {
  IterateFlatMetrics(
      metrics, [this] (const std::string& path, const Metric& metric) {
        auto it = metrics_.find(path);
        if (it == metrics_.end()) {
          metrics_[path] = metric;
        } else {
          metrics_[path] = ConcatMetrics(it->second, metric);
        }
      });
}

void MetricCollector::LogMetrics() const {
  std::stringstream ss;
  for (const auto& item : metrics_) {
    ss << "  " << item.first << " = ";
    const auto& metric = item.second;
    switch (metric.value_case()) {
      case Metric::kMean:
        ss << metric.mean().value();
        break;
      default:
        ss << "UNKOWN METRIC TYPE!";
        break;
    }
    ss << "\n";
  }
  LOG(INFO) << "Metrics:\n" << ss.str();
}

}  // namespace gomoku
