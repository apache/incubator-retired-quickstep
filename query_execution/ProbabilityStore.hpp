/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
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
 * @brief A class that stores the probabilities of objects. We use an integer field
 *        called "key" to identify each object.
 *        A probability is expressed as a fraction. All the objects share a common denominator.
 **/
class ProbabilityStore {
 public:
  /**
   * @brief Constructor.
   **/
  ProbabilityStore()
      : common_denominator_(1.0), dist_(0.0, 1.0), engine_(std::random_device()()) {}

  /**
   * @brief Get the number of objects in the store.
   **/
  inline std::size_t getNumObjects() const {
    DCHECK_EQ(individual_probabilities_.size(), cumulative_probabilities_.size());
    return individual_probabilities_.size();
  }

  /**
   * @brief Get the common denominator.
   */
  inline std::size_t getDenominator() const {
    return common_denominator_;
  }

  /**
   * @brief Check if an object with a given key is present.
   * @param key The key of the given object.
   * @return True if the object is present, false otherwise.
   */
  inline bool hasObject(const std::size_t key) const {
    return (individual_probabilities_.find(key) != individual_probabilities_.end());
  }

  /**
   * @brief Add individual (not cumulative) probability for a given object with
   *        updated denominator.
   *
   * @note This function leaves the cumulative probabilities in a consistent
   *       state. An alternative lazy implementation should be written if cost
   *       of calculating cumulative probabilities is high.
   *
   * @param key The key of the given object.
   * @param numerator The numerator for the given object.
   * @param new_denominator The updated denominator for the store.
   **/
  void addOrUpdateObjectNewDenominator(const std::size_t key,
                                       const float numerator,
                                       const float new_denominator) {
    CHECK_GT(new_denominator, 0u);
    DCHECK_LE(numerator, new_denominator);
    common_denominator_ = new_denominator;
    addOrUpdateObjectHelper(key, numerator);
  }

  /**
   * @brief Add or update multiple objects with a new denominator.
   * @param keys The keys to be added/updated.
   * @param numerators The numerators to be added/updated.
   * @param new_denominator The new denominator.
   */
  void addOrUpdateObjectsNewDenominator(
      const std::vector<std::size_t> &keys,
      const std::vector<float> &numerators,
      const float new_denominator) {
    CHECK_GT(new_denominator, 0u);
    common_denominator_ = new_denominator;
    addOrUpdateObjectsHelper(keys, numerators);
  }

  /**
   * @brief Remove an object from the store.
   *
   * @note This function decrements the denominator with the value of the numerator being removed.
   *
   * @param key The key of the object to be removed.
   **/
  void removeObject(const std::size_t key) {
    auto individual_it = individual_probabilities_.find(key);
    DCHECK(individual_it != individual_probabilities_.end());
    const float new_denominator = common_denominator_ - individual_it->second;
    individual_probabilities_.erase(individual_it);
    common_denominator_ = new_denominator;
    updateCumulativeProbabilities();
  }

  /**
   * @brief Get the individual probability (not cumulative) for an object.
   *
   * @param key The key of the object.
   **/
  const float getIndividualProbability(const std::size_t key) const {
    const auto it = individual_probabilities_.find(key);
    DCHECK(it != individual_probabilities_.end());
    DCHECK_NE(0.0, common_denominator_);
    return it->second / common_denominator_;
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
    float cumulative_probability = 0;
    for (const auto p : individual_probabilities_) {
      cumulative_probability += p.second / common_denominator_;
      cumulative_probabilities_.emplace_back(p.first,
                                             cumulative_probability);
    }
    if (!cumulative_probabilities_.empty()) {
      // Adjust the last cumulative probability manually to 1, so that
      // floating addition related rounding issues are ignored.
      cumulative_probabilities_.back().updateProbability(1);
    }
  }

  /**
   * @brief Return a uniformly chosen random key.
   **/
  inline const std::size_t pickRandomKey() const {
    return getKeyForProbability(dist_(engine_));
  }

 private:
  struct ProbabilityInfo {
   public:
    ProbabilityInfo(const std::size_t key, const float probability)
        : key_(key), probability_(probability) {
      DCHECK(probability_ <= 1.0);
    }

    void updateProbability(const float new_probability) {
      probability_ = new_probability;
    }

    const std::size_t key_;
    float probability_;
  };

  /**
   * @brief Get a key for a given cumulative probability.
   *
   * @param key_cumulative_probability The input cumulative probability.
   *
   * @return The object that has a cumulative probability that is greater than
   *         or equal to the input cumulative probability.
   **/
  inline std::size_t getKeyForProbability(
      const float key_cumulative_probability) const {
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
    return it->key_;
  }

  /**
   * @brief Add individual (not cumulative) probability for a given object.
   *
   * @note This function leaves the cumulative probabilities in a consistent
   *       state. An alternative lazy implementation should be written if cost
   *       of calculating cumulative probabilities is high.
   * @note This function may override previously written probability values.
   *
   * @param key The key of the given object.
   * @param numerator The numerator for the given object.
   **/
  void addOrUpdateObjectHelper(const std::size_t key,
                               const float numerator) {
    DCHECK_LE(numerator, common_denominator_);
    individual_probabilities_[key] = numerator;
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
   * @param keys A vector of keys to be added.
   * @param numerators The numerators of the given objects.
   **/
  void addOrUpdateObjectsHelper(const std::vector<std::size_t> &keys,
                                const std::vector<float> &numerators) {
    DCHECK_EQ(keys.size(), numerators.size());
    for (std::size_t i = 0; i < keys.size(); ++i) {
      DCHECK_LE(numerators[i], common_denominator_);
      individual_probabilities_[keys[i]] = numerators[i];
    }
    updateCumulativeProbabilities();
  }

  // Key = key of the object.
  // Value = Numerator of the object.
  std::unordered_map<std::size_t, float> individual_probabilities_;
  std::vector<ProbabilityInfo> cumulative_probabilities_;

  float common_denominator_;

  mutable std::uniform_real_distribution<float> dist_;
  mutable std::default_random_engine engine_;

  DISALLOW_COPY_AND_ASSIGN(ProbabilityStore);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_PROBABILITY_STORE_HPP_
