#include <algorithm>
#include <cassert>

#include "SetCover.hh"

SetCover::~SetCover() {
    release();
}

void SetCover::release() {
    foreach(i, mId2Elems) {
        delete i->second;
    }
    mId2Elems.clear();
}

void SetCover::add(size_t set_id, size_t elem_id) {
    Id2ElemsMap::iterator pos = mId2Elems.find(set_id);
    if (pos != mId2Elems.end()) {
        addElem(*pos->second, elem_id);
    } else {
        Elems* elems = new Elems();
        addElem(*elems, elem_id);
        mId2Elems.insert(std::make_pair(set_id, elems));
    }
}

void SetCover::run() {
    // setup
    foreach(i, min_sets) {
        Id2ElemsMap::iterator pos = mId2Elems.find(*i);
        foreach(it, mId2Elems) {
            unplugElems(*it->second, *pos->second);
        }
    }
    SetSet sets;
    foreach(i, mId2Elems) {
        if (!i->second->empty()) {
            sets.add(i->first, i->second);
        }
    }

    // find minimum cover sets
    while(!sets.empty()) {
        min_sets.insert(sets.unplug());
    }

    release();
}

size_t SetCover::size() const {
    size_t sz = 0;
    foreach(i, mId2Elems) {
        sz  += i->second->size();
    }
    return sz;
}

void SetCover::addElem(Elems& elems, size_t elem_id) {
#ifdef USE_SET_TO_REPRESENT_ELEMS
    elems.insert(elem_id);
#else
    Elems::reverse_iterator it = elems.rbegin();
    while(it != elems.rend()) {
        if (elem_id > *it) {
            elems.insert(it.base(), elem_id);
            return;
        } else if (elem_id == *it) {
            return;
        } else {
            ++it;
        }
    }
    elems.insert(it.base(), elem_id);
#endif
}

void SetCover::unplugElems(Elems& elems, const Elems& ano) {
#ifdef USE_SET_TO_REPRESENT_ELEMS
    foreach(i, ano) {
        elems.erase(*i);
    }
#else
    const size_t INVALID_ID = static_cast<size_t>(-1);
    for(size_t i = 0, j = 0; i < elems.size() && j < ano.size();) {
        size_t& v0 = elems[i];
        const size_t& v1 = ano[j];
        if (v0 < v1) {
            ++i;
        } else if (v0 > v1) {
            ++j;
        } else {
            v0 = INVALID_ID;
            ++i; ++j;
        }
    }
    elems.erase(remove(elems.begin(), elems.end(), INVALID_ID), elems.end());
#endif
}

size_t SetCover::SetSet::unplug() {
    assert(!empty());
//    d(); // uncomment this line for debugging
    Set target = *sets.begin();
    sets.erase(sets.begin());
    container ss;
    ss.swap(sets);
    for (container::iterator it = ss.begin(); it != ss.end();) {
        Set set = *it;
        ss.erase(it++);
        SetCover::unplugElems(*set.elems, *target.elems);
        if (!set.elems->empty()) {
            sets.insert(set);
        }
    }
    return target.id;
}

// for debug only. 
// It is not put in "#ifndef NDBEBUG" just becaue I may use it in release mode as well.
void SetCover::SetSet::d() const {
    std::cerr << std::endl;
    foreach(i, sets) {
        const Set& set = *i;
        std::cerr << set.id << ":";
        foreach(i, *set.elems) {
            std::cerr << " " << *i;
        }
        std::cerr << std::endl;
    }
}
