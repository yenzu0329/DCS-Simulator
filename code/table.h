#ifndef TABLE_H
#define TABLE_H

#include "simul.h"
#include "combin.h"

class Table : public Simul
{
    Q_OBJECT
public:
    explicit Table(Component *, QWidget *parent = nullptr);
    ~Table();
    void make_table(Component *);
    void draw();
    Value* d2b(int dec);

private:
    QSet<CombIn *> variables;
    int row_num;
    int column_num;
    Value **table;
};

#endif // TABLE_H
