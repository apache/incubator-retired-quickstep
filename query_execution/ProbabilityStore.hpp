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
#include <utility>
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
      : common_denominator_(1.0), mt_(std::random_device()()) {}

  /**
   * @brief Get the number of objects in the store.
   **/
  const std::size_t getNumObjects() const {
    DCHECK_EQ(individual_probabilities_.size(), cumulative_probabilities_.size());
    return individual_probabilities_.size();
  }

  inline const std::size_t getDenominator() const {
    return common_denominator_;
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
   * @param numerator The numerator for the given object.
   **/
  void addOrUpdateObject(const std::size_t property,
                         const float numerator) {
    DCHECK_LE(numerator, common_denominator_);
    // We should have the correct individual probability in
    // individual_probabilities_ for the newly added object at this point.
    // Because we rely on the probabilities for all the objects in
    // updateCumulativeProbabilities().
    individual_probabilities_[property] =
        std::make_pair(numerator, numerator / common_denominator_);
    updateCumulativeProbabilities();
  }

  /**
   * @brief Add individual (not cumulative) probability for a given object with
   *        updated denominator.
   *
   * @note This function leaves the cumulative probabilities in a consistent
   *       state. An alternative lazy implementation should be written if cost
   *       of calculating cumulative probabilities is high.
   * @note This function may override previously written probability values.
   *
   * @param property The property of the given object.
   * @param numerator The numerator for the given object.
   * @param new_denominator The updated denominator for the store.
   **/
  void addOrUpdateObjectNewDenominator(const std::size_t property,
                                       const float numerator,
                                       const float new_denominator) {
    CHECK_GT(new_denominator, 0u);
    DCHECK_LE(numerator, new_denominator);
    common_denominator_ = new_denominator;
    // It is alright to not have the correct probability in
    // individual_probabilities_ for the newly added object at this point.
    // Because we compute the probabilities for all the objects in
    // updateProbabilitiesNewDenominator().
    individual_probabilities_[property] = std::make_pair(numerator, 0.0);
    updateProbabilitiesNewDenominator();
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
   * @param numerators The numerators of the given objects.
   **/
  void addOrUpdateObjects(const std::vector<std::size_t> &properties,
                          const std::vector<float> &numerators) {
    DCHECK_EQ(properties.size(), numerators.size());
    for (std::size_t i = 0; i < properties.size(); ++i) {
      DCHECK_LE(numerators[i], common_denominator_);
      // We should have the correct individual probability in
      // individual_probabilities_ for the newly added object at this point.
      // Because we rely on the probabilities for all the objects in
      // updateCumulativeProbabilities().
      individual_probabilities_[properties[i]] =
          std::make_pair(numerators[i], numerators[i] / common_denominator_);
    }
    updateCumulativeProbabilities();
  }

  void addOrUpdateObjectsNewDenominator(
      const std::vector<std::size_t> &properties,
      const std::vector<float> &numerators,
      const float new_denominator) {
    CHECK_GT(new_denominator, 0u);
    DCHECK_EQ(properties.size(), numerators.size());
    common_denominator_ = new_denominator;
    for (std::size_t i = 0; i < properties.size(); ++i) {
      DCHECK_LE(numerators[i], common_denominator_);
      // It is alright to not have the correct probability in
      // individual_probabilities_ for the newly added object at this point.
      // Because we compute the probabilities for all the objects in
      // updateProbabilitiesNewDenominator().
      individual_probabilities_[properties[i]] =
          std::make_pair(numerators[i], 0.0);
    }
    updateProbabilitiesNewDenominator();
  }

  /**
   * @brief Remove an object from the store.
   *
   * @param property The property of the object to be removed.
   **/
  void removeObject(const std::size_t property) {
    auto individual_it = individual_probabilities_.find(property);
    DCHECK(individual_it != individual_probabilities_.end());
    individual_probabilities_.erase(individual_it);
    if (!individual_probabilities_.empty()) {
      float new_denominator = 0;
      for (auto it = individual_probabilities_.begin();
           it != individual_probabilities_.end();
           ++it) {
        new_denominator += it->second.first;
      }
      CHECK_GT(new_denominator, 0);
      common_denominator_ = new_denominator;
      updateCumulativeProbabilities();
    } else {
      // In order to keep the store consistent, we should keep the sizes of
      // individual_probabilities_ and cumulative_probabilities_ the same.
      cumulative_probabilities_.clear();
    }
  }

  /**
   * @brief Get the individual probability (not cumulative) for an object.
   *
   * @param property The property of the object.
   **/
  const float getIndividualProbability(const std::size_t property) const {
    const auto it = individual_probabilities_.find(property);
    DCHECK(it != individual_probabilities_.end());
    return it->second.second;
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
    for (const auto p : individual_probabilities_) {
      cumulative_probabilities_.emplace_back(p.first,
                                             cumulative_probability);
      cumulative_probability += p.second.second;
    }
    // Adjust the last cumulative probability manually to 1.0, so that
    // floating addition related rounding issues are ignored.
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

  inline void updateProbabilitiesNewDenominator() {
    // First update the individual probabilities.
    for (auto it = individual_probabilities_.begin();
         it != individual_probabilities_.end();
         ++it) {
      DCHECK_LE(it->second.first, common_denominator_);
      it->second.second = it->second.first / common_denominator_;
    }
    updateCumulativeProbabilities();
  }

  // Key = property of the object.
  // Value = A pair ...
  // 1st element: Numerator of the object.
  // 2nd element: Individual probability of the object.
  std::unordered_map<std::size_t, std::pair<float, float>> individual_probabilities_;
  std::vector<ProbabilityInfo> cumulative_probabilities_;

  float common_denominator_;

  std::mt19937_64 mt_;

  DISALLOW_COPY_AND_ASSIGN(ProbabilityStore);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_PROBABILITY_STORE_HPP_
