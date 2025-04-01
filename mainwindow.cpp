#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stack>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();

    QString a = ui->lineEdit->text();
    QString b = ui->lineEdit_2->text();

    int m = a.toInt();
    int n = b.toInt();

    if(n < 0 || m < 0)
    {
        ui->lineEdit_5->setText("Введены m < 0 или n < 0");
        return;
    }

    int a_result_rec = Ackermann_rec(m, n);
    ui->lineEdit_3->setText(QString::number(a_result_rec));

    int a_result_cycle = Ackermann_cycle(m, n);
    ui->lineEdit_4->setText(QString::number(a_result_cycle));
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
}

int MainWindow::Ackermann_rec(int m, int n) {
    if (m == 0) {
        return n + 1; // Условие A(0, n) = n + 1
    } else if (m > 0 && n == 0) {
        return Ackermann_rec(m - 1, 1); // Условие A(m, 0) = A(m - 1, 1)
    } else if (m > 0 && n > 0) {
        return Ackermann_rec(m - 1, Ackermann_rec(m, n - 1)); // Условие A(m, n) = A(m - 1, A(m, n - 1))
    }
    return -1; // На всякий случай, если условия не выполнены
}

int MainWindow::Ackermann_cycle(int m, int n) {
    std::stack<int> s;
    s.push(m);
    while(!s.empty()) {
        m = s.top();
        s.pop();
        if(m == 0) {
            n++;
        }
        else if(n == 0) {
            s.push(--m);
            n = 1;
        }
        else {
            s.push(m-1);
            s.push(m);
            n--;
        }
    }
    return n;
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->lineEdit_5->clear();

    QString a = ui->lineEdit_6->text();
    int m = a.toInt();
    if(m < 0)
    {
        ui->lineEdit_5->setText("Введено m < 0");
        return;
    }

    QVector<double> N {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    QVector<double> time_rec {};
    QVector<double> time_cycle {};

    for(int i = 0; i < N.size(); i++)
    {
        clock_t start_one = clock();
        for(int j = 0; j < 3; j++)
        {
            Ackermann_rec(m, static_cast<int>(N[i]));
        }
        clock_t end_one = clock();
        double result_one = (double(end_one - start_one) / CLOCKS_PER_SEC) * 1000 / 3; // milliseconds
        time_rec.push_back(result_one);

        clock_t start_two = clock();
        for(int j = 0; j < 3; j++)
        {
            Ackermann_cycle(m, static_cast<int>(N[i]));
        }
        clock_t end_two = clock();
        double result_two = (double(end_two - start_two) / CLOCKS_PER_SEC) * 1000 / 3; // milliseconds
        time_cycle.push_back(result_two);
    }

    double max = time_cycle[0];
    for(int i = 0; i < time_cycle.size(); i++)
    {
        if(time_cycle[i] > max)
        {
            max = time_cycle[i];
        }
        if(time_rec[i] > max)
        {
            max = time_rec[i];
        }
    }

    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(N, time_rec);
    ui->widget->graph(0)->setPen(QColor(50, 50, 50, 255));
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, 5));
    ui->widget->addGraph();
    ui->widget->graph(1)->setData(N, time_cycle);
    ui->widget->graph(1)->setPen(QColor(50, 50, 250, 255));
    ui->widget->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 5));
    ui->widget->xAxis->setLabel("n");
    ui->widget->yAxis->setLabel("t, ms");
    ui->widget->xAxis->setRange(0, N.size()+1);
    ui->widget->yAxis->setRange(0, max+0.1*max);
    ui->widget->replot();
}

