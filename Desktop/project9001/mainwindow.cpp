#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    //делаем сеточку
    int h=70;
//    test.dat
    int ii = 50;
    int jj = 98;
//    f.dat
//    int ii = 22;
//    int jj =102;
    double x[ii][jj];
    double y[ii][jj];
    double sizeF[ii][jj];
    double sizex[ii][jj];
    double sizey[ii][jj];
    double F[ii][jj];
    double Nx[ii][jj];
    double Ny[ii][jj];
    double eps = pow(10, -30);
    double alpha[ii][jj];
    double SB[ii][jj];
    double ST[ii][jj];
    double SR[ii][jj];
    double SL[ii][jj];
    int CS;

    int n=0;
    for(int i=0;i<ii; i++){
        for(int j=0; j<jj; j++){
            x[i][j]=h*i;
            y[i][j]=h*j;
            n++;

            scene->addRect(QRectF(x[i][j],(-1)*y[i][j],h,h),QPen(Qt::black), QBrush(Qt::cyan));
        }
    }

    //пробегаемся по массиву
    QFile file("C:/Users/oneone/Downloads/test.dat");
    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!file.atEnd())
        {
            for (int i=0;i<ii; i++){
                for (int j=0; j<jj; j++){
                    //читаем строку
                    QString str = file.readLine();
                    //Делим строку на слова разделенные пробелом

                    QStringList lst = str.split(" ");

                    sizex[i][j]=lst.at(0).toDouble();
                    sizey[i][j]=lst.at(1).toDouble();
                    sizeF[i][j]=lst.at(2).toDouble();
                    //костыль
                    F[i][j]=sizeF[i][j];
                    //Nx[i][j]=sizex[i][j];
                    //Ny[i][j]=sizey[i][j];

                    //qDebug()<<Nx[i][j];
                    //qDebug() << k <<"  " <<" F  " << sizeF[i][j];
                }
            }
        }
    }

    for (int i=0; i<ii; i++){
        for(int j=0; j<jj; j++){
            Nx[i][j] = -(F[i + 1][j + 1] + 2 * F[i + 1][j] + F[i + 1][j - 1] - F[i - 1][j + 1] - 2 * F[i - 1][j] - F[i - 1][j - 1]);
            Ny[i][j] = -(F[i + 1][j + 1] + 2 * F[i][j + 1] + F[i - 1][j + 1] - F[i - 1][j - 1] - 2 * F[i][j - 1] - F[i + 1][j - 1]);
        }
    }

    //заполнение значений альфа
    for(int i=0; i<ii; i++){
        for (int j=0; j<jj; j++){
            if((Nx[i][j]>=0 and Ny[i][j]>=0)or(Nx[i][j]<=0 and Ny[i][j]<=0)){
                alpha[i][j]=M_PI_2-fabs(atan((Ny[i][j])/(Nx[i][j]+eps)));
            }
            else if((Nx[i][j]>=0 and Ny[i][j]<=0)or(Nx[i][j]<=0 and Ny[i][j]>=0)){
                alpha[i][j]=fabs(atan((Nx[i][j])/(Ny[i][j]+eps)));
            }
        }
    }



    for(int i=0;i<ii; i++){
        for(int j=0; j<jj; j++){
            //F=1
            if(F[i][j]==1) {
                scene->addRect(
                            QRectF(x[i][j],(-1)*y[i][j],h,h),
                            QPen(Qt::black),
                            QBrush(Qt::blue));
            }


            if(alpha[i][j]<M_PI_4){
                if (F[i][j]<0.5*tan(alpha[i][j])){
                    CS=1;
                }
                else if(F[i][j]<1-0.5*tan(alpha[i][j])){
                    CS=2;
                }
                else {
                    CS=4;
                }
            }
            else if(alpha[i][j]>M_PI_4){
                if (F[i][j]<0.5/tan(alpha[i][j])){
                    CS=1;
                }
                else if(F[i][j]<(1-0.5/tan(alpha[i][j]))){
                    CS=3;
                }
                else{
                    CS=4;
                }
            }

            switch (CS)
            {
            case 1:

                if ((Nx[i][j] < 0.0) && (Ny[i][j] >= 0.0))
                {
                    SB[i][j] = sqrt(2.0 * F[i][j] * h * h / tan(alpha[i][j]));
                    SR[i][j] = sqrt(2.0 * F[i][j] * h * h * tan(alpha[i][j]));

                    if(SB[i][j] or SR[i][j]>0){
                        double aaa = SB[i][j];
                        double bbb = SR[i][j];
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(-aaa+x[i][j]+h,(-1)*y[i][j]+h)
                                          <<QPointF(x[i][j]+h,-bbb-y[i][j]+h)
                                          <<QPointF(x[i][j]+h,(-1)*y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    ST[i][j] = sqrt(2.0 * F[i][j] * h * h / tan(alpha[i][j]));
                    SR[i][j] = sqrt(2.0 * F[i][j] * h * h * tan(alpha[i][j]));
                    if(ST[i][j]>0 or SR[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(-ST[i][j]+h+x[i][j],-y[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j])
                                          <<QPointF(x[i][j]+h,SR[i][j]-y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    ST[i][j] = sqrt(2.0 * F[i][j] * h * h / tan(alpha[i][j]));
                    SL[i][j] = sqrt(2.0 * F[i][j] * h * h * tan(alpha[i][j]));

                    if(ST[i][j]>0 or SL[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(ST[i][j]+x[i][j],-y[i][j])
                                          <<QPointF(x[i][j],SL[i][j]-y[i][j])
                                          <<QPointF(x[i][j],-y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] >= 0.0))
                {
                    SB[i][j] = sqrt(2.0 * F[i][j] * h * h / tan(alpha[i][j]));
                    SL[i][j] = sqrt(2.0 * F[i][j] * h * h * tan(alpha[i][j]));
                    if(SB[i][j]>0 or SL[i][j]>0){
                        //лишняя координата wut
                        //i=1, j=0; SB=14,142 Sl=14,142
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(SB[i][j]+x[i][j],(-1)*y[i][j]+h)
                                          <<QPointF(x[i][j],-SL[i][j]-y[i][j]+h)
                                          <<QPointF(x[i][j],(-1)*y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                break;

            case 2:

                if ((Nx[i][j] < 0.0) && (Ny[i][j] >= 0.0))
                {
                    SL[i][j] = F[i][j] * h - 0.5 * h * tan(alpha[i][j]);
                    SR[i][j] = F[i][j] * h + 0.5 * h * tan(alpha[i][j]);

                    if (SL[i][j]>0 or SR[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j], -SL[i][j]-y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -SR[i][j]-y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    SL[i][j] = F[i][j] * h - 0.5 * h * tan(alpha[i][j]);
                    SR[i][j] = F[i][j] * h + 0.5 * h * tan(alpha[i][j]);

                    if (SL[i][j]>0 or SR[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j], -y[i][j])
                                          <<QPointF(x[i][j], SL[i][j]-y[i][j])
                                          <<QPointF(x[i][j]+h, SR[i][j]-y[i][j])
                                          <<QPointF(x[i][j]+h, -y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    SL[i][j] = F[i][j] * h + 0.5 * h * tan(alpha[i][j]);
                    SR[i][j] = F[i][j] * h - 0.5 * h * tan(alpha[i][j]);
                    if (SL[i][j]>0 or SR[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j], -y[i][j])
                                          <<QPointF(x[i][j], SL[i][j]-y[i][j])
                                          <<QPointF(x[i][j]+h, SR[i][j]-y[i][j])
                                          <<QPointF(x[i][j]+h, -y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));

                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] >= 0.0))
                {
                    SL[i][j] = F[i][j] * h + 0.5 * h * tan(alpha[i][j]);
                    SR[i][j] = F[i][j] * h - 0.5 * h * tan(alpha[i][j]);
                    if (SL[i][j]>0 or SR[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j], -SL[i][j]-y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -SR[i][j]-y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                break;

            case 3:

                if ((Nx[i][j] < 0.0) && (Ny[i][j] >= 0.0))
                {
                    SB[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);

                    if(SB[i][j]>0 or ST[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(h - ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(h - SB[i][j]+x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    SB[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    if(SB[i][j]>0 or ST[i][j]>0){
                        //часть криво wat
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(h-ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(h - SB[i][j]+x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    SB[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    if(SB[i][j]>0 or ST[i][j]>0){
                        //часть криво wat
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(SB[i][j]+x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] >= 0.0))
                {
                    SB[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);

                    if(SB[i][j]>0 or ST[i][j]>0){
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(SB[i][j]+x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                break;

            case 4:

                if ((Nx[i][j] < 0.0) && (Ny[i][j] > 0.0))
                {
                    ST[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    SL[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    if(h>ST[i][j]>0 || h>SL[i][j]>0){
                        QPolygonF polygon;

                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j],h-SL[i][j]-y[i][j])
                                          <<QPointF(x[i][j]-ST[i][j]+h,-y[i][j])
                                          <<QPointF(x[i][j]+h, -y[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    SL[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    SB[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    if(SL[i][j]>0 or SB[i][j]>0){
                        //лишние фигуры которые не особо мешают хз
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(h-SB[i][j]+x[i][j],(-1)*y[i][j]+h)
                                          <<QPointF(x[i][j],SL[i][j]-y[i][j])
                                          <<QPointF(x[i][j],-y[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    SB[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    SR[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    if(SR[i][j]>0 or SB[i][j]>0){
                        //лишние фигуры которые не особо мешают хз
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j],-y[i][j]+h)
                                          <<QPointF(x[i][j],-y[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j])
                                          <<QPointF(x[i][j]+h,SR[i][j]-y[i][j])
                                          <<QPointF(SB[i][j]+x[i][j],-y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] >= 0.0))
                {
                    SR[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    ST[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    if(h>SR[i][j]>0 or h>ST[i][j]>0){

                        //перепроверить
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j],-y[i][j]+h)
                                          <<QPointF(x[i][j],-y[i][j])
                                          <<QPointF(x[i][j]+ST[i][j],-y[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j]+h-SR[i][j])
                                          <<QPointF(x[i][j]+h,-y[i][j]+h),
                                          QPen(Qt::black), QBrush(Qt::blue));
                    }
                }
                break;
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
