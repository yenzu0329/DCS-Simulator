#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "simul.h"
#include "seqin.h"

class Waveform : public Simul
{
public:
    explicit Waveform(Component *,QWidget *parent = nullptr);
    void make_waveform(Component *);
    void draw(int, int);

private:
    QString waveform;
};

#endif // WAVEFORM_H
