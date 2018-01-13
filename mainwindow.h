#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <vector>
#include "fit.h"
#include <QFileDialog>
#include <QMenu>
#include <QAction>

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void readData();
    void equationOutput();                  //输出拟合方程
    void errorOutput(Fit *fitResult);               //误差输出函数
    int N=0;     //多项式拟合次数
    ~MainWindow();

private slots:
    void on_pushButton_clicked();




    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);
   void on_copy_Clicked();
    void on_paste_Clicked();
    void on_delete_Clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    vector<double>lx,ly;
    vector<double>factor;
    QFileDialog *fileDialog;
    QString filePath;
    QMenu *popMenu;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;

};

#endif // MAINWINDOW_H
