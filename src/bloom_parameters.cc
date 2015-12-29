#include "bloom_parameters.h"

bool BloomParameters::compute_optimal_parameters() {
    double optimal_m = std::numeric_limits<double>::infinity();
    double optimal_k = 0;
    double k = 0;

    while (k <= 1000) {
        double numerator = - (k * projected_element_number);
        double denominator = std::log(
            1 - std::pow(false_positive_probability, 1.0 / k));
        double cur_m = numerator / denominator;
        if (cur_m < optimal_m) {
            optimal_m = cur_m;
            optimal_k = k;
        }
        ++k;
    }

    number_of_hashes = static_cast<unsigned int>(optimal_k);
    table_size = static_cast<unsigned long long int>(optimal_m);
    table_size += (table_size % kBitsPerChar != 0) ? 
        (kBitsPerChar - table_size % kBitsPerChar) : 0;
    
    if (number_of_hashes < min_number_of_hashes) {
        number_of_hashes = min_number_of_hashes;
    }
    else if (number_of_hashes > max_number_of_hashes) {
        number_of_hashes = max_number_of_hashes;
    }

    if (table_size < min_table_size) {
        table_size = min_table_size;
    }
    else if (table_size > max_table_size) {
        table_size = max_table_size;
    }

    return true;
}


