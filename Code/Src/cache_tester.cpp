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
#include <cache_tester.hpp>

namespace cache_tester
{
    
    void cache_tester_t::test_add(const int elem)
    {
        test.push_back({elem, 0});
        test_len_++;
    }

    void cache_tester_t::init_test ()
    {
        auto itter = test.rbegin();
        
        std::unordered_map<int, ssize_t> elems;

        for(ssize_t i = test_len_ ; itter != test.rend(); itter++, i--)
        {
            if(elems.find(itter->first) == elems.end())
            {
                elems[itter->first] = i;
            }
            else
            {
                itter->second = elems[itter->first];
                elems[itter->first] = i;
            }
        } 
    }

    int cache_tester_t::find_m_far()
    {
        
        auto itter = mem_list.begin();
        ssize_t i = 0;
        int far_elem = -1;
        ssize_t len_to_fel = 0;


        for(itter = mem_list.begin(), i = 0; itter != mem_list.lend(); ++itter, i++)
        {
            
            if(itter->second == 0)
            {
                return itter->first;
            }
            else if(itter->second > len_to_fel)
            {
                len_to_fel = itter->second;
                far_elem = itter->first;
            }
        }

        return far_elem;

    }

    void cache_tester_t::start_IDEAL ()
    {
        this->init_test();
        
        auto itter = test.begin();
        ssize_t i = 0;

        ideal_mising_ = 0;

        for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
        {
            
            if(mem_list.find(itter->first) == mem_list.end())
            {
                if(mem_list.size() < sz_)
                {
                    
                    mem_list.push_tf(itter->first, itter->second);
                    
                }
                else        
                {
                    
                    mem_list.push_tf(itter->first, itter->second);
                    mem_list.erase(find_m_far());
                    
                }
                ideal_mising_++;
            }
            else
            {
                mem_list.find(itter->first)->second->second = itter->second;
            }

        }

    }

    void cache_tester_t::cash_test(int cache_tester (const int elelm))
    {
        auto itter = test.begin();
        ssize_t i = 0;

        test_mising_ = 0;

        for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
        {
            
            if(cache_tester(itter->first))
            {
                test_mising_++;
            }

        }
    }

    ssize_t cache_tester_t::return_id ()
    {
        return ideal_mising_;
    }

    ssize_t cache_tester_t::return_test ()
    {
        return test_mising_;
    }

}