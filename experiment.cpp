//
// Created by Алексей Ярошенко on 18.12.2019.
//

#include "experiment.h"
#include <iostream>


float ExperimentResult::MeanMinPath() const {
    float mean = 0;
    for (const float min_len : min_path_lens) {
        mean += min_len;
    }
    return mean / min_path_lens.size();
}

float ExperimentResult::MeanApproximatedPath() const {
    float mean = 0;
    for (const float approximated_len : approximated_path_lens) {
        mean += approximated_len;
    }
    return mean / approximated_path_lens.size();
}

float ExperimentResult::ApproximationQuality() const {
    return MeanMinPath() / MeanApproximatedPath();
}

ExperimentResult RunExperiment(int points_count, int repeats) {
    ExperimentResult result;

    for (int i = 0; i < repeats; ++i) {
        std::vector<Point> points = GeneratePoints(points_count);
        result.min_path_lens.push_back(FindMinPath(points));
        result.approximated_path_lens.push_back(ApproximatedPath(points));
    }

    return result;
}
