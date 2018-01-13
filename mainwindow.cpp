#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QCursor>
#include <QClipboard>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowTitle(tr("Fitting Analysis"));
    ui->setupUi(this);
    QTableWidgetItem *headerH1=new QTableWidgetItem("测量值x");
    QTableWidgetItem *headerH2=new QTableWidgetItem("真实值y");
    ui->tableWidget->setHorizontalHeaderItem(0,headerH1);
    ui->tableWidget->setHorizontalHeaderItem(1,headerH2);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    popMenu=new QMenu(ui->tableWidget);
    copyAction=new QAction("copy",this);
    pasteAction=new QAction("paste",this);
    deleteAction=new QAction("clear",this);
    //copyAction->setShortcut(QKeySequence::Copy);
    //pasteAction->setShortcut(QKeySequence::Paste);
    connect(copyAction,SIGNAL(triggered(bool)),this,SLOT(on_copy_Clicked()));
    connect(deleteAction,SIGNAL(triggered(bool)),this,SLOT(on_delete_Clicked()));
    connect(pasteAction,SIGNAL(triggered(bool)),this,SLOT(on_paste_Clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    readData();
    Fit *fit=new Fit;
    fit->linearFit(lx,ly);
    for(int i=0;i<2;i++)
    {
        factor.push_back(fit->getFactor(i));
    }
    equationOutput();
    errorOutput(fit);
}

//从表格读入数据
void MainWindow::readData()
{
    lx.clear();
    ly.clear();
    factor.clear();
    //.clear();
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        if(!(ui->tableWidget->item(i,0)==NULL||             //判断指向该cell的指针是否为空
                     (ui->tableWidget->item(i,0)&&
                      ui->tableWidget->item(i,0)->text()==tr("")))) //判断该cell的text是否为空
        {
           lx.push_back(ui->tableWidget->item(i,0)->text().toDouble());
        }
        if(!(ui->tableWidget->item(i,1)==NULL||
                     (ui->tableWidget->item(i,1)&&
                      ui->tableWidget->item(i,1)->text()==tr(""))))
        {
           ly.push_back(ui->tableWidget->item(i,1)->text().toDouble());
        }
    }
    ui->textEdit->append(tr("Data inputs successfully"));
}

void MainWindow::equationOutput()                                   //拟合方程输出
{
    QString eq;
    vector<double>::iterator it_factor=factor.begin();
        for (int i=0;i<factor.size();i++)
        {
            if(i==0)
                eq+=tr("y=")+QString::number(*it_factor);
            else if(i==1)
                eq+=tr("+")+QString::number(*it_factor)+tr("*x");
            else
                eq+=tr("+")+QString::number(*it_factor)+tr("*x^")+QString::number(i);
            it_factor++;
        }

    ui->textEdit->append(eq);
}


//误差输出函数实现
void MainWindow::errorOutput(Fit *fitResult)
{
   ui->textEdit->append(tr("回归平方和SSR=")+QString::number(fitResult->ssr));
   ui->textEdit->append(tr("剩余平方和SSE=")+QString::number(fitResult->sse));
   ui->textEdit->append(tr("均方根误差RMSE=")+QString::number(fitResult->rmse));
   ui->textEdit->append(tr("----------------------------------------------------"));       //分隔符
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox msg;
        msg.setText(tr("please enter the degree to fit!"));
        //msg.about(this,tr("warning"),tr("please input the degree to fit"));
        msg.exec();
    }
    else{
        N=ui->lineEdit->text().toInt();
        readData();
        Fit *fit=new Fit;
        fit->polyfit(lx,ly,N);
        for(int i=0;i<=N;i++)
        {
            factor.push_back(fit->getFactor(i));
        }
        equationOutput();
        errorOutput(fit);
    }
}

void MainWindow::on_pushButton_3_clicked()
{


   filePath=QFileDialog::getOpenFileName(this,tr("选择标定文件"),".",tr("Text files(*.ini *.txt)"));
    ui->lineEdit_2->setText(filePath);

}

//选择标定文件对话框实现


void MainWindow::on_pushButton_4_clicked()
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream in(&file);
    QString line=in.readLine();
    while(!in.atEnd()){
        line=in.readLine();
        int i=line.indexOf('.',0);
        QString name=line.mid(0,i);
        ui->comboBox->addItem(name);
    }


}

void MainWindow::on_pushButton_5_clicked()
{
    /*int index=ui->comboBox->currentIndex();
    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
       { cout<<"文件打开失败";
        return;}
    QTextStream in(&file);
    QString line;
    for(int i=0;i<index+1;i++)
        line=in.readLine();
    line=in.readLine();
    int i=line.indexOf(',',0);
    QString a=QString::number(factor[0]);
    QString b=QString::number(factor[1]);
   // QString c=QString::number(factor[2]);
    line.replace(i+1,4,a);
    i=line.indexOf(',',i+1);
    line.replace(i+1,4,b);
    //in<<line<<'\n';
    //i=line.indexOf(',',i+1);
    //line.replace(i,4,c);
    file.close();*/
    int index=ui->comboBox->currentIndex();
    QString strAll;
    QStringList strList;
    QFile readFile(filePath);
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
     {
     QTextStream stream(&readFile);
     strAll=stream.readAll();
     }
     strList=strAll.split("\n");
     QString tmpStr=strList.at(index+1);
     int i=tmpStr.indexOf(',',0);
     int j=tmpStr.indexOf(',',i+1);
     QString a=QString::number(factor[0]);
     QString b=QString::number(factor[1]);
     tmpStr.replace(i+1,j-i-1,a);
     i=tmpStr.indexOf(',',i+1);
     j=tmpStr.indexOf(',',i+1);
     tmpStr.replace(i+1,j-i-1,b);
     if(N==2){
          QString c=QString::number(factor[2]);
         i=tmpStr.indexOf(',',i+1);
         j=tmpStr.indexOf(',',i+1);
         tmpStr.replace(i+1,j-i-1,c);
     }
     readFile.close();
     QFile writeFile(filePath);
     if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
      {
              QTextStream stream(&writeFile);
              for(int i=0;i<strList.count();i++)
              {
                  if(i==strList.count()-1)
                  {
                      if(i==index+1)
                          stream<<tmpStr;
                      else
                      //最后一行不需要换行
                      stream<<strList.at(i);
                  }
                  else
                  {
                      if(i==index+1)
                          stream<<tmpStr<<'\n';
                      else
                        stream<<strList.at(i)<<'\n';
                  }
              }
       }
     writeFile.close();
}



void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    popMenu->addAction(copyAction);
    popMenu->addAction(pasteAction);
    popMenu->addAction(deleteAction);
    popMenu->exec(QCursor::pos());
}


void MainWindow::on_delete_Clicked()
{
    ui->tableWidget->clearContents();
}

 void MainWindow::on_copy_Clicked()
 {
    QClipboard *board=QApplication::clipboard();
    QList<QTableWidgetItem*> selectedItem=ui->tableWidget->selectedItems();
    QString textString;
    for(int i=0;i<selectedItem.size();i++)
    {
        textString=textString+selectedItem[i]->text()+'\t';
        i++;
        textString=textString+selectedItem[i]->text()+'\n';
    }
    board->setText(textString);
 }
void MainWindow::on_paste_Clicked()
{
    QClipboard *board=QApplication::clipboard();
    QString str=board->text();
    str=str.simplified();
    QStringList strList=str.split(' ');
    QList<QTableWidgetItem*>listItem;
    for(int i=0;i<strList.count();i++)
    {
        QTableWidgetItem *item=new QTableWidgetItem(strList.at(i));
        listItem<<item;
    }
    int rowCount=strList.count()/2;
    int currentRow=ui->tableWidget->currentRow();
    for(int i=0;i<rowCount;i++)
    {
        ui->tableWidget->setItem(currentRow+i,0,listItem[2*i]);
        ui->tableWidget->setItem(currentRow+i,1,listItem[2*i+1]);

    }
}

void MainWindow::on_pushButton_6_clicked()
{
    int index=ui->comboBox->currentIndex();
    QString strAll;
    QStringList strList;
    QFile readFile(filePath);
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
     {
     QTextStream stream(&readFile);
     strAll=stream.readAll();
     }
     strList=strAll.split("\n");
     QString tmpStr=strList.at(index+1);
     int i=tmpStr.indexOf(',',0);
     int j=tmpStr.indexOf(',',i+1);
     QString a=tr("0.00");
     QString b=tr("1.00");
     tmpStr.replace(i+1,j-i-1,a);
     i=tmpStr.indexOf(',',i+1);
     j=tmpStr.indexOf(',',i+1);
     tmpStr.replace(i+1,j-i-1,b);
          QString c=tr("0.00");
         i=tmpStr.indexOf(',',i+1);
         j=tmpStr.indexOf(',',i+1);
         tmpStr.replace(i+1,j-i-1,c);
     readFile.close();
     QFile writeFile(filePath);
     if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
      {
              QTextStream stream(&writeFile);
              for(int i=0;i<strList.count();i++)
              {
                  if(i==strList.count()-1)
                  {
                      if(i==index+1)
                          stream<<tmpStr;
                      else
                      //最后一行不需要换行
                      stream<<strList.at(i);
                  }
                  else
                  {
                      if(i==index+1)
                          stream<<tmpStr<<'\n';
                      else
                        stream<<strList.at(i)<<'\n';
                  }
              }
       }
     writeFile.close();
}
