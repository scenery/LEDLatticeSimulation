#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QPicture>
#include <QFont>
#include <QDebug>
#include <QTextDocument>
#include <QTextCodec>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}
uchar code[128]={0};

void Widget::paintEvent(QPaintEvent *event)
{
    QString input = ui->textEdit->toPlainText();//获取输入框文本
    if ( input.isEmpty() )
    {
        //输入为空
    }
    else
    {
        int x = 32;
        int y = 4;
        int textLength;
        if(input.length() == 0){
            textLength = 1;
        }else{
            textLength = input.length();
        }
        x = x * textLength;
        y = y * textLength;

        QFont font;
        font.setFamily("Ubuntu");
        //font.setWeight(50);
        font.setPixelSize(12);
        QFontMetrics fm(font);
//        int charWidth = fm.width(input);
//        charWidth = fm.boundingRect(input).width();
        //指定图片大小为字体的大小
//        int x = charWidth+8;
//        int y = 17;

        QSize size(x, y);
        QImage image(x,x, QImage::Format_MonoLSB);
        image.fill(Qt::black);
        QPainter painter;

        //step1:绘制文字
        painter.begin(&image);
        //font.setPointSize(16);
        painter.setFont(font);
        painter.drawText(image.rect(),Qt::AlignCenter,input);
        for(int i=0;i<x;i++)
        {
            //for(int j=0;j<8;j++)
            //    qDebug()<<image.scanLine(i)[j];
            for(int m=0;m<y;m++){
                code[i*y+m]=~*(image.bits()+i*y+m); //image.bits()获取图像像素字节数据的首地址
            }
        }

        painter.end();

        //step2:绘制位图
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing,true);//抗锯齿
        //painter.drawImage(32,32,image.scaled(50,20));
        painter.drawImage(400,10,image);
        painter.end();

        //step3:绘制点阵字模
        painter.begin(this);
        QPen pen(Qt::red);
        for(int j=0;j<x;j++)
        {
            for(int k=0;k<y;k++)
            {
                for(int m=0;m<(x/y);m++)
                {
                    if((code[j*y+k])&0x01)
                    {
                        QFont font;
                        font.setPointSize(12);
                        //设置字符间距
                        //font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
                        painter.setFont(font);
                        pen.setColor(Qt::red);
                        painter.setPen(pen);
                        painter.drawText(QRectF((m+k*8)*5+50,j*5,200,20), Qt::AlignCenter,"■");
                    }
                    else
                    {
                        QFont font;
                        font.setPointSize(12);
                        //设置字符间距
                        //font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
                        painter.setFont(font);
                        pen.setColor(Qt::black);
                        painter.setPen(pen);
                        //painter.drawText(QRectF((m+k*8)*5+50,j*5,200,20), Qt::AlignCenter,"■");
                        painter.drawText(QRectF((m+k*8)*5+50,j*5,200,20), Qt::AlignCenter,"■");
                    }

                    code[j*y+k]>>=1;
                }

            }
        }
        painter.end();
    }

}

void Widget::on_pushButton_clicked()
{
    QWidget::repaint();
}
