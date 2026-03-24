#ifndef STARS_H
#define STARS_H

class Stars {
public:
    int count;

    Stars() : count(0) {}
    explicit Stars(int c) : count(c) {}

    bool use() {
        if (count > 0) {
            count--;
            return true;
        }
        return false;
    }

    bool hasAny() const {
        return count > 0;
    }

    void gain() {
        if (count < 3) count++;  // Maximo 3 estrellas segun reglas oficiales
    }
};

#endif
