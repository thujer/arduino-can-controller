
#ifndef countdown_h_
#define countdown_h_


class Countdown {

private:
    unsigned long countdownDelay = 0;
    unsigned long countdownOnStart = 0;

public:
    void set(unsigned long ms);
    unsigned long get();
    bool timeout();
};

#endif
