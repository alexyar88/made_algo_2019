//
// Created by Алексей Ярошенко on 18.12.2019.
//

#ifndef INC_15_EXPERIMENT_H
#define INC_15_EXPERIMENT_H

#include "point_utils.h"
#include "graph.h"
#include "min_path_find.h"
#include "kruskal_approximation.h"

struct ExperimentResult {
    std::vector<float> min_path_lens;
    std::vector<float> approximated_path_lens;

    float MeanMinPath() const;

    float MeanApproximatedPath() const;

    float ApproximationQuality() const;

};

ExperimentResult RunExperiment(int points_count, int repeats);

#endif //INC_15_EXPERIMENT_H
