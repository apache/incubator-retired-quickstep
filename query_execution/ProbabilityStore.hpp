/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_QUERY_EXECUTION_PROBABILITY_STORE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_PROBABILITY_STORE_HPP_

#include <algorithm>
#include <cstddef>
#include <random>
#include <unordered_map>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief A class that stores the probabilities of objects. We use a field
 *        called "property" to identify each object.
 **/
class ProbabilityStore {
 public:
  /**
   * @brief Constructor.
   **/
  ProbabilityStore()
      : mt_(std::random_device()()) {}

  /**
   * @brief Get the number of objects in the store.
   **/
  const std::size_t getNumObjects() const {
    DCHECK_EQ(individual_probabilities_.size(), cumulative_probabilities_.size());
    return individual_probabilities_.size();
  }

  /**
   * @brief Add individual (not cumulative) probability for a given object.
   *
   * @note This function leaves the cumulative probabilities in a consistent
   *       state. An alternative lazy implementation should be written if cost
   *       of calculating cumulative probabilities is high.
   * @note This function may override previously written probability values.
   *
   * @param property The property of the given object.
   * @param individual_probability The individual (not cumulative) probability
   *        of the given object.
   **/
  void addProbability(const std::size_t property,
                      const float individual_probability) {
    individual_probabilities_[property] = individual_probability;
    updateCumulativeProbabilities();
  }

  /**
   * @brief Add individual (not cumulative) probabilities for given objects.
   *
   * @note This function leaves the cumulative probabilities in a consistent
   *       state. An alternative lazy implementation should be written if cost
   *       of calculating cumulative probabilities is high.
   * @note This function may override previously written probability values.
   *
   * @param properties A vector of properties to be added.
   * @param individual_probabilities The individual (not cumulative)
   *        probabilities of the given objects.
   **/
  void addProbabilities(const std::vector<std::size_t> &properties,
                        const std::vector<float> &individual_probabilities) {
    DCHECK_EQ(properties.size(), individual_probabilities.size());
    for (std::size_t i = 0; i < properties.size(); ++i) {
      individual_probabilities_[properties[i]] = individual_probabilities[i];
    }
    updateCumulativeProbabilities();
  }

  /**
   * @brief Update  the probability of a given object to a new value.
   *
   * @param property The property of the object.
   * @param new_individual_probability The new probability to be set.
   **/
  void updateProbability(const std::size_t property,
                         const float new_individual_probability) {
    auto it = individual_probabilities_.find(property);
    DCHECK(it != individual_probabilities_.end());
    it->second = new_individual_probability;
    updateCumulativeProbabilities();
  }

  /**
   * @brief Remove an object from the store.
   *
   * @param property The property of the object to be removed.
   **/
  void removeObject(const std::size_t property) {
    auto it = individual_probabilities_.find(property);
    DCHECK(it != individual_probabilities_.end());
    individual_probabilities_.erase(it);
    updateCumulativeProbabilities();
  }

  /**
   * @brief Get the individual probability (not cumulative) for an object.
   *
   * @param property The property of the object.
   **/
  const float getIndividualProbability(const std::size_t property) const {
    const auto it = individual_probabilities_.find(property);
    DCHECK(it != individual_probabilities_.end());
    return it->second;
  }

  /**
   * @brief Update the cumulative probabilities.
   *
   * @note This function should be called upon if there are any updates,
   *       additions or deletions to the individual probabilities.
   * @note An efficient implementation should be written if there are large
   *       number of objects.
   **/
  void updateCumulativeProbabilities() {
    cumulative_probabilities_.clear();
    if (individual_probabilities_.empty()) {
      // No need to modify the cumulative probabilities.
      return;
    }
    float cumulative_probability = 0;
    for (const auto property_probability_pair : individual_probabilities_) {
      cumulative_probabilities_.emplace_back(property_probability_pair.first,
                                             cumulative_probability);
      cumulative_probability += property_probability_pair.second;
    }
    // Adjust the last cumulative probability manually to 1.0, so that floating
    // addition related rounding issues are ignored.
    cumulative_probabilities_.back().updateProbability(1.0);
  }

  /**
   * @brief Return a randomly chosen property.
   *
   * @note The random number is uniformly chosen.
   **/
  inline const std::size_t pickRandomProperty() {
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    const float chosen_probability = dist(mt_);
    return getPropertyForProbability(chosen_probability);
  }

 private:
  class ProbabilityInfo {
   public:
    ProbabilityInfo(const std::size_t property, const float probability)
        : property_(property), probability_(probability) {
      DCHECK_LE(probability, 1.0);
    }

    ProbabilityInfo(const ProbabilityInfo &other) = default;

    ProbabilityInfo& operator=(const ProbabilityInfo &other) = default;

    void updateProbability(const float new_probability) {
      DCHECK_LE(new_probability, 1.0);
      probability_ = new_probability;
    }

    std::size_t property_;
    float probability_;
  };

  /**
   * @brief Get a property for a given cumulative probability.
   *
   * @param key_cumulative_probability The input cumulative probability.
   *
   * @return The object that has a cumulative probability that is greater than
   *         or equal to the input cumulative probability.
   **/
  inline const std::size_t getPropertyForProbability(
      const float key_cumulative_probability) {
    DCHECK(!cumulative_probabilities_.empty());
    // It doesn't matter in which order the objects are arranged in the
    // cumulative_probabilities_ vector.
    ProbabilityInfo search_key(0, key_cumulative_probability);
    const auto it = std::upper_bound(
        cumulative_probabilities_.begin(),
        cumulative_probabilities_.end(),
        search_key,
        [](const ProbabilityInfo &a, const ProbabilityInfo &b) {
          return a.probability_ < b.probability_;
        });
    DCHECK(it != std::end(cumulative_probabilities_));
    return it->property_;
  }

  std::unordered_map<std::size_t, float> individual_probabilities_;
  std::vector<ProbabilityInfo> cumulative_probabilities_;

  std::mt19937_64 mt_;

  DISALLOW_COPY_AND_ASSIGN(ProbabilityStore);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_PROBABILITY_STORE_HPP_
