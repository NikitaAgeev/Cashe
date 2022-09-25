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

namespace cache_tester
{
    
    void cache_tester_t::test_add(int elem)
    {
        test.push_back(elem);
    }

    void cache_tester_t::add_elelm(int elem, size_t iter)
    {
        
        if(ff_table.find(elem) != ff_table.end())
        {
            ff_table.find(elem)->second.push(iter);
        }
        else
        {
            ff_table.insert({elem, micro_line_t(iter)});
        }

    }

    void cache_tester_t::test_ff_ininter ()
    {
        
        auto itter = test.begin();
        size_t i = 0;

        for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
        {
            add_elelm(*itter, i);
        }

    }

    int cache_tester_t::find_m_far_ff ()
    {
        
        auto itter = mem_list.begin();
        size_t i = 0;
        int far_elem = -1;
        size_t len_to_fel = 0;


        for(itter = mem_list.begin(), i = 0; itter != mem_list.lend(); ++itter, i++)
        {
            
            if((ff_table.find(itter->second))->second.start_point == nullptr)
            {
                return itter->second;
            }
            else if((ff_table.find(itter->second))->second.start_point->elem > len_to_fel)
            {
                len_to_fel = (ff_table.find(itter->second))->second.start_point->elem ;
                far_elem = itter->second;
            }
        }

        return far_elem;

    }

    void cache_tester_t::start_IDEAL ()
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
                    
                    mem_list.erase(find_m_far_ff());
                    mem_list.push_tf(*itter, *itter);
                    
                    
                }
                ideal_mising_++;
            }

            
            if(ff_table.find(*itter)->second.start_point != nullptr)
            {
                ff_table.find(*itter)->second.pop();
            }

        }

    }

    void cache_tester_t::cash_test(int cach_tester (int elelm))
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

    size_t cache_tester_t::return_id ()
    {
        return ideal_mising_;
    }

    size_t cache_tester_t::return_test ()
    {
        return test_mising_;
    }

}