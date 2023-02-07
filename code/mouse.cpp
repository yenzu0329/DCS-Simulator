#include "mouse.h"

Mouse mouse;

inline Mouse::Mouse()
{
    mode = NONE;
    type = AND;
    radio = 1;
    node = nullptr;
    wire = nullptr;
    is_seq = false;
    is_simul = false;
    temp_select = false;
}
void Mouse::setMode(Mode m)
{
    if(mode != m)   //change mode
    {
        mode = m;
        changeAction->trigger();
    }
    changeBackgroundCursor();
}
void    Mouse::setType(Type t)      {type = t;}
void    Mouse::setRadio(double r)   {radio *= r;}
void    Mouse::setNode(Node *n)     {node = n;}
void    Mouse::setWire(Wire *w)     {wire = w;}
void    Mouse::setSimul(bool s)     {is_simul = s;}
void    Mouse::setSeq(bool s)       {is_seq = s;}
void    Mouse::setBackground(QGraphicsItem *bg) {background = bg;}

bool    Mouse::isSimul()    {return is_simul;}
bool    Mouse::isSeq()      {return is_seq;}
int     Mouse::getMode()    {return mode;}
int     Mouse::getType()    {return type;}
double  Mouse::getRadio()   {return radio;}
Node*   Mouse::getNode()    {return node;}
Wire*   Mouse::getWire()    {return wire;}

QString Mouse::typeToString(Type t)
{
    switch (t)
    {
    case AND:    return "AND";
    case OR:     return "OR";
    case NAND:   return "NAND";
    case NOR:    return "NOR";
    case XOR:    return "XOR";
    case NOT:    return "NOT";
    case DFF:    return "DFF";
    case RSFF:   return "RSFF";
    case JKFF:   return "JKFF";
    case TFF:    return "TFF";
    case IN:     return "IN";
    case OUT:    return "OUT";
    case LABEL:  return "LABEL";
    }
}

int Mouse::stringToType(QString str)
{
    if(str == "AND")    return AND;
    if(str == "OR")     return OR;
    if(str == "NAND")   return NAND;
    if(str == "NOR")    return NOR;
    if(str == "XOR")    return XOR;
    if(str == "NOT")    return NOT;
    if(str == "DFF")    return DFF;
    if(str == "RSFF")   return RSFF;
    if(str == "JKFF")   return JKFF;
    if(str == "TFF")    return TFF;
    if(str == "IN")     return IN;
    if(str == "OUT")    return OUT;
    if(str == "LABEL")  return LABEL;
    return -1;
}

void Mouse::changeBackgroundCursor()
{
    if(mode == ADD)
    {
        if(type == LABEL)
        {
            background->setCursor(Qt::IBeamCursor);
            return;
        }
        else if(type == IN || type == OUT)
        {
            QCursor cursor;
            QPixmap pixmap;
            if(type == IN)
                pixmap.load(":/components/label/in.png");
            else
                pixmap.load(":/components/label/out.png");
            QPixmap scaleMap = pixmap.scaledToHeight(32*radio);
            cursor = QCursor(scaleMap);
            background->setCursor(cursor);
        }
        else
        {
            QCursor cursor;
            QPixmap pixmap;
            if(type < NOT)
                pixmap.load(":/components/"+ typeToString(type).toLower() +"/2.png");
            else if(type == NOT)
                pixmap.load(":/components/"+ typeToString(type).toLower() +"/1.png");
            else if(type <= JKFF)
                pixmap.load(":/components/ff/"+ typeToString(type).toLower() +".png");
            QSize size(pixmap.width()*0.1*radio, pixmap.height()*0.1*radio); // 設定cursor大小
            QPixmap scaleMap = pixmap.scaled(size, Qt::KeepAspectRatio);
            cursor = QCursor(scaleMap);
            background->setCursor(cursor);
        }
    }
    else if(mode == WIRE || mode == SELECT)
        background->setCursor(Qt::CrossCursor);
    else
        background->setCursor(Qt::ArrowCursor);
}
