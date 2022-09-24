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
    
    //printf("Введите размер кеша: ");
    size_t cach_sz = 0;
    scanf("%lu", &cach_sz); 

    //printf("Введите количество данных: ");
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

    clock_t cach_time = clock();
    test.cash_test(test_looker);
    cach_time = clock() - cach_time;

    clock_t ideal_time = clock();
    test.start_IDEAL();
    ideal_time = clock() - ideal_time;

    printf("mycach [%.3lf s]: %lu\n", ((double)cach_time)/CLOCKS_PER_SEC,  test.return_test());
    printf("idealc [%.3lf s]: %lu\n", ((double)ideal_time)/CLOCKS_PER_SEC, test.return_id());

}