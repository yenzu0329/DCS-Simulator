#ifndef RSFF_H
#define RSFF_H

#include <flipflop.h>

class RSff: public FlipFlop
{
public:
    RSff();
    RSff(FlipFlop *);
    RSff(QStringList);
    void compute();
    void init();
private:
    Value store_r;
    Value store_s;
};

#endif // RSFF_H
