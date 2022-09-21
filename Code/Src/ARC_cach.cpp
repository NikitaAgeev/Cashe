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
    
    printf("Введите размер кеша: ");
    size_t cach_sz = 0;
    scanf("%lu", &cach_sz); 

    printf("Введите количество данных: ");
    size_t n_data = 0;
    scanf("%lu", &n_data);

    c.change_sz(cach_sz);
    
    size_t i = 0;

    cach_tester::cach_tester_t test (cach_sz);

    int buf = 0;
    for(i = 0; i < n_data; i++)
    {
        scanf("%d", &buf);
        test.test_add(buf);
    }

    test.cash_test(test_looker);
    test.start_IDEAL();

    printf("cach_miss: %lu\n", test.return_test());
    printf("ideal: %lu\n", test.return_id());

}