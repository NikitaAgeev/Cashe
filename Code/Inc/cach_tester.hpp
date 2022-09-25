

namespace cache_tester
{   

    #ifndef TESTER
    #define TESTER
    
    class micro_line_nod
    {
        public:
            micro_line_nod* next;
            size_t elem;
            micro_line_nod* prev;

        public:
            micro_line_nod
            (size_t elem_, micro_line_nod* prev_): elem(elem_), prev(prev_) 
            {
                next = nullptr;
                if(prev != nullptr) prev->next = this;
            };
            ~micro_line_nod() 
            {
                if(prev != nullptr) prev->next = next;
                if(next != nullptr) next->prev = prev;
                elem = 0;
            };

            void add_node(int elem)
            {
                next = new micro_line_nod(elem, this);
            };


    };

    class micro_line_t
    {
        public:
            micro_line_nod* start_point;
        private:
            micro_line_nod* end_point;

        public:
            micro_line_t
            (size_t f_elem) {start_point = new micro_line_nod(f_elem, nullptr); end_point = start_point;};
            
            ~micro_line_t ()
            {
                auto buf = end_point;

                if(buf == nullptr)
                {
                    return;
                }

                end_point = nullptr;
                start_point = nullptr;

                while(buf->prev != nullptr)
                {

                    buf = buf->prev;
                    delete buf->next;

                }

                
            };
            

            void push (size_t elem)
            {
                micro_line_nod* buf = new micro_line_nod(elem, end_point);
                end_point = buf;
            };

            void pop ()
            {
                
                if(start_point == nullptr)
                {
                    return;
                }
                if(start_point->next != nullptr) 
                {
                    start_point = start_point->next;
                    delete start_point->prev;
                }
                else if (start_point != nullptr)
                {
                    end_point = nullptr;
                    delete start_point;
                    start_point = nullptr;
                }
            };

            void print()
            {
                for(micro_line_nod*  it = start_point; it != nullptr; it = it->next)
                {
                    printf("%lu|", it->elem);
                }
                printf("\n");
            }

    };
    

    class cache_tester_t
    {
        private:
            //test_info
            std::list<int> test;
            size_t sz_;
            
            //test_result_info
            size_t ideal_mising_ ;
            size_t test_mising_ ;
            
            //ideal_test_elem_data
            std::unordered_map <int, micro_line_t> ff_table;

        
            qline::qline_t <int, size_t> mem_list;
        
        public:
            cache_tester_t (size_t sz): sz_(sz) {ideal_mising_ = 0; test_mising_ = 0;};

        
            void test_add(int elem);

        private:
            void add_elelm(int elem, size_t iter);

            void test_ff_ininter ();

            int find_m_far_ff ();


        public:
            void start_IDEAL ();

            void cash_test(int cach_tester (int elelm));

            size_t return_id ();

            size_t return_test ();


    };

    #endif
}