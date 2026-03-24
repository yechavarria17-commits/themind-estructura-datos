#ifndef LIVES_H
#define LIVES_H

class Lives {
public:
    int count;

    Lives() : count(0) {}
    explicit Lives(int c) : count(c) {}

    void lose() {
        if (count > 0) count--;
    }

    bool isDepleted() const {
        return count <= 0;
    }

    void gain() {
        if (count < 5) count++;  // Maximo 5 vidas segun reglas oficiales
    }
};

#endif
