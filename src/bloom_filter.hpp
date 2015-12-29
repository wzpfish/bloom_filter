#ifndef BLOOM_FILTER_H_
#define BLOOM_FILTER_H_

#include "bloom_parameters.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "MurmurHash2.h"

static const unsigned char kBitMask[kBitsPerChar] = {
    0x01, //00000001
    0x02, //00000010
    0x04, //00000100
    0x08, //00001000
    0x10, //00010000
    0x20, //00100000
    0x40, //01000000
    0x80, //10000000
};

class BloomFilter {
public:
    BloomFilter()
        : bit_table_(nullptr),
          table_size_(0),
          number_of_hashes_(0),
          projected_element_number_(0),
          inserted_element_number_(0),
          random_seed_(0) {
    }

    BloomFilter(const BloomParameters& p)
        : bit_table_(nullptr),
          table_size_(p.table_size),
          number_of_hashes_(p.number_of_hashes),
          projected_element_number_(p.projected_element_number),
          inserted_element_number_(0),
          random_seed_((p.random_seed * 0xA5A5A5A5) + 1) {
      GenerateUniqueSalt();
      raw_table_size_ = table_size_ / kBitsPerChar;
      bit_table_ = new CellType[raw_table_size_];
      std::fill_n(bit_table_, raw_table_size_, 0x00);
    }

    virtual ~BloomFilter() {
        delete[] bit_table_;
    }
    
    //Insert a key to the bloom filter, key_begin
    //is the pointer to the beginging of the key,
    //and key_length is the length of the key 
    void insert(const unsigned char* key_begin,
                const std::size_t& key_length) {
        for (std::size_t i = 0; i != number_of_hashes_; ++i) {
            auto hash_value = MurmurHash2(
                reinterpret_cast<const void*>(key_begin),
                key_length, 
                salt_[i]);
            std::size_t char_index, bit_index;
            ComputeIndex(hash_value, char_index, bit_index);
            bit_table_[char_index] |= kBitMask[bit_index];
        } 
        ++inserted_element_number_;
    }

    void insert(const char* key, const std::size_t& key_length) {
        insert(reinterpret_cast<const unsigned char*>(key), key_length);
    }

    void insert(const std::string& key) {
        insert(reinterpret_cast<const unsigned char*>(key.c_str()),
               key.size());
    }

    template<typename T>
    void insert(const T& key) {
        insert(reinterpret_cast<const unsigned char*>(&key), sizeof(key)); 
    } 
    
    bool contains(const unsigned char* key_begin,
                  const std::size_t& key_length) {
        for (std::size_t i = 0; i != number_of_hashes_; ++i) {
            auto hash_value = MurmurHash2(
                reinterpret_cast<const void*>(key_begin),
                key_length,
                salt_[i]);
            std::size_t char_index, bit_index;
            ComputeIndex(hash_value, char_index, bit_index);
            if ((bit_table_[char_index] & kBitMask[bit_index]) == 0x00) {
                return false;
            }
        }
        return true;
    }

    bool contains(const char* key, const std::size_t& key_length) {
        return contains(reinterpret_cast<const unsigned char*>(key),
                        key_length);
    }

    bool contains(const std::string& key) {
        return contains(
            reinterpret_cast<const unsigned char*>(key.c_str()),
            key.size());
    }

    template<typename T>
    bool contains(const T& key) {
        return contains(reinterpret_cast<const unsigned char*>(&key),
                        sizeof(key));
    }
    
    unsigned long long int size() const {
        return table_size_;
    }

    std::size_t element_number() const {
        return inserted_element_number_;
    }

    double effective_false_positive_probability() const {
        return std::pow(1 - std::exp(-number_of_hashes_ * inserted_element_number_ / table_size_), number_of_hashes_);
    }


private:
    // Generate number_of_hashes_ unique salts considered 
    // of the random_seed_ user specifies
    void GenerateUniqueSalt() {
        std::srand(static_cast<unsigned int>(random_seed_));
        while (salt_.size() < number_of_hashes_) {
            auto salt = static_cast<unsigned int>(std::rand());
            salt_.push_back(salt);
        }
    }
    
    //Compute the char_index in bit_table_ and the 
    //bit_index in that char, given the hash value.
    //Parameter char_index and bit_index are return 
    //value of this function.
    void ComputeIndex(unsigned int hash_value,
                      std::size_t& char_index,
                      std::size_t& bit_index) {
        char_index = (hash_value % table_size_) / kBitsPerChar;
        bit_index  = (hash_value % table_size_) % kBitsPerChar;
    }

    typedef unsigned int BloomType;
    typedef unsigned char CellType;

    unsigned char* bit_table_;
    unsigned long long int table_size_;
    unsigned long long int raw_table_size_;

    unsigned long long int projected_element_number_;
    unsigned long long int inserted_element_number_;
    
    unsigned int number_of_hashes_;
    std::vector<unsigned int> salt_;
    unsigned long long int random_seed_;
    
};

#endif

