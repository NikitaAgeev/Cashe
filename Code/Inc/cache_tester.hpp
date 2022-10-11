

namespace cache_tester
{   

    #ifndef TESTER
    #define TESTER

    class cache_tester_t
    {
        private:
            
            using test_elem = std::pair<int, size_t>;
            
            //test_info
            std::list<test_elem> test;
            ssize_t sz_;
            ssize_t test_len_ = 0;
            
            //test_result_info
            ssize_t ideal_mising_ = 0;
            ssize_t test_mising_ = 0;
            
        
            qline::qline_t <int, ssize_t> mem_list;
        
        public:
            cache_tester_t (ssize_t sz): sz_(sz) {};

        
            void test_add(const int elem);

        private:

            void init_test ();

            int find_m_far();

        public:
            void start_IDEAL ();

            void cash_test(int cache_tester (const int elelm));

            ssize_t return_id ();

            ssize_t return_test ();


    };

    #endif
}