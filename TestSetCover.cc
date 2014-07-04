#include "SetCover.hh"

#include <sstream>
#include <iterator>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace TestUtility {
void split(vector<string>& res, const string& s, char delimiter) {
    size_t i = 0;
    while (i < s.size()) {
        size_t p = s.find_first_of(delimiter, i);
        if (p != string::npos) {
            if (p != i) {
                res.push_back(s.substr(i, p - i));
            }
            i = p + 1;
        }
        else {
            res.push_back(s.substr(i));
            break;
        }
    }
}

} // end of namespace TestUtility

static bool test_SetCoverBasic() {
    const size_t SET_CNT = 2;
    string tests[SET_CNT] = {
"1 2 3\n"
"2 4\n"
"3 4\n"
"4 5",

"10 10 5\n"
"5 7 8 9 10\n"
"5 7\n"
"1 3 6 10\n"
"4 6 7 9 10\n"
"1 2 8 10"
};
    for (size_t n = 0; n < SET_CNT; ++n) {
        SetCover sc;
        vector<string> res;
        TestUtility::split(res, tests[n], '\n');
        size_t set_id = 0;
        foreach(i, res) {
            const string &s = *i;
            istringstream iss(s);
            vector<int> list((istream_iterator<int>(iss)), istream_iterator<int>());
            foreach(i, list) {
                sc.add(set_id, *i);
            }
            ++set_id;
        }
        sc.run();

        cout << endl;
        cout << tests[n] << endl;
        cout << "min_sets:";
        foreach(i, sc.result()) {
            cout << " " << *i;
        }
        cout << endl;
    }
    return true;
}

// for performance test only
static bool test_SetCoverPerf() {
    const size_t SET_CNT = 100;
    const size_t MAX_SET_SIZE = 1 << 20;
    cout << "Testing for performance: SET_CNT(" << SET_CNT << "), MAX_SET_SIZE(" << MAX_SET_SIZE << ") " << endl;
    SetCover sc;
    for(size_t set_id = 0; set_id < SET_CNT; ++set_id) {
        size_t set_size = MAX_SET_SIZE; // abs(rand()) % MAX_SET_SIZE;
        cout << "// Beging of adding set with id(" << set_id << ")" << endl;
        for (size_t i = 0, elem_id = 0; i < set_size; ++i) {
            sc.add(set_id, elem_id);
            if (set_id %2) {
                elem_id += 1;
            } else {
                elem_id += abs(rand()) % 1000  + 1;
            }
        }
        cout << "// End of adding set with id(" << set_id << ")" << endl;
        if (sc.size() > SetCover::SIZE_LIMIT) {
            const size_t size = sc.size();
            cout << "Start Run with total elems num == " << size << endl;
            sc.run();
            cout << "End of Running with total elems num == " << size << endl;
        }
    }
    const size_t size = sc.size();
    cout << "Start Run with total elems num == " << size << endl;
    sc.run();
    cout << "End of Running with total elems num == " << size << endl;
    return true;
}

static void show_help_msg() {
    cout << "test app for SetCover solver" << endl;
    cout << "Usage: test_app [<option> ...]" << endl;
    cout << "Options: " << endl;
    cout << "-h|-help    print this help message" << endl;
    cout << "-func       run functionality test" << endl;
    cout << "-perf       run the performance test" << endl;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        show_help_msg();
        return 0;
    }

    enum {FUNC_TEST = 0, PERF_TEST = 1, TEST_NUM };

    bool run_test[TEST_NUM] = {false, false };

    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        if (!strcmp(arg, "-h") || !strcmp(arg, "-help")) {
            show_help_msg();
            return 0;
        } else if (!strcmp(arg, "-func")) {
            run_test[FUNC_TEST] = true;
        } else if (!strcmp(arg, "-perf")) {
            run_test[PERF_TEST] = true;
        } else {
            cerr << "Warning: unknown option '" << arg << "'" << endl;
        }
    }
    if (run_test[FUNC_TEST]) {
        test_SetCoverBasic();
    }
    if (run_test[PERF_TEST]) {
        test_SetCoverPerf();
    }

    return 0;}
