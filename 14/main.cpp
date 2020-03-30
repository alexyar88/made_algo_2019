#include <iostream>
#include "experiment.h"
#include <iomanip>


int main() {
    int repeats = 5;
    std::cout << std::endl;
    std::cout << "min1..min5 - exact min salesman path" << std::endl;
    std::cout << "approx1..approx5 - approximated min salesman path" << std::endl;
    std::cout << std::endl;
    std::cout
            << std::setw(2) << std::right << "N" << " | "
            << std::setw(8) << std::right << "min1" << " | "
            << std::setw(8) << std::right << "approx1" << " | "
            << std::setw(8) << std::right << "min2" << " | "
            << std::setw(8) << std::right << "approx2" << " | "
            << std::setw(8) << std::right << "min3" << " | "
            << std::setw(8) << std::right << "approx3" << " | "
            << std::setw(8) << std::right << "min4" << " | "
            << std::setw(8) << std::right << "approx4" << " | "
            << std::setw(8) << std::right << "min5" << " | "
            << std::setw(8) << std::right << "approx5" << " | "

            << std::setw(12) << std::right << "Min Mean" << " | "
            << std::setw(12) << std::right << "Approx. Mean" << " | "
            << std::setw(12) << std::right << "Quality" << " | "
            << std::endl;

    for (int i = 0; i < 158; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;


    for (int points_count = 2; points_count < 11; ++points_count) {
        ExperimentResult result = RunExperiment(points_count, repeats);

        std::cout << std::setw(2) << std::right << points_count << " | ";
        for (int i = 0; i < repeats; ++i) {
            std::cout << std::setw(8) << std::right << result.min_path_lens[i] << " | ";
            std::cout << std::setw(8) << std::right << result.approximated_path_lens[i] << " | ";
        }

        std::cout
                << std::setw(12) << std::right << result.MeanMinPath() << " | "
                << std::setw(12) << std::right << result.MeanApproximatedPath() << " | "
                << std::setw(12) << std::right << result.ApproximationQuality() << " | "
                << std::endl;
    }


    return 0;
}
