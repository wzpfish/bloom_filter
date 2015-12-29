#ifndef BLOOM_PARAMETERS_H_
#define BLOOM_PARAMETERS_H_

#include <cstddef>
#include <limits>
#include <cmath>

static const std::size_t kBitsPerChar = 0x08;

// BloomParameters is a simple structure contains the parameters needed
// for bloom filter and it can calculate the best hash table numbers 
// based on the given element numbers and false positive rate.
class BloomParameters {
public:
    BloomParameters()
        : min_table_size(1),
          max_table_size(std::numeric_limits<unsigned long long int>::max()),
          min_number_of_hashes(1),
          max_number_of_hashes(std::numeric_limits<unsigned int>::max()),
          projected_element_number(10000),
          false_positive_probability(1.0 / projected_element_number),
          random_seed(0xA5A5A5A55A5A5A5AULL) {}
    
    virtual ~BloomParameters() {}
    
    //Based on the projected_element_number and false_positive_probability
    //It tries to find the optimal number_of_hashes and table_size.
    virtual bool compute_optimal_parameters(); 

    //Min/max number of the bloom filter size in bits
    unsigned long long int min_table_size;
    unsigned long long int max_table_size;
    
    //Min/max number of hash functions
    unsigned int min_number_of_hashes;
    unsigned int max_number_of_hashes;
    
    //The approximate number of elements to be inserted to
    //the bloom filter, default is 10000.
    unsigned long long int projected_element_number;
    
    //The expected false positive probability of bloom filter.
    //The default is the reciprocal of the projected_element_number.
    double false_positive_probability;

    unsigned long long int random_seed;

    //Optimal hash numbers and table size calculated based on the 
    //projected_element_number and false_positive_probability
    unsigned int number_of_hashes;
    unsigned long long int table_size;
};

#endif
