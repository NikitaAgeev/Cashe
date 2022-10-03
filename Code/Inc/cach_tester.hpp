

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
            size_t sz_;
            size_t test_len_ = 0;
            
            //test_result_info
            size_t ideal_mising_ ;
            size_t test_mising_ ;
            
        
            qline::qline_t <int, size_t> mem_list;
        
        public:
            cache_tester_t (size_t sz): sz_(sz) {ideal_mising_ = 0; test_mising_ = 0;};

        
            void test_add(const int elem);

        private:

            void init_test ();

            const int find_m_far();

        public:
            void start_IDEAL ();

            void cash_test(const int cach_tester (const int elelm));

            size_t return_id ();

            size_t return_test ();


    };

    #endif
}