#include "bloom_filter.hpp"
#include <iostream>
#include <string>

int main() {
    BloomParameters bp;
    bp.projected_element_number = 20000;
    bp.false_positive_probability = 1.0 / bp.projected_element_number;
    bp.compute_optimal_parameters();
    BloomFilter bloom(bp);
    
    int numbers[5] = {1, 2, 3, 4, 5};
    std::string s[5] = {"hello", "xz", "I", "love", "U"};
    
    for (auto it: numbers) {
        bloom.insert(it);
    }
    for (auto it: s) {
        bloom.insert(it);
    }

    if (bloom.contains(std::string("hello"))) {
      std::cout << "hello is in bloom" << std::endl;  
    }
    if (bloom.contains(2)) {
      std::cout << "2 is in bloom" << std::endl;  
    }

    return 0;
}
