

namespace qline
{

#ifndef QLINE
#define QLINE
    /*!
    qline_t быстрая очередь, нужна для реализации очереди, которая включает в себя структуру list и скорость map 
    по поиску произвольных элементов, хотя по итогу данная структура даже избыточна в моём коде (с точки и зрения наличия у неё сохраняемого элемента)
    но она больше нужна как примитив общего плана 
    */
    template <typename T, typename KeyT> class qline_t
    {
        
        using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
        using HashIt = typename std::unordered_map<KeyT, ListIt>::iterator;

        // Пара списка и таблицы, работой которых будет урпавять класс
        private:

            std::list<std::pair<KeyT, T>> list;
            std::unordered_map<KeyT, ListIt> hash;

        // Методы по работе с list_map
        public:
            //Элементарные методы_прокладки, реализация которых есть в исходных элементах и они необходимы для необходимой работы

            ListIt begin ()
            {
                return list.begin();
            }

            ListIt lend()
            {
                return list.end();
            }

            void push_tf (KeyT key, T data)
            {
                list.push_front({key, data});
                hash.insert({key, list.begin()});
            }

            void pop_be ()
            {
                if(list.size() > 0)
                {
                    KeyT key = list.back().first;
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

            std::pair<KeyT, T> back()
            {
                auto elem = list.back();
                return elem;
            }

            //Усложнённые методы, нужные в реализации правильной работы

            void app_to_top (KeyT key)
            {
                if( key != (list.front().second))
                {
                    HashIt buf = this->find(key);
                    this->erase (key);
                    this->push_tf (key, (buf->second)->second);
                }
            }

            void push_top_from_list (KeyT key, qline_t* from_list)
            {
                this->push_tf(key, from_list->find(key)->second->first);
                from_list->erase(key);
            }

            //Дебаг, метод
            void list_print()
            {
                ListIt iter = this->list.begin();
                for(size_t i = 0; i < this->size(); i++, ++iter)
                {
                    std::cout << " " << iter->second << " ";
                }
            }

    };
#endif
}