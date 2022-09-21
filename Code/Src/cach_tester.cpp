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
#include <cach_tester.hpp>

namespace cach_tester
{
    
    void cach_tester_t::test_add(int elem)
    {
        test.push_back(elem);
    }

    void cach_tester_t::add_elelm(int elem, size_t iter)
    {
        
        auto searcher = ff_table.find(elem);
        
        if(searcher != ff_table.end())
        {
            searcher->second.push_back(iter);
        }
        else
        {
            ff_table.insert({elem, {}});

            auto searcher = ff_table.find(elem);
            searcher->second.push_back(iter);
        }

    }

    void cach_tester_t::test_ff_ininter ()
    {
        
        auto itter = test.begin();
        size_t i = 0;

        for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
        {
            add_elelm(*itter, i);
        }

    }

    int cach_tester_t::find_m_far_ff ()
    {
        
        auto itter = mem_list.begin();
        size_t i = 0;
        int far_elem = -1;
        size_t len_to_fel = 0;


        for(itter = mem_list.begin(), i = 0; itter != mem_list.lend(); ++itter, i++)
        {
            auto elem_pos_mas = (ff_table.find(itter->second))->second;
            
            if(elem_pos_mas.begin() == elem_pos_mas.end())
            {
                return itter->second;
            }
            else if(*(elem_pos_mas.begin()) > len_to_fel)
            {
                len_to_fel = *(elem_pos_mas.begin());
                far_elem = itter->second;
            }
        }

        return far_elem;

    }

    void cach_tester_t::start_IDEAL ()
    {
        this->test_ff_ininter();
        
        auto itter = test.begin();
        size_t i = 0;

        ideal_mising_ = 0;

        for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
        {
            
            if(mem_list.find(*itter) == mem_list.end())
            {
                if(mem_list.size() < sz_)
                {
                    
                    mem_list.push_tf(*itter, *itter);
                    
                }
                else        
                {
                    
                    //printf("ID_err %ld: %d\n", i, find_m_far_ff());

                    mem_list.erase(find_m_far_ff());
                    mem_list.push_tf(*itter, *itter);
                    
                    
                }
                ideal_mising_++;
            }

            auto it_ff_list = ff_table.find(*itter)->second;
            
            if(it_ff_list.begin() != it_ff_list.end())
            {
                ff_table.find(*itter)->second.erase(ff_table.find(*itter)->second.begin());
            }

        }

    }

    void cach_tester_t::cash_test(int cach_tester (int elelm))
    {
        auto itter = test.begin();
        size_t i = 0;

        test_mising_ = 0;

        for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
        {
            
            if(cach_tester(*itter))
            {
                test_mising_++;
            }

        }
    }

    size_t cach_tester_t::return_id ()
    {
        return ideal_mising_;
    }

    size_t cach_tester_t::return_test ()
    {
        return test_mising_;
    }

}