#ifndef worldh
#define worldh

typedef struct chunk {

} chunk;

chunk *chunks;

// load a chunk
void loadchunk(int x, int y) {

}

// request a chunk to be loaded (loadchunk(x,y) but threaded)
void loadchunkreq(int x, int y) {

}

// request a radius of chunks to be loaded
void loadchunkrreq(int x, int y, int r) {

}

#endif