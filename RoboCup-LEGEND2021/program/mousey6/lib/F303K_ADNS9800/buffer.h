#ifndef buffer_hpp
#define buffer_hpp

#define BUFFER_SIZE 10

class Buffer {
    
public:
    Buffer();
    void set(unsigned int where, int value);
    void setWithIntegral(unsigned int where, int value);
    void reset(int where);
    int get(unsigned int where);
    
};

#endif /* buffer_hpp */
