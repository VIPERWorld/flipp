#include "linkerwidget.h"

LinkerWidget::LinkerWidget(QWidget *parent) :
    QWidget(parent)
{
    linkerUnit = new LinkerUnit;

    setupUI();
    connect(connectionBox,SIGNAL(activated(QString)),this,SLOT(changeConnection(QString)));
    connect(parserBox,SIGNAL(activated(QString)),this,SLOT(changeParser(QString)));
    connect(removeButton,SIGNAL(clicked()),this,SIGNAL(removeLinker()));
    connect(linkerUnit,SIGNAL(newVariableList()),this,SLOT(populateParserTable()));
    connect(linkerUnit,SIGNAL(newDataPoint()),this,SLOT(linkerDataReady()));
}

LinkerWidget::LinkerWidget(QWidget *parent, LinkerUnit *lUnit) :
    QWidget(parent),
    linkerUnit(lUnit)
{
    setupUI();
    connect(connectionBox,SIGNAL(activated(QString)),this,SLOT(changeConnection(QString)));
    connect(parserBox,SIGNAL(activated(QString)),this,SLOT(changeParser(QString)));
    connect(removeButton,SIGNAL(clicked()),this,SIGNAL(removeLinker()));
    connect(linkerUnit,SIGNAL(newVariableList()),this,SLOT(populateParserTable()));
    connect(linkerUnit,SIGNAL(newDataPoint()),this,SLOT(linkerDataReady()));
}

LinkerWidget::~LinkerWidget()
{

}

void LinkerWidget::setupUI()
{
    quint8 controlHeight = 28;

    connectionBox = new QComboBox;
    connectionBox->setFixedHeight(controlHeight);
    parserBox = new QComboBox;
    parserBox->setFixedHeight(controlHeight);
    removeButton = new QPushButton("Delete");
    removeButton->setFixedHeight(controlHeight);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);
    QStringList tableHeaders;
    tableHeaders.append("Variable");
    tableHeaders.append("Value");
    tableWidget->setHorizontalHeaderLabels(tableHeaders);
    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QHeaderView *hv = tableWidget->horizontalHeader();

    hv->setSectionsClickable(false);

    dataSourceLayout = new QHBoxLayout;
    dataSourceLayout->addWidget(connectionBox);
    dataSourceLayout->addWidget(parserBox);
    dataSourceLayout->addWidget(removeButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(dataSourceLayout);
    mainLayout->addWidget(tableWidget);

    this->setLayout(mainLayout);

    setMinimumWidth(100);

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void LinkerWidget::updateConnections(QStringList connectionNames)
{
    connectionBox->clear();
    connectionBox->addItems(connectionNames);
}

void LinkerWidget::updateParsers(QStringList parserNames)
{
    parserBox->clear();
    parserBox->addItems(parserNames);
    if(parserBox->count()==1)
    {
        emit linkerParserRequest(parserBox->currentText());
    }
}

QString LinkerWidget::getConnection()
{
    return linkerUnit->getConnectionName();
}

QString LinkerWidget::getParser()
{
    return parserBox->currentText();
}

void LinkerWidget::setConnection(QString newConnection)
{
    connectionBox->setCurrentText(newConnection);
    emit linkerConnectionRequest(newConnection);
}

void LinkerWidget::changeConnection(QString connection)
{
    emit linkerConnectionRequest(connection);
}

void LinkerWidget::assignConnection(ConnectionUnit *connUnit)
{
    linkerUnit->assignConnection(connUnit);
}

void LinkerWidget::detachConnection()
{
    linkerUnit->detachConnection();
}

void LinkerWidget::changeParser(QString parserName)
{
        emit linkerParserRequest(parserName);
}


//void LinkerWidget::assignParser(ParserUnit *pUnit)
//{
//    linkerUnit->
//    variables = parser->variableList;

//    linkerUnit->assignVariables(parser->parserUnit->getList());
//    populateParserTable();


//    variables = parser->variableList;
//    results.clear();
//    foreach(ComplexVariable cv,variables)
//    {
//        ParsedVariable i;
//        i.type=cv.type;
//        results.append(i);
//    }
//    parserEngine->setVariables(parser->variableList);
//    populateParserTable();
//    connect(parser,SIGNAL(updateVariableList(QList<ComplexVariable>)),this,SLOT(newParserVariables(QList<ComplexVariable>)));
//    connect(parser,SIGNAL(deleteParser()),this,SLOT(detachParser()));
//    connect(connectionUnit,SIGNAL(dataIn(QByteArray)),parserEngine,SLOT(parseData(QByteArray)));
//    connect(parserEngine,SIGNAL(dataParsed(VariableList)),this,SLOT(parsedDataReady(VariableList)));
//    parserEngine->setParser(true);
//}

//void LinkerWidget::newParserVariables()
//{
//        populateParserTable();
//    variables = newVars;
//    linkerUnit->assignVariables(newVars);


//    variables = newVars;
//    results.clear();
//    foreach(ComplexVariable cv,newVars)
//    {
//        ParsedVariable i;
//        i.type=cv.type;
//        results.append(i);
//    }

//    disconnect(connectionUnit,SIGNAL(dataIn(QByteArray)),parserEngine,SLOT(parseData(QByteArray)));
//    disconnect(parserEngine,SIGNAL(dataParsed(VariableList)),this,SLOT(parsedDataReady(VariableList)));
//    parserEngine->setVariables(newVars);
//    parserEngine->clearVariables();
//    populateParserTable();
//    connect(connectionUnit,SIGNAL(dataIn(QByteArray)),parserEngine,SLOT(parseData(QByteArray)));
//    connect(parserEngine,SIGNAL(dataParsed(VariableList)),this,SLOT(parsedDataReady(VariableList)));
//}

void LinkerWidget::detachParser()
{


    //    if(parserBox->count()>0)
    //    {
    //        emit linkerParserRequest(parserBox->currentText());
    //    }
    //    else
    //    {
    //        disconnect(connectionUnit,SIGNAL(dataIn(QByteArray)),parserEngine,SLOT(parseData(QByteArray)));
    //        QList<ComplexVariable> emptyList;
    //        parserEngine->setVariables(emptyList);
    //        connect(connectionUnit,SIGNAL(dataIn(QByteArray)),parserEngine,SLOT(parseData(QByteArray)));
    //    }
}

void LinkerWidget::populateParserTable()
{
    quint8 i=0;

    tableWidget->setRowCount(calcRowCount());
    //    if(parserEngine->getVariables().size()>0)
    if(linkerUnit->getVariables().size()>0)
    {
        foreach(ComplexVariable var, linkerUnit->getVariables())
            //        foreach(ComplexVariable var, parserEngine->getVariables())
            //        variables = linkerUnit->getVariables();
            //        for(quint8 j=0;j<variables->size();j++)
        {
            if(var.type==VECTYPE)
            {
                QString fullName;
                foreach(BaseVariable bvar, var.vector)
                {
                    fullName.clear();
                    fullName.append(var.name);
                    fullName.append(".");
                    fullName.append(bvar.name);
                    QTableWidgetItem *item = new QTableWidgetItem(fullName);
                    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                    item->setFlags(Qt::NoItemFlags);
                    tableWidget->setItem(i,0,item);
                    QTableWidgetItem *item2 = new QTableWidgetItem("-");
                    item2->setTextAlignment(Qt::AlignBottom);
                    item2->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                    item2->setFlags(Qt::NoItemFlags);
                    tableWidget->setItem(i,1,item2);
                    i++;
                }
            }
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem(var.name);
                item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                item->setFlags(Qt::NoItemFlags);
                tableWidget->setItem(i,0,item);
                QTableWidgetItem *item2 = new QTableWidgetItem("-");
                item2->setTextAlignment(Qt::AlignBottom);
                item2->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
                item2->setFlags(Qt::NoItemFlags);
                tableWidget->setItem(i,1,item2);
                i++;
            }
            tableWidget->setRowHeight(i,28);
        }

        //        QByteArray *check = new QByteArray;
        //        if(!parserEngine->isValid(check))
        //        {
        //            qDebug() << *check;
        //        }
    }
}

quint8 LinkerWidget::calcRowCount()
{
    // Calculate the total number of rows to use based on the total amount of variables.
    quint8 total=0;
    //    variables = linkerUnit->getVariables();
    foreach(ComplexVariable cv, linkerUnit->getVariables())
        //    foreach(ComplexVariable cv, parserEngine->getVariables())
        //    for(quint8 j=0;j<variables->size();j++)
    {
        if(cv.type==VECTYPE)
        {
            total+=cv.vector.size();
        }
        else
        {
            total++;
        }
    }
    return total;
}

//void LinkerWidget::parsedDataReady(VariableList parsedData)
//{
//    int complexCount=0;

//    if(parsedData.size()==tableWidget->rowCount())
//    {
//        foreach(RepeatedVector repVector, parsedData)
//        {
//            QTableWidgetItem *item = tableWidget->item(complexCount,1);
//            if(repVector.vectors.size()<2)
//            {
//                // Single variable
//                if(repVector.vectors.at(0).vector.at(0).varType==BYTTYPE)
//                {
//                    item->setText(repVector.vectors.at(0).vector.at(0).varBytes);
//                }
//                else
//                {
//                    // Number variable
//                    double numVal = repVector.vectors.at(0).vector.at(0).varValue;
//                    item->setText(QString("%1").arg(numVal));
//                    results[complexCount].content = repVector.vectors.at(0).vector.at(0).varBytes;
//                    results[complexCount].value = repVector.vectors.at(0).vector.at(0).varValue;
//                }

//            }
//            complexCount++;
//        }
//        emit newDataPoint();
//    }
//}

void LinkerWidget::linkerDataReady()
{
    quint8 complexCount=0;
    foreach(ParsedVariable variable, linkerUnit->results)
    {
        QTableWidgetItem *item = tableWidget->item(complexCount,1);
        // Single variable
        if(variable.type==BYTTYPE)
        {
            item->setText(variable.content);
        }
        else if(variable.type==NUMTYPE)
        {
            // Number variable
            item->setText(QString("%1").arg(variable.value));
        }
        complexCount++;
    }
}
