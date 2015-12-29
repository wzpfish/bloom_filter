# Bloom Filter实现

## 介绍
bloom filter是一个数据结构，用来保存已经出现过的key。它一般包含两个操作，
insert 和 contains，顾名思义，insert用来插入key，contains判断key是否存在。

bloom filter的优势在于当数据量很大时，相对与其他结构（如leveldb），它可以节约内存，并且十分高效。

## 原理
原理很简单，它维护一个bit数组。当插入一个key时，通过K个hash函数，得到K个整数值，并将这K个整数值对应的bit数组的位置置1。当判断一个key是否存在时，同样通过K个hash函数，得到K个整数，如果这K个整数对应的bit位置都为1，则key存在。

当然，这样做有一定的误差，就是会把不存在的key误判为存在，也即存在一个false positive rate，这个rate可以根据实际容错需求自己设置。在我的实现中，hash个数和bit数组大小会根据设置的rate自动调整。

更详细的原理参考: http://billmill.org/bloomfilter-tutorial/

## 实现
实现主要参考了https://code.google.com/p/bloom/ ，大致相同，有些地方（如hash）有细微区别。

BloomParameters类用来设置参数，并根据false positive probability自动计算最优参数。

BloomFilter类实现insert和contains函数。
