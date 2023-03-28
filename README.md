# Cashe
This is a project to implement an ARP cache in c++ and test its performance. It can also be used as a data container in systems with limited memory, in which the cost of obtaining data is extremely high, for example browsers.

# ARC_cache
## How to use
The container of the ARC cache is template class `ARC_cache_t`

To access the data cell, call the function
`T get_elem(const KeyT key, T get_elem_to_mem(const KeyT key))`
* `key` - the key to the data
* `T get_elem_to_men(const KeyT key))` - function for getting data by key

## Debug functions
`debug_Print()` is a function that outputs the contents of the cache to it in accordance with its structure.

## Other tulchane for test
If you need to read a cache element without reading, you can use  `int update_cache(const KeyT key, T get_elem_to_mem(const KeyT key))`. At the same time, in case of a cache miss, the function returns 1, otherwise 0

# Perfomence test
This program can automatically test the function for the number of caches. At the same time, an ideal cache algorithm is implemented to analyze the quality of the generated test.

## How to make
To build a project, you need to write `make` in the root folder of the project.
## How to generate test 
To generate test you can use `gen_test.py`
## How to test
To start perfomance test, you need to wright `make start`.


