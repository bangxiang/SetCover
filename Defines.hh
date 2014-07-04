#if 0 // gcc 4.6 reports error "taking address of temporary"
#define foreach(it, container) \
    if ( const typeof(container)* c = &container) for (typeof(c->begin()) it = c->begin(); it != c->end(); ++it)
#else
#define TOKENPASTE_HELPER(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_HELPER(x, y)
#define UNIQUENAME TOKENPASTE(var, __LINE__)
#define foreach(it, container) \
    const typeof(container) &UNIQUENAME = container; for (typeof(UNIQUENAME.begin()) it = UNIQUENAME.begin(); it != UNIQUENAME.end(); ++it)
#endif
