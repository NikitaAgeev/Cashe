#include <iterator>
#include <stdlib.h>
#include <list>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <time.h>
#include <iomanip>

#include <qline.hpp>
#include <ARC_cache.hpp>
#include <cache_tester.hpp>


ARC_cache::ARC_cache_t<int,int> c (0);

int get_page(const int key)
{
    return key;
}

int test_looker(const int elem)
{
    //c.debug_print();
    return c.update_cache(elem, get_page);
}

int main()
{
    
    ssize_t cache_sz = 0;
    std::cin >> cache_sz; 

    ssize_t n_data = 0;
    std::cin >> n_data;

    c.change_sz(cache_sz);
    
    ssize_t i = 0;

    cache_tester::cache_tester_t test (cache_sz);

    int buf = 0;
    for(i = 0; i < n_data; i++)
    {
        std::cin >> buf;
        test.test_add(buf);
    }

    clock_t cache_time = clock();
    test.cash_test(test_looker);
    cache_time = clock() - cache_time;

    clock_t ideal_time = clock();
    test.start_IDEAL();
    ideal_time = clock() - ideal_time;
    
    std::ios old_format(nullptr);
    old_format.copyfmt(std::cout);

    std::cout << "mycache  [" << std::fixed;
    std::cout.precision(3);
    std::cout << ((double)cache_time)/CLOCKS_PER_SEC;
    std::cout.copyfmt(old_format);
    std::cout << " s]: " << n_data - test.return_test() << std::endl;
    
    std::cout << "idealch [" << std::fixed;
    std::cout.precision(3);
    std::cout << ((double)ideal_time)/CLOCKS_PER_SEC;
    std::cout.copyfmt(old_format);
    std::cout << " s]: " << n_data - test.return_id() << std::endl;

}