

namespace cach_tester
{   

    #ifndef TESTER
    #define TESTER
    class cach_tester_t
    {
        private:
            //test_info
            std::list<int> test;
            size_t sz_;
            
            //test_result_info
            size_t ideal_mising_ ;
            size_t test_mising_ ;
            
            //ideal_test_elem_data
            std::unordered_map <int, std::list<size_t>> ff_table;

        
            qline::qline_t <int, size_t> mem_list;
        
        public:
            cach_tester_t (size_t sz): sz_(sz) {ideal_mising_ = 0; test_mising_ = 0;};

        
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