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
#include <ARC_cach.hpp>
#include <cach_tester.hpp>


ARC_cach::ARC_cach_t<int,int> c (0);

int get_page(int key)
{
    return key;
}

int test_looker(int elem)
{
    //c.debug_print();
    return c.abdate_cach(elem, get_page);
}

int main()
{
    
    size_t cach_sz = 0;
    std::cin >> cach_sz; 

    size_t n_data = 0;
    std::cin >> n_data;

    c.change_sz(cach_sz);
    
    size_t i = 0;

    cache_tester::cache_tester_t test (cach_sz);

    int buf = 0;
    for(i = 0; i < n_data; i++)
    {
        std::cin >> buf;
        test.test_add(buf);
    }

    clock_t cach_time = clock();
    test.cash_test(test_looker);
    cach_time = clock() - cach_time;

    clock_t ideal_time = clock();
    test.start_IDEAL();
    ideal_time = clock() - ideal_time;
    
    std::ios old_format(nullptr);
    old_format.copyfmt(std::cout);

    std::cout << "mycach  [" << std::fixed;
    std::cout.precision(3);
    std::cout << ((double)cach_time)/CLOCKS_PER_SEC;
    std::cout.copyfmt(old_format);
    std::cout << " s]: " << n_data - test.return_test() << std::endl;
    
    std::cout << "idealch [" << std::fixed;
    std::cout.precision(3);
    std::cout << ((double)ideal_time)/CLOCKS_PER_SEC;
    std::cout.copyfmt(old_format);
    std::cout << " s]: " << n_data - test.return_id() << std::endl;

}