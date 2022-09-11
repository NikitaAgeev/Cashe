#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <stdio.h>

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

    void lookup_update(KeyT key, T slow_get_page(KeyT key)) 
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
        }

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

int slow_get_page(int key)
{
    return (int)(key);
}


int main()
{

    
    ARC_cach::ARS_cach_t<int,int> c(6);
    int key, i = 0;
    while(1)
    {
        scanf("%d", &key);
        c.lookup_update(key, slow_get_page);
        c.debug_print();    
    }
    c.lookup_update(3, slow_get_page);
    c.debug_print();
    
}