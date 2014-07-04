#ifndef SetCover_hh_include
#define SetCover_hh_include

#include <set>
#include <map>
#include <vector>
#include <iostream>

#include "Defines.hh"

class SetCover {
public:
    /*
     * call run() once size() > SIZE_LIMIT
     * this can make sure its maximum memory usage is less than 2G
     * to avoid potential out-of-memory problem
     */
    enum { SIZE_LIMIT = 200 * (1 << 20) };

    ~SetCover();

    void add(size_t set_id, size_t elem_id);

    size_t size() const;

    void run();

    const std::set<size_t>& result() const { return min_sets; }

private:

#ifdef USE_SET_TO_REPRESENT_ELEMS
    typedef std::set<size_t> Elems;
#else
    /*
     * we use sorted vector but not set to represent set of elems for performance consideration
     * Performance data from test case set_cover_perf:
     *                100 sets * (1M elems/set)          20 sets * (1M elems/set)
     *     vector      1.5G, 33sec                         0.3G, 2sec
     *     set         >5G, >10minutes                     1.9G, 46sec
     */
    typedef std::vector<size_t> Elems;
#endif

    typedef std::map<size_t, Elems*> Id2ElemsMap;

    struct SetSet {
        struct Set {
            size_t id;
            Elems *elems;
            explicit Set(size_t set_id) : id(set_id), elems(0) {}
            Set(size_t set_id, Elems* e) : id(set_id), elems(e) {}

            size_t size() const { return elems ? elems->size() : 0; };

            struct CmpFunc {
                bool operator()(const Set &l, const Set &r) const {
                    return l.size() == r.size() ? l.id < r.id : l.size() > r.size();
                }
            };
        };
        typedef std::set<Set, Set::CmpFunc> container;
        container sets;

        void add(size_t set_id, Elems* elems) { sets.insert(Set(set_id, elems)); }

        bool empty() const { return sets.empty(); }

        size_t unplug();

        void d() const;
    };

    static void addElem(Elems& elems, size_t elem_id);
    static void unplugElems(Elems& elems, const Elems& ano);

    void release();

    Id2ElemsMap mId2Elems;
    std::set<size_t> min_sets;
};

#endif // SetCover_hh_include
