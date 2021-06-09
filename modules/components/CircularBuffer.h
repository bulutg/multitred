//
// Created by Bulut Gözübüyük on 7.06.2021.
//

#ifndef SIYAH1_CIRCULARBUFFER_H
#define SIYAH1_CIRCULARBUFFER_H

#include <vector>
#include <stdexcept>

class CircularBuffer {

private:
    static const size_t EMPTY = ~0u;
    std::vector<char> data;
    size_t write_ptr = 0;               // index at which write_ptr insert will occur
    size_t read_ptr = EMPTY;           // index of write_ptr pop, or EMPTY marker
    int count;

public:
    explicit CircularBuffer(size_t size) : data(size) {
        count = 0;
    }

    virtual ~CircularBuffer() = default;

    bool empty() const {
        return read_ptr == EMPTY;
    }

    size_t size() const {
        if (empty()) return 0;
        if (write_ptr > read_ptr) return write_ptr - read_ptr;
        return data.size() - read_ptr + write_ptr;
    }

    int gsize(){
        return data.size();
    }

    void push(char val) {
        if (write_ptr == read_ptr) throw std::logic_error("Buffer overrun");

        data[write_ptr] = val;
        if (empty()) {
            read_ptr = write_ptr;
        }
        if (++write_ptr == data.size()) {
            write_ptr = 0;
        }
        this->count++;
    }

    char pop() {
        if (empty()) throw std::logic_error("Buffer underrun");

        const char popVal = data[read_ptr];

        if (++read_ptr == data.size()) {
            read_ptr = 0;
        }
        if (read_ptr == write_ptr) {
            read_ptr = EMPTY;
        }
        this->count--;

        return popVal;
    }

    int getCount() const {
        return count;
    }
};


#endif //SIYAH1_CIRCULARBUFFER_H
