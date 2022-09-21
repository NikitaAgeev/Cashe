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




namespace ARC_cach
{

// Hesh_list для паралельнйо работы
    template <typename T, typename KeyT> struct list_hash_t
    {
        
        // Обявление полей
        std::list<std::pair<T, KeyT>> list;

        using ListIt = typename std::list<std::pair<T, KeyT>>::iterator;
        using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;

        std::unordered_map<KeyT, ListIt> hash;

        // Создание методов

        //Элементарные методы
        void push_tf (KeyT key, T data)
        {
            list.push_front({data, key});
            hash.insert({key, list.begin()});
        }

        void pop_be ()
        {
            if(list.size() > 0)
            {
                KeyT key = list.back().second;
                list.erase(hash.find(key)->second);
                hash.erase(key); 
            }    
        }

        HashIt find (KeyT key)
        {
            return hash.find(key);
        }       

        HashIt end (void)
        {
            return hash.end();
        }

        size_t size()
        {
            return list.size();
        }

        void erase(KeyT key)
        {
            list.erase((hash.find(key))->second);
            hash.erase(key);
        }    

        std::pair<T, KeyT> back()
        {
            auto elem = list.back();
            return elem;
        }

        //Полезные методы

        void app_to_top (KeyT key)
        {
            if( key != (list.front().second))
            {
                HashIt buf = this->find(key);
                this->erase (key);
                this->push_tf (key, (buf->second)->second);
            }
        }

        void push_top_from_list (KeyT key, list_hash_t* from_list)
        {
            this->push_tf(key, from_list->find(key)->second->first);
            from_list->erase(key);
        }

        //Дебаг, методы
        void list_print()
        {
            ListIt iter = this->list.begin();
            for(size_t i = 0; i < this->size(); i++, ++iter)
            {
                std::cout << " " << iter->second << " ";
            }
        }

    };

// Enum ключей сравнения
    enum LIST_CODE
    {    
        RL = 1,         //most_recency data
        FL = 1 << 1,    //most_frequency data
        RLG = 1 << 2,   //most_recency delated data
        FLG = 1 << 3,   //most_frequency delated data
        MISS = 1 << 4   //cach miss
    };

//Структура самого кеша
    template <typename T = int, typename KeyT =  size_t> struct ARS_cach_t
    {
        
    //data elements
        list_hash_t <T, KeyT> data_list;
        using ListIt = typename std::list<T>::iterator;
        using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;

        list_hash_t <uint8_t, KeyT> T1;
        list_hash_t <uint8_t, KeyT> T2;
        list_hash_t <uint8_t, KeyT> B1;
        list_hash_t <uint8_t, KeyT> B2;

        size_t sz_;
        size_t p_;

        ARS_cach_t (size_t sz) : sz_(sz) {p_ = 0;}

        
    //hesh cheaker
        uint8_t cach_find (KeyT key)
        {
            
            if (T1.find(key) != T1.end()) return RL;

            if (T2.find(key) != T2.end()) return FL;

            if (B1.find(key) != B1.end()) return RLG;

            if (B2.find(key) != B2.end()) return FLG;

            return MISS;
        }

    //replaser
    void replase (uint8_t hit_status, KeyT hit_key)
    {
        if ((T1.size() >= 1) && ((hit_status & FLG) && (T1.size() == p_) || (T1.size() > p_)))
        {
            data_list.erase(T1.back().second);
            B1.push_top_from_list(T1.back().second, &T1);
        }
        else
        {
            data_list.erase(T2.back().second);
            B2.push_top_from_list(T2.back().second, &T2);
        }
    }

    int lookup_update(KeyT key, T slow_get_page(KeyT key)) 
    {
        uint8_t hit_status = cach_find(key);

        //case_1: hit on the cach
        if (hit_status & (RL | FL))
        {
            if(hit_status == RL)
            {
                T2.push_top_from_list(key, &T1);
            }
            else
            {
                T2.app_to_top(key);
                
            }
            
        } //case 2 hit on B1
        else if (hit_status & RLG)
        {
            p_ = std::min(sz_, p_ + std::max( (size_t)(B2.size()/B1.size()) , (size_t)1));
            replase(hit_status, key);
            T2.push_top_from_list(key, &B1);

            data_list.push_tf(key, slow_get_page(key));
            return 1;
        } //case 3
        else if (hit_status & FLG)
        {
            size_t dp = std::max( (size_t)(B1.size()/B2.size()) , (size_t)1);
            
            if(dp <= p_)
            {
                p_ = std::max((size_t)0, p_ - dp);
            }

            replase(hit_status, key);
            T2.push_top_from_list(key, &B2);

            data_list.push_tf(key, slow_get_page(key));
            return 1;
        } //case 4
        else
        {   
            //case4.1
            if ((T1.size() + B1.size()) == sz_)
            {
                if (T1.size() < sz_)
                {
                    B1.pop_be();
                    replase(hit_status, key);
                }
                else
                {
                    data_list.erase(T1.back().second);
                    T1.pop_be();
                } 
            }//case 4.2
            else if(((T1.size() + B1.size()) < sz_) && ((T1.size() + B1.size() + T2.size() + B2.size()) >= sz_))
            {
                if((T1.size() + B1.size() + T2.size() + B2.size()) == 2*sz_)
                {
                    B2.pop_be();
                }
                replase(hit_status, key);
            }

            data_list.push_tf(key, slow_get_page(key));
            T1.push_tf(key, 0); 

            return 1;  
        }

        return 0;

    }

    void debug_print()
        {
            std::cout << "P: " << p_ << " Sz: " << sz_ << "\n";
            std::cout << "ARC1: ";
            T1.list_print();
            std::cout << " | ";
            B1.list_print();
            std::cout << "\n";
            std::cout << "ARC2: ";
            T2.list_print();
            std::cout << " | ";
            B2.list_print();
            std::cout << "\n";
        }

    };


};



namespace cach_tester
{
    struct cach_tester_t
    {
        
        //test_info
        std::list<int> test;
        size_t sz_;
        int(*cach_tester)(int elem);
        
        //test_result_info
        size_t ideal_mising_ ;
        size_t test_mising_ ;
        
        //ideal_test_elem_data
        std::unordered_map <int, std::list<size_t>> ff_table;

        ARC_cach::list_hash_t <int, size_t> mem_list;
        
        cach_tester_t (size_t sz, int(cach_tester)(int elem)): sz_(sz), cach_tester(cach_tester) {ideal_mising_ = 0; test_mising_ = 0;};

        
        void test_add(int elem)
        {
            test.push_back(elem);
        }

        void add_elelm(int elem, size_t iter)
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

        void test_ff_ininter ()
        {
            
            auto itter = test.begin();
            size_t i = 0;

            for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
            {
                add_elelm(*itter, i);
            }

        }

        int find_m_far_ff ()
        {
            
            auto itter = mem_list.list.begin();
            size_t i = 0;
            int far_elem = -1;
            size_t len_to_fel = 0;


            for(itter = mem_list.list.begin(), i = 0; itter != mem_list.list.end(); ++itter, i++)
            {
                auto elem_pos_mas = (ff_table.find(itter->first))->second;
                
                if(elem_pos_mas.begin() == elem_pos_mas.end())
                {
                    return itter->first;
                }
                else if(*(elem_pos_mas.begin()) > len_to_fel)
                {
                    len_to_fel = *(elem_pos_mas.begin());
                    far_elem = itter->first;
                }
            }

            return far_elem;

        }

        void start_IDEAL ()
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

        void cash_test()
        {
            auto itter = test.begin();
            size_t i = 0;

            test_mising_ = 0;

            for(itter = test.begin(), i = 0; itter != test.end(); ++itter, i++)
            {
                
                if((*cach_tester)(*itter))
                {
                    test_mising_++;
                }

            }
        }


    };
}



ARC_cach::ARS_cach_t<int,int> c (0);

int get_page(int key)
{
    return key;
}

int test_looker(int elem)
{
    //c.debug_print();
    return c.lookup_update(elem, get_page);
}

int main()
{
    
    printf("Введите размер кеша: ");
    size_t cach_sz = 0;
    scanf("%lu", &cach_sz); 

    printf("Введите количество данных: ");
    size_t n_data = 0;
    scanf("%lu", &n_data);

    c.sz_ = cach_sz;
    
    size_t i = 0;

    cach_tester::cach_tester_t test (cach_sz, test_looker);

    int buf = 0;
    for(i = 0; i < n_data; i++)
    {
        scanf("%d", &buf);
        test.test_add(buf);
    }

    test.cash_test();
    test.start_IDEAL();

    printf("cach_miss: %lu\n", test.test_mising_);
    printf("ideal: %lu\n", test.ideal_mising_);

}