#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qpainter.h"
#include "math.h"
#include "D:\User\Documents\VisualStudio2012\Projects\test1\test1\test1.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    ui->setupUi(this);
}
void MainWindow::paintEvent(QPaintEvent *event)
{
	   
       Q_UNUSED(event);
       QPainter painter(this);
       // 反走样
       //painter.setRenderHint(QPainter::Antialiasing, true);
       QPen pen;
       pen.setWidth(1);
       pen.setBrush(Qt::black);
       // 设置画笔颜色
       painter.setPen(pen);
       //drawLine(点1，点2);QPointF(x,y)
       double a,b,c,d;
       double e,f;
       e=240;f=360;
       a=50;b=50;c=170;d=480;
       painter.drawLine(a,b,a,c);//绘制y轴(左)
       painter.drawLine(d,b,d,c);//绘制y'轴(右)
       painter.drawLine(a,b,d,b);//绘制x轴(下)
       painter.drawLine(a,c,d,c);//绘制x'轴(上)
       float i;
       //图1
       //title,xlabel,ylabel
       painter.rotate(270);//旋转文本方向
       painter.drawText(-130,20,QStringLiteral("幅值"));
       painter.drawText(-300,20,QStringLiteral("基频"));
       painter.rotate(90);//恢复
       painter.drawText((d-a)/2,b-15,QStringLiteral("语音信号"));
       painter.drawText((d-a)/2,f+35,QStringLiteral("时间/s"));
       painter.drawText((d-a)/2,e-15,QStringLiteral("倒谱法基音周期检测"));
       for(i=0;i<3.5;i+=0.5)
       {
           painter.drawLine(i*120+a,c,i*120+a,c-5);//绘制x轴上的点
           painter.drawLine(i*120+a,b,i*120+a,b+5);
           painter.drawText(i*120+a,c+20,QString::number(i));//绘制文本
       }
       for(i=0;i<2.5;i+=0.5)
       {
           painter.drawLine(a,c-i*60,a+5,c-i*60);//绘制y轴上的点
           painter.drawLine(d,c-i*60,d-5,c-i*60);
           if(i-1==-0.5)
               painter.drawText(a-30,c-i*60,QString::number(i-1));//绘制文本
           else
            painter.drawText(a-20,c-i*60,QString::number(i-1));//绘制文本
       }
       //图2

       //绘制网格线
       QPen pen1;
       pen1.setBrush(Qt::gray);
       painter.setPen(pen1);
       for(i=0;i<3.5;i+=0.5)
       {
           painter.drawLine(a+i*120,f,a+i*120,e);
       }
       for(i=0;i<2;i+=0.5)
       {
           painter.drawLine(a,f-i*80,d,f-i*80);
       }
       //绘制坐标
       painter.setPen(pen);
       painter.drawLine(a,e,a,f);//绘制y轴(左)
       painter.drawLine(d,e,d,f);//绘制y'轴(右)
       painter.drawLine(a,f,d,f);//绘制x轴(下)
       painter.drawLine(a,e,d,e);//绘制x'轴(上)
       for(i=0;i<3.5;i+=0.5)
       {
           painter.drawLine(i*120+a,f,i*120+a,f-5);//绘制x轴上的点
           painter.drawLine(i*120+a,e,i*120+a,e+5);
           painter.drawText(i*120+a,f+20,QString::number(i));//绘制文本
       }
       for(i=0;i<4;i+=1)
       {
           painter.drawLine(a,f-i*40,a+5,f-i*40);//绘制y轴上的点
           painter.drawLine(d,f-i*40,475,f-i*40);
           painter.drawText(a-20,f-i*40,QString::number(i*200));//绘制文本
       }

       //数据 time-->x,x-->y
       QVector<float> time;
       QVector<float> x;
       /*for(i=0;i<100;i++)
            time.append(sin(i)+1);
       for(i=0;i<100;i++)
            x.append(sin(i));*/

       time.append(0);time.append(0.5);time.append(1);time.append(1.5);
       x.append(-1);x.append(1);x.append(-0.5);x.append(0);
       painter.setPen(pen);
       double x1,y1,x2,y2;
       x1=time[0];
       y1=x[0];
       //painter.drawLine(x1*120+50,110-y1*60,x2*120+50,110-y2*60);
       for(i=1;i<time.length();i++)
       {

            x2=time[i];
            y2=x[i];
            painter.drawLine(x1*120+a,c-60-y1*60,x2*120+a,c-60-y2*60);
            x1=time[i];
            y1=x[i];
       }
       //画图二绿框
       QVector<float> frameTime;
       QVector<float> F0;
       frameTime.append(0);frameTime.append(0.2);frameTime.append(0.4);frameTime.append(0.6);
       F0.append(0);F0.append(100);F0.append(200);F0.append(300);

       for(i=0;i<F0.length();i++)
       {
           x1=frameTime[i];
           y1=F0[i];
           QPen pen2(QColor(0,255,0),5);//绿色填充
           QPen pen3(QColor(0,0,0),1.5);//黑色
           painter.setPen(pen2);
           painter.drawRect(x1*120+a-3,f-y1/200*40-3,6,6);
           painter.setPen(pen3);
           painter.drawRect(x1*120+a-4,f-y1/200*40-5,10,10);
       }

       //根据voiceseg的值画蓝线红线
       QVector<double> nx1,nx2;
       nx1.append(0.128);nx2.append(0.576);

       QPen pen4(QColor(0,0,255),1);//蓝色
       QVector<qreal>dashes;
       qreal space =3;
       dashes<<10<<space<<10<<space;//设置虚线
       pen4.setDashPattern(dashes);
       QPen pen5(QColor(255,0,0),1);//红色
       //图1
       //画红线
       painter.setPen(pen5);
       painter.drawLine(nx1[0]*120+a,c,nx1[0]*120+a,b);
       //画蓝线
       painter.setPen(pen4);
       painter.drawLine(nx2[0]*120+a,c,nx2[0]*120+a,b);

       //图2
       //画红线
       painter.setPen(pen5);
       painter.drawLine(nx1[0]*120+a,f,nx1[0]*120+a,e);
       //画蓝线
       painter.setPen(pen4);
       painter.drawLine(nx2[0]*120+a,f,nx2[0]*120+a,e);
}
MainWindow::~MainWindow()
{
    delete ui;
}
