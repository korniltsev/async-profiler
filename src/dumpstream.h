#ifndef STD_H
#define STD_H

#include <stdlib.h>
#include <cstring>
#include <string>

class dumpstream {
public:
    explicit dumpstream(FILE* f) : _f(f), _err(0) {
        if (!_f) {
            _err = 1;
        }
    }

    ~dumpstream() {
    }

    dumpstream& operator<<(const char* s) {
        return write(s, static_cast<long>(strlen(s)));
    }

    dumpstream& operator<<(const char c) {
        return write(&c, 1);
    }

    dumpstream& operator<<(const std::string& s) {
        return write(s.c_str(), static_cast<long>(s.size()));
    }

    dumpstream& operator<<(const long n) {
        char buf[0x20];
        return write(buf, snprintf(buf, sizeof(buf), "%ld", n));
    }

    dumpstream& operator<<(const int n) {
        char buf[0x10];
        return write(buf, snprintf(buf, sizeof(buf), "%d", n));
    }

    dumpstream& write(const char* data, const long sz) {
        if (!_f || static_cast<long>(fwrite(data, 1, sz, _f)) != sz) {
            _err = 1;
        }
        return *this;
    }

    void close() {
        if (!_f) {
            return;
        }
        if (fclose(_f) != 0) {
            _err = 1;
        }
        _f = NULL;
    }

    void flush() {
        if (!_f) {
            return;
        }
        if (fflush(_f) != 0) {
            _err = 1;
        }
    }

    bool good() const {
        return _err == 0;
    }

private:
    FILE* _f;
    size_t _err;
};

class memdumpstream {
public:
    explicit memdumpstream(): _f(open_memstream(&_buf, &_size)), _stream(_f) {
        if (!_f) {
            _buf = NULL;
            _size = 0;
        }
    }

    ~memdumpstream() {
        if (_f) {
            fclose(_f);
        }
        free(_buf);
    }

    dumpstream& stream() {
        return _stream;
    }

    char* buf() const {
        return _buf;
    }

    size_t size() const {
        return _size;
    }

private:
    FILE *_f;
    dumpstream _stream;
    char* _buf;
    size_t _size;
};


#endif //STD_H
