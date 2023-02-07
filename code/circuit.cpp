#include "circuit.h"

bool sign = false;
bool move = true;
QPointF prevScreenPos;

Circuit::Circuit(qreal x, qreal y, qreal width, qreal height): QGraphicsScene(x, y, width, height)
{
    curr_wire = nullptr;
    centerPos = QPointF(width/2, height/2);
    setItemIndexMethod(QGraphicsScene::NoIndex);

    //設定背景
    QPixmap  *bgPixmap  = new QPixmap(5000,5000);
    QPainter *bgPainter = new QPainter(bgPixmap);
    bgPainter->fillRect(0,0,5000,5000,Qt::white);
    bgItem = new QGraphicsPixmapItem(*bgPixmap);
    addItem(bgItem);
    mouse.setBackground(bgItem);

    selectBox = new SelectBox;
    addItem(selectBox);

    vLine = new QGraphicsLineItem(0, 0, 0, height);
    hLine = new QGraphicsLineItem(0, 0, width, 0);
    vLine->setPen(QPen(Qt::gray, 0.5, Qt::DashLine));
    hLine->setPen(QPen(Qt::gray, 0.5, Qt::DashLine));
    addItem(vLine);
    addItem(hLine);
    connect(mouse.changeAction, &QAction::triggered, this, &Circuit::changeMode);
}

void Circuit::init()
{
    mouse.setBackground(bgItem);
    connect(mouse.changeAction, &QAction::triggered, this, &Circuit::changeMode);
    mouse.changeBackgroundCursor();
    clearSelection();
    resetNames();
}

Circuit::~Circuit()
{
    delete bgItem;
    delete hLine;
    delete vLine;
    delete selectBox;
}

void Circuit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        if(mouse.getMode() == ADD)
        {
            history.addStep(this);
            Component *curr_component;
            switch (mouse.getType())
            {
            case AND:   curr_component = new And;       break;
            case OR:    curr_component = new Or;        break;
            case NAND:  curr_component = new Nand;      break;
            case NOR:   curr_component = new Nor;       break;
            case XOR:   curr_component = new Xor;       break;
            case NOT:   curr_component = new Not;       break;
            case DFF:   curr_component = new Dff;       break;
            case TFF:   curr_component = new Tff;       break;
            case JKFF:  curr_component = new JKff;      break;
            case RSFF:  curr_component = new RSff;      break;
            case LABEL: curr_component = new Label;     break;
            case OUT:   curr_component = new OutLabel;  break;
            case IN:
                if(mouse.isSeq())   curr_component = new SeqIn;
                else                curr_component = new CombIn;
                break;
            }
            curr_component->setPos(event->scenePos());
            addItem(curr_component);
            curr_component->setScene(this);

            if(mouse.getType() == LABEL || mouse.getType() == OUT || mouse.getType() == IN)
            {
                Label *label = dynamic_cast<Label *>(curr_component);
                label->setFocus();
                connect(label->text, &Text::lostFocus, this, &Circuit::textLostFocus);
            }
        }
        else if(mouse.getMode() == NONE)
        {
            if(selectedItems().empty())
            {
                prevScreenPos = event->screenPos();
                sign = true;
            }
            else
            {
                QList<QGraphicsItem *>select = selectedItems();
                mouse.temp_select = true;
                move = false;
                for (QGraphicsItem *item: qAsConst(select))
                {
                    item->setSelected(false);
                    if(dynamic_cast<FlipFlop *>(item))
                        dynamic_cast<FlipFlop *>(item)->spreadSelect();
                    else if(dynamic_cast<Component *>(item))
                        dynamic_cast<Component *>(item)->spreadSelect();
                }
            }
        }
        else if(mouse.getMode() == SELECT)
        {
            if(!selectBox->isSelected() || !selectBox->isVisible())
            {
                closeSelectBox();
                selectBox->setVisible(true);
                selectBox->setStartPos(event->scenePos());
                sign = true;
            }
            else
            {
                QList<QGraphicsItem *>select = selectedItems();
                for (QGraphicsItem *item: qAsConst(select))
                {
                    if(dynamic_cast<Component *>(item))
                    {
                        item->setSelected(false);
                        if(dynamic_cast<FlipFlop *>(item))
                            dynamic_cast<FlipFlop *>(item)->spreadSelect();
                        else
                            dynamic_cast<Component *>(item)->spreadSelect();
                    }
                    setMovable(true);
                    selectBox->reshape();
                }
                move = false;
            }
        }
        else if(mouse.getMode() == WIRE)
        {
            if(sign)
            {
                //add wire end point or stop drawing wire
                QGraphicsItem *item = itemAt(curr_wire->getPos(), QTransform());
                if(dynamic_cast<Wire *>(item) && item != curr_wire)
                {
                    sign = false;
                    if(!curr_wire->connectWire((Wire *)item))
                    {
                        removeItem(curr_wire);
                        delete curr_wire;
                    }
                    curr_wire = nullptr;
                    mouse.setNode(nullptr);
                    mouse.setWire(nullptr);
                }
                else if(dynamic_cast<Node *>(item))
                {
                    Node *node = (Node *)item;
                    if(node->getConnect())  return;
                    sign = false;
                    if(!curr_wire->connectNode(node))
                    {
                        removeItem(curr_wire);
                        delete curr_wire;
                    }
                    curr_wire = nullptr;
                    mouse.setNode(nullptr);
                    mouse.setWire(nullptr);
                }
                else
                    curr_wire->addEndPoint();
            }
            else
            {
                //create a new wire
                if(mouse.getNode())
                {
                    history.addStep(this);
                    sign = true;
                    curr_wire = new Wire(mouse.getNode());
                    mouse.setWire(curr_wire);
                    addItem(curr_wire);
                    curr_wire->setScene(this);
                }
                else if(mouse.getWire())
                {
                    history.addStep(this);
                    sign = true;
                    curr_wire = mouse.getWire();
                }
            }
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        if(curr_wire != nullptr)
        {
            if(!curr_wire->deleteLastEndPoint())
            {
                removeItem(curr_wire);
                delete curr_wire;
            }
            sign = false;
            curr_wire = nullptr;
        }
    }
}
void Circuit::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    mouse.temp_select = false;
    if(sign)
    {
        if(mouse.getMode() == NONE)
        {
            QPointF offsetPos = (event->screenPos() - prevScreenPos) / mouse.getRadio();
            QGraphicsView *view = views().at(0);
            view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            view->centerOn(centerPos - offsetPos);
        }
        else if(mouse.getMode() == SELECT)
        {
            selectBox->setEndPos(event->scenePos());
        }
        else if(mouse.getMode() == WIRE)
        {
            //draw wire
            curr_wire->changeLastEndPoint(event->scenePos());
            vLine->setPos(curr_wire->getPos().x(), 0);
            hLine->setPos(0, curr_wire->getPos().y());
            QGraphicsItem *item = itemAt(curr_wire->getPos(), QTransform());
            if (dynamic_cast<Wire *>(item) && item != curr_wire)
                dynamic_cast<Wire *>(item)->setHover();
            else if(dynamic_cast<Node *>(item))
                dynamic_cast<Node *>(item)->setHover();
        }
    }
    else
    {
        if(mouse.getMode() == WIRE)
        {
            vLine->setPos(event->scenePos().x(), 0);
            hLine->setPos(0, event->scenePos().y());
            QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
            if (dynamic_cast<Wire *>(item))
                dynamic_cast<Wire *>(item)->setHover();
            else if(dynamic_cast<Node *>(item))
                dynamic_cast<Node *>(item)->setHover();
        }
        if(!move && !mouse.isSimul())
        {
            history.addStep(this);
            move = true;
        }
    }
    update();
}
void Circuit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    mouse.temp_select = false;
    move = true;
    if(mouse.getMode() == NONE)
    {
        if(sign)
        {
            sign = false;
            QPointF offsetPos = (event->screenPos() - prevScreenPos) / mouse.getRadio();
            centerPos -= offsetPos;
        }
    }
    else if(mouse.getMode() == SELECT)
    {
        if(sign)
        {
            sign = false;
            selectBox->setEndPos(event->scenePos());
            setSelectionArea(selectBox->shape());
            if(selectedItems().length() <= 1)   //nothing in the selectBox
                closeSelectBox();
            else
            {
                setMovable(true);
                selectBox->reshape();
            }
        }
        selectBox->fixPos();
    }
    update();
}

void Circuit::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    mousePressEvent(event);
    QGraphicsScene::mouseDoubleClickEvent(event);
    QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
    // show info box
    if(mouse.getMode() != NONE)     return;
    if(selectedItems().empty())     return;
    if(mouse.isSimul())
    {
        if(!dynamic_cast<OutLabel *>(item) && !dynamic_cast<Text *>(item))
            return;
        if(dynamic_cast<Text *>(item))
        {
            Text *t = dynamic_cast<Text *>(item);
            if(!dynamic_cast<OutLabel *>(t->getBelong()))
                return;
            item = (OutLabel*)t->getBelong();
        }
        if(mouse.isSeq())
        {
            Waveform *waveform;
            waveform = new Waveform((OutLabel*)item);
            mouse.simul = waveform;
            mouse.showSimul->trigger();
        }
        else
        {
            Table *table;
            table = new Table((OutLabel*)item);
            mouse.simul = table;
            mouse.showSimul->trigger();
        }
        return;
    }
    if(dynamic_cast<Gate *>(item))
    {
        history.addStep(this);
        GateInfo gateinfo((Gate*)item);
        gateinfo.exec();
    }
    else if(dynamic_cast<FlipFlop *>(item))
    {
        history.addStep(this);
        FFInfo ffinfo((FlipFlop*)item);
        ffinfo.exec();
    }
    else if(dynamic_cast<CombIn *>(item))
    {
        history.addStep(this);
        CombInInfo ininfo((CombIn*)item);
        ininfo.exec();
    }
    else if(dynamic_cast<SeqIn *>(item))
    {
        history.addStep(this);
        SeqInInfo ininfo((SeqIn*)item);
        ininfo.exec();
    }
    else if(dynamic_cast<Text *>(item))
    {
        item = dynamic_cast<Text *>(item)->getBelong();
        if(dynamic_cast<CombIn *>(item))
        {
            history.addStep(this);
            CombInInfo ininfo((CombIn*)item);
            ininfo.exec();
        }
        else if(dynamic_cast<SeqIn *>(item))
        {
            history.addStep(this);
            SeqInInfo ininfo((SeqIn*)item);
            ininfo.exec();
        }
    }
}

void Circuit::setMovable(bool b)
{
    QList<QGraphicsItem *> selects = selectedItems();
    for (QGraphicsItem *item : qAsConst(selects))
        item->setFlag(QGraphicsItem::ItemIsMovable, b);
}

void Circuit::changeMode()
{
    setSelectionArea(bgItem->shape());
    if(mouse.getMode() == NONE)     setMovable(true);
    else     setMovable(false);
    clearSelection();

    selectBox->setVisible(false);
    vLine->setPos(-1, 0);
    hLine->setPos(0, -1);
}

void Circuit::textLostFocus(Label *item)
{
    QTextCursor cursor = item->text->textCursor();
    cursor.clearSelection();
    item->text->setTextCursor(cursor);

    if(OutLabel* out = dynamic_cast<OutLabel *>(item))
        out->setName(item->text->toPlainText());
    else
        item->setName(item->text->toPlainText());
    if (item->text->toPlainText().isEmpty()) {
        removeItem(item);
        delete item;
    }
}

void Circuit::closeSelectBox()
{
    setSelectionArea(bgItem->shape());
    setMovable(false);
    selectBox->setVisible(false);
    clearSelection();
}

bool Circuit::checkError()
{
    setSelectionArea(bgItem->shape());
    QVector <InNode *> in;
    QVector <OutNode *> out;
    QList<QGraphicsItem *> selects = selectedItems();
    clearSelection();
    // 檢查是否所有元件的input nodes都有連接上
    for (QGraphicsItem *item : qAsConst(selects))
    {
        in = dynamic_cast<Component *>(item)->getInNodes();
        out = dynamic_cast<Component *>(item)->getOutNodes();
        item->setSelected(true);
        if(!in.empty())
            for(auto i=in.begin(); i != in.end(); i++)
            {
                if((*i)->getConnect() == nullptr)
                    return false;
            }
        if(out.empty() && dynamic_cast<Wire *>(item))
            return false;
        if(FlipFlop* ff = dynamic_cast<FlipFlop *>(item))
            if(ff->getClock()->getConnect() == nullptr)
                return false;
        item->setSelected(false);
    }
    clearSelection();
    return true;
}

void Circuit::findComponents(QString keyword)
{
    setSelectionArea(bgItem->shape());
    QList<QGraphicsItem *> selects = selectedItems();
    clearSelection();
    for (QGraphicsItem *item : qAsConst(selects))
    {
        Component* c = dynamic_cast<Component *>(item);
        if(c)
        {
            if(c->getType().toLower()==keyword)
                c->setSelected(true);
            else if(c->getName().toLower().contains(keyword))
                c->setSelected(true);
        }
    }
}

void Circuit::writeToFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))     return;
    QTextStream out(&file);
    out << "CIRCUIT ";
    if(mouse.isSeq())   out << "SEQ\n";
    else                out << "COMB\n";

    setSelectionArea(bgItem->shape());
    QList<QGraphicsItem *> selects = selectedItems();
    clearSelection();
    if(!selects.empty())
    {
        QList<Wire *> wires;
        for (QGraphicsItem *item : qAsConst(selects))
        {
            Component* component = dynamic_cast<Component *>(item);
            Wire* wire = dynamic_cast<Wire *>(item);
            if(wire)
            {
                //wire->fixPos();
                wires.push_back(wire);
            }
            else if(component)
                out << component->getInfo() + '\n';
        }
        for(Wire* wire : qAsConst(wires))
            out << wire->getInfo() + '\n';
    }
    file.close();
}

void Circuit::readFromFile(QString filename)
{
    // init static data members
    Component::seq_num = 0;
    OutLabel::seq_num = 0;
    InLabel::seq_num = 0;
    Node::seq_num = 2;

    QFile file(filename);
    QTextStream in(&file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  return;
    QString line = in.readLine();
    QStringList words = line.split(" ");

    if(words[1] == "COMB")  mouse.setSeq(false);
    else                    mouse.setSeq(true);
    while(!(line = in.readLine()).isNull())
    {
        words = line.split("$");
        int type = mouse.stringToType(words[0]);
        Component *curr_component;
        switch (type)
        {
        case AND:   curr_component = new And(words);       break;
        case OR:    curr_component = new Or(words);        break;
        case NAND:  curr_component = new Nand(words);      break;
        case NOR:   curr_component = new Nor(words);       break;
        case XOR:   curr_component = new Xor(words);       break;
        case NOT:   curr_component = new Not(words);       break;
        case DFF:   curr_component = new Dff(words);       break;
        case TFF:   curr_component = new Tff(words);       break;
        case JKFF:  curr_component = new JKff(words);      break;
        case RSFF:  curr_component = new RSff(words);      break;
        case LABEL: curr_component = new Label(words);     break;
        case OUT:   curr_component = new OutLabel(words);  break;
        case IN:
            if(mouse.isSeq())   curr_component = new SeqIn(words);
            else                curr_component = new CombIn(words);
            break;
        default:
            QStringList endpoints_str = in.readLine().split(" ");
            QStringList check_str = in.readLine().split(" ");
            QVector<void *> check;
            int len = check_str.length() - 1;
            int id, i = 0;
            while(i < len)
            {
                id = check_str[i++].toInt();
                if(id == 0)
                    check.push_back(nullptr);
                else if(id == 1)
                    check.push_back((void *)1);
                else
                    check.push_back(findNodeByID(id));
            }
            curr_component = new Wire(words, endpoints_str, check);
            break;
        }
        addItem(curr_component);
        curr_component->setScene(this);
        if(type == LABEL || type == OUT || type == IN)
        {
            Label *label = dynamic_cast<Label *>(curr_component);
            connect(label->text, &Text::lostFocus, this, &Circuit::textLostFocus);
        }
    }
    file.close();
    // set movable
    if(mouse.getMode() == NONE)
    {
        setSelectionArea(bgItem->shape());
        setMovable(true);
    }
    clearSelection();
}

Node* Circuit::findNodeByID(int id)
{
    setSelectionArea(bgItem->shape());
    QList<QGraphicsItem *> selects = selectedItems();
    Node *node = nullptr;
    clearSelection();
    for (QGraphicsItem *item : qAsConst(selects))
    {
        if(dynamic_cast<Wire *>(item))  continue;
        if(dynamic_cast<FlipFlop *>(item))
            node = dynamic_cast<FlipFlop *>(item)->findNodeByID(id);
        else
            node = dynamic_cast<Component *>(item)->findNodeByID(id);
        if(node != nullptr)
            break;
    }
    return node;
}

void Circuit::centerOn()
{
    selectBox->setVisible(true);
    selectBox->setStartPos(QPointF(0, 0));
    selectBox->setEndPos(QPointF(width(), height()));
    setSelectionArea(selectBox->shape());
    if(selectedItems().length() > 1)
    {
        selectBox->reshape();
        centerPos = selectBox->boundingRect().center();
        views().at(0)->centerOn(centerPos);
    }
    selectBox->setVisible(false);
    clearSelection();
}

void Circuit::resetNames()
{
    setSelectionArea(bgItem->shape());
    QList<QGraphicsItem *> selects = selectedItems();
    clearSelection();

    InLabel *in;
    OutLabel *out;
    InLabel::names.clear();
    OutLabel::names.clear();
    for (QGraphicsItem *item : qAsConst(selects))
    {
        if((in = dynamic_cast<InLabel *>(item)))
            InLabel::names.push_back(in->getName());
        else if((out = dynamic_cast<OutLabel *>(item)))
            OutLabel::names.push_back(out->getName());
    }
}
