
namespace ARC_cache
{
#ifndef ARC_CACHE
#define ARC_CACHE
// Enum comparison keys
    enum LIST_CODE
    {    
        RL = 1,         //most_recency data
        FL = 1 << 1,    //most_frequency data
        RLG = 1 << 2,   //most_recency delated data
        FLG = 1 << 3,   //most_frequency delated data
        MISS = 1 << 4   //cache miss
    };

// The class of the cache
    template <typename T = int, typename KeyT =  ssize_t> class ARC_cache_t
    {
    
        private:
            using ListIt = typename std::list<T>::iterator;
            using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;
        
        // The cache memory is needed separately from the control lists to ensure uniformity of their typing (otherwise the code becomes more complicated)
            qline::qline_t <T, KeyT> data_list;

        // Control Lists 
            // Elements contained in memory
            qline::qline_t <int, KeyT> T1;
            qline::qline_t <int, KeyT> T2;
            // Trash bin with deleted item data
            qline::qline_t <int, KeyT> B1;
            qline::qline_t <int, KeyT> B2;

        // Cache characteristics
            ssize_t sz_;
            ssize_t p_ = 0;

        public:

            ARC_cache_t (ssize_t sz) : sz_(sz) {}

        // Checking for the presence of an element in control structures and coordinating the position
        private:
            int cache_find (const KeyT key)
            {
                
                if (T1.find(key) != T1.end()) return RL;

                if (T2.find(key) != T2.end()) return FL;

                if (B1.find(key) != B1.end()) return RLG;

                if (B2.find(key) != B2.end()) return FLG;

                return MISS;
            }

        //replacer : removes unnecessary data from memory and transfers their controls to the trash
            void replace_p (const int hit_status)
            {
                if ((T1.size() >= 1) && (((hit_status & FLG) && (T1.size() == p_)) || (T1.size() > p_)))
                {
                    data_list.erase(T1.back().first);
                    B1.push_top_from_list(T1.back().first, &T1);
                }
                else
                {
                    data_list.erase(T2.back().first);
                    B2.push_top_from_list(T2.back().first, &T2);
                }
            }

            // trsh TX (BX) -> read elem -> top T2
            void move_from_trash(const int hit_status, const KeyT key, T get_elem_to_mem(const KeyT key))
            {
                switch (hit_status)
                { 
                    //case 2 : trash T1 (B1) hit => read elem -> top T2 
                    case RLG:
                        p_ = std::min(sz_, p_ + std::max( (ssize_t)(B2.size()/B1.size()) , (ssize_t)1)); //recalculation p
                        replace_p(hit_status);

                        T2.push_top_from_list(key, &B1); // B1->elem->top T2
                        data_list.push_tf(key, get_elem_to_mem(key));// elem->mem
                    
                    break;

                    //case 3 : trash T2 (B2) hit => read elem -> top T2
                    case FLG:
                        ssize_t dp = std::max( (ssize_t)(B1.size()/B2.size()) , (ssize_t)1); //recalculation p
                        p_ = std::max((ssize_t)0, p_ - dp);
                        replace_p(hit_status);
                        
                        T2.push_top_from_list(key, &B2); // B2 -> elem -> top T2
                        data_list.push_tf(key, get_elem_to_mem(key)); // elem->mem

                    break;
                }
            }

            //read & add new elem to cach
            void read_new_elem (const int hit_status, const KeyT key, T get_elem_to_mem(const KeyT key))
            {
                //case4.1 : long list T1 + B1 (> sz_)
                if ((T1.size() + B1.size()) == sz_)
                {
                    if (T1.size() < sz_)
                    {
                        B1.pop_be();
                        replace_p(hit_status);
                    }
                    else
                    {
                        data_list.erase(T1.back().first);
                        T1.pop_be();
                    } 
                }//case 4.2 : long list T1 + B1 + T2 + B2 (>=sz_) 
                else if(((T1.size() + B1.size()) < sz_) && ((T1.size() + B1.size() + T2.size() + B2.size()) >= sz_))
                {
                    if((T1.size() + B1.size() + T2.size() + B2.size()) == 2*sz_)
                    {
                        B2.pop_be();
                    }
                    replace_p(hit_status);
                }

                data_list.push_tf(key, get_elem_to_mem(key)); //read elem
                T1.push_tf(key, 0); //elem -> top T1
            }

        public:

            // Update cache + return 1 if a new element was added to memory
            int update_cache(const KeyT key, T get_elem_to_mem(const KeyT key)) 
            {
                int hit_status = cache_find(key);
                
                switch (hit_status)
                { 
                    //case_1 : cache memory hit => move elem -> top T2
                    case RL:
                    case FL:
                        if(hit_status == RL) T2.push_top_from_list(key, &T1); //if T1 => T1->elem->top T2
                        else T2.app_to_top(key); //if T2 => T2->elem->top T2
                    break;

                    //case 2&3 : trsh TX (BX) hit => read elem -> top T2 
                    case RLG:
                    case FLG:
                        move_from_trash(hit_status, key, get_elem_to_mem);
                        return 1;
                    break;

                    //case 4 : cach full miss => read elem -> top T1
                    default:
                        read_new_elem (hit_status, key, get_elem_to_mem);
                        return 1;
                    break;
                }

                return 0;

            }

            // Retrieves an element from the cache and, if necessary, adds it to the cache
            T get_elem( const KeyT key, T get_elem_to_mem(const KeyT key))
            {
                this->update_cache(key, get_elem_to_mem);
                return data_list.find(key)->second;
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

        void change_sz(const ssize_t size)
        {
            sz_ = size;
        }
    };

#endif
}