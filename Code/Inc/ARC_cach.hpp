
namespace ARC_cach
{
#ifndef ARC_CACH
#define ARC_CACH
// Enum ключей сравнения
    enum LIST_CODE
    {    
        RL = 1,         //most_recency data
        FL = 1 << 1,    //most_frequency data
        RLG = 1 << 2,   //most_recency delated data
        FLG = 1 << 3,   //most_frequency delated data
        MISS = 1 << 4   //cach miss
    };

//Класс самого кеша
    template <typename T = int, typename KeyT =  size_t> class ARC_cach_t
    {
    
        private:
            using ListIt = typename std::list<T>::iterator;
            using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;
        
        // Память кеша нужна отдельн от управляющих списко для обеспечения однородности их типизации (иначе код усложняется)
            qline::qline_t <T, KeyT> data_list;

        // Управляющие списки 
            // Элементы содержащиеся в памяти
            qline::qline_t <uint8_t, KeyT> T1;
            qline::qline_t <uint8_t, KeyT> T2;
            // Мусорка с данными о удалённых элементах
            qline::qline_t <uint8_t, KeyT> B1;
            qline::qline_t <uint8_t, KeyT> B2;

        // Характеристики кеша
            size_t sz_;
            size_t p_;

        public:

            ARC_cach_t (size_t sz) : sz_(sz) {p_ = 0;}

        //Проверка на присутствие элелемента в управляющих структурах и координация положения
        private:
            const uint8_t cach_find (const KeyT key)
            {
                
                if (T1.find(key) != T1.end()) return RL;

                if (T2.find(key) != T2.end()) return FL;

                if (B1.find(key) != B1.end()) return RLG;

                if (B2.find(key) != B2.end()) return FLG;

                return MISS;
            }

        //replacer : удаляет лишние данные из памяти и переносит их упраляющие элементы в мусорку
            void replace (const uint8_t hit_status)
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
        
        public:

            //Обновление кеша + возврат 1 если был добавлен новый элемент в память
            const int apdate_cach(const KeyT key, const T get_elem_to_mem(const KeyT key)) 
            {
                uint8_t hit_status = cach_find(key);

                //case_1 : попаданияе в память кеша
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
                    
                } //case 2 : попадание в мусорку B1
                else if (hit_status & RLG)
                {
                    p_ = std::min(sz_, p_ + std::max( (size_t)(B2.size()/B1.size()) , (size_t)1));
                    replace(hit_status);
                    T2.push_top_from_list(key, &B1);

                    data_list.push_tf(key, get_elem_to_mem(key));
                    return 1;
                } //case 3 : попадание в мусорку B2
                else if (hit_status & FLG)
                {
                    size_t dp = std::max( (size_t)(B1.size()/B2.size()) , (size_t)1);
                    
                    if(dp <= p_)
                    {
                        p_ = std::max((size_t)0, p_ - dp);
                    }

                    replace(hit_status);
                    T2.push_top_from_list(key, &B2);

                    data_list.push_tf(key, get_elem_to_mem(key));
                    return 1;
                } //case 4 : мимо всех списков
                else
                {   
                    //case4.1 : обработка случая длинного списка последних встречаемых элементов + его мусорки
                    if ((T1.size() + B1.size()) == sz_)
                    {
                        if (T1.size() < sz_)
                        {
                            B1.pop_be();
                            replace(hit_status);
                        }
                        else
                        {
                            data_list.erase(T1.back().first);
                            T1.pop_be();
                        } 
                    }//case 4.2 : Обработка слишком большого общего размера управляющих структур
                    else if(((T1.size() + B1.size()) < sz_) && ((T1.size() + B1.size() + T2.size() + B2.size()) >= sz_))
                    {
                        if((T1.size() + B1.size() + T2.size() + B2.size()) == 2*sz_)
                        {
                            B2.pop_be();
                        }
                        replace(hit_status);
                    }

                    // добавление нового элемента и его управляющего элемента
                    data_list.push_tf(key, get_elem_to_mem(key));
                    T1.push_tf(key, 0); 

                    return 1;  
                }

                return 0;

            }

            // Выдаёт элемент из кеша и, если надо, добавляет его в кеш
            const T get_elem( const KeyT key, const T get_elem_to_mem(const KeyT key))
            {
                this->apdate_cach(key, get_elem_to_mem);
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

        void change_sz(const size_t size)
        {
            sz_ = size;
        }
    };

#endif
}