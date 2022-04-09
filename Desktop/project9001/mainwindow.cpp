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
    int h=10;
    //double alpha =1;
    double x[22][102];
    double y[22][102];
    double sizeF[22][102];
    double sizex[22][102];
    double sizey[22][102];
    double F[22][102];
    double Nx[22][102];
    double Ny[22][102];
    double eps = pow(10, -30);
    double alpha[22][102];
    double SB[22][102];
    double ST[22][102];
    double SR[22][102];
    double SL[22][102];
    int CS;
    int temp=0;

    int n=0;
    for(int i=0;i<22; i++){
        for(int j=0; j<102; j++){
            x[i][j]=h*i;
            y[i][j]=h*j;
            n++;

            scene->addRect(QRectF(x[i][j],(-1)*y[i][j],h,h),QPen(Qt::black), QBrush(Qt::cyan));

            /*qDebug()<<i<<"  "<<j<< " x  "<<x[i][j];
            qDebug()<<i<<"  "<<j<< " y  "<<y[i][j];
            qDebug();*/
        }
    }
    //    qDebug()<<n;

    scene->addRect(x[0][0],(-1)*y[0][0],x[21][101],(-1)*y[21][101]);

    //пробегаемся по массиву
    QFile file("C:/Users/oneone/Downloads/F.dat");
    if(file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!file.atEnd())
        {
            for (int i=0;i<22; i++){
                for (int j=0; j<102; j++){
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

    for (int i=0; i<22; i++){
        for(int j=0; j<102; j++){
            Nx[i][j] = -(F[i + 1][j + 1] + 2 * F[i + 1][j] + F[i + 1][j - 1] - F[i - 1][j + 1] - 2 * F[i - 1][j] - F[i - 1][j - 1]);
            Ny[i][j] = -(F[i + 1][j + 1] + 2 * F[i][j + 1] + F[i - 1][j + 1] - F[i - 1][j - 1] - 2 * F[i][j - 1] - F[i + 1][j - 1]);
        }
    }

    //заполнение значений альфа
    for(int i=0; i<22; i++){
        for (int j=0; j<102; j++){
            if((Nx[i][j]>=0 and Ny[i][j]>=0)or(Nx[i][j]<=0 and Ny[i][j]<=0)){
                alpha[i][j]=M_PI_2-fabs(atan((Ny[i][j])/(Nx[i][j]+eps)));

            }
            else if((Nx[i][j]>=0 and Ny[i][j]<=0)or(Nx[i][j]<=0 and Ny[i][j]>=0)){
                alpha[i][j]=fabs(atan((Nx[i][j])/(Ny[i][j]+eps)));
            }
        }
    }



    for(int i=0;i<22; i++){
        for(int j=0; j<102; j++){
            //F=1
            if(F[i][j]==1) {
                scene->addRect(QRectF(x[i][j],(-1)*y[i][j],h,h),QPen(Qt::black), QBrush(Qt::blue));
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
                                          QPen(Qt::black), QBrush(Qt::red));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    ST[i][j] = sqrt(2.0 * F[i][j] * h * h / tan(alpha[i][j]));
                    SR[i][j] = sqrt(2.0 * F[i][j] * h * h * tan(alpha[i][j]));
                    //ни разу не выполняется
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    ST[i][j] = sqrt(2.0 * F[i][j] * h * h / tan(alpha[i][j]));
                    SL[i][j] = sqrt(2.0 * F[i][j] * h * h * tan(alpha[i][j]));

                    if(ST[i][j]>0 or SL[i][j]>0){
                        //выводит некорректное значение
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
                                          QPen(Qt::black), QBrush(Qt::red));
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
                                          QPen(Qt::red), QBrush(Qt::white));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    SL[i][j] = F[i][j] * h - 0.5 * h * tan(alpha[i][j]);
                    SR[i][j] = F[i][j] * h + 0.5 * h * tan(alpha[i][j]);

                    if (SL[i][j]>0 or SR[i][j]>0){
                        //ни разу не заходит в это условие
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    SL[i][j] = F[i][j] * h + 0.5 * h * tan(alpha[i][j]);
                    SR[i][j] = F[i][j] * h - 0.5 * h * tan(alpha[i][j]);
                    if (SL[i][j]>0 or SR[i][j]>0){
                        //ни разу не заходит в это условие
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
                                          QPen(Qt::red), QBrush(Qt::white));
                }
            }

            break;

            case 3:

                if ((Nx[i][j] < 0.0) && (Ny[i][j] >= 0.0))
                {
                    SB[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);

                    if(SB[i][j]>0 or ST[i][j]>0){
//                        qDebug()<<SB[i][j]<<ST[i][j];
                        QPolygonF polygon;
                        //ошибка в расчетах
                        qDebug()<<x[i][j]+h<<-y[i][j];
                        scene->addPolygon(polygon
                                          <<QPointF(ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(SB[i][j]+x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]),
                                          QPen(Qt::red), QBrush(Qt::black));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    SB[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    if(SB[i][j]>0 or ST[i][j]>0){
                        //не выполняется
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    SB[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    if(SB[i][j]>0 or ST[i][j]>0){
                        //не выполняется
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] >= 0.0))
                {
                    SB[i][j] = F[i][j] * h + 0.5 * h / tan(alpha[i][j]);
                    ST[i][j] = F[i][j] * h - 0.5 * h / tan(alpha[i][j]);

                    if(SB[i][j]>0 or ST[i][j]>0){
//                        qDebug()<<SB[i][j]<<ST[i][j];
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(SB[i][j]+x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]),
                                          QPen(Qt::red), QBrush(Qt::black));
                    }
                }

                break;

            case 4:

                if ((Nx[i][j] < 0.0) && (Ny[i][j] >= 0.0))
                {
                    ST[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    SL[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    if(h>ST[i][j]>0 || h>SL[i][j]>0){
                        //криво
//                        qDebug()<<x[i][j]<<-y[i][j]+h;
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j], -y[i][j])
                                          <<QPointF(ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(x[i][j]+h, -y[i][j])
                                          <<QPointF(x[i][j]+h, -y[i][j]+h)
                                          <<QPointF(x[i][j], -y[i][j]+h),
                                          QPen(Qt::red), QBrush(Qt::green));
                    }
                }

                if ((Nx[i][j] < 0.0) && (Ny[i][j] < 0.0))
                {
                    SL[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    SB[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    if(SL[i][j]>0 or SB[i][j]>0){
                        //не заходим
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] < 0.0))
                {
                    SB[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    SR[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    if(SR[i][j]>0 or SB[i][j]>0){
                        //сыпется NAN
//                        qDebug()<<SR[i][j]<<SB[i][j];
                    }
                }

                if ((Nx[i][j] >= 0.0) && (Ny[i][j] >= 0.0))
                {
                    SR[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h * tan(alpha[i][j]));
                    ST[i][j] = h - sqrt(2.0 * (1.0 - F[i][j]) * h * h / tan(alpha[i][j]));
                    if(h>SR[i][j]>0 or h>ST[i][j]>0){
//                        qDebug()<<SR[i][j]<<ST[i][j];
                        QPolygonF polygon;
                        scene->addPolygon(polygon
                                          <<QPointF(x[i][j]+h, SR[i][j]-y[i][j])
                                          <<QPointF(ST[i][j]+x[i][j], -y[i][j])
                                          <<QPointF(x[i][j], -y[i][j])
                                          <<QPointF(x[i][j], -y[i][j]+h)
                                          <<QPointF(x[i][j]+h, -y[i][j]+h),
                                          QPen(Qt::red), QBrush(Qt::green));
                    }
                }
                break;
        }
    }
}
//      int a=1;
//      int b=90;
//      qDebug()<<SB[a][b]<<SR[a][b]<<ST[a][b]<<SL[a][b];

//      for(int i=0;i<22; i++){
//          for(int j=0; j<102; j++){
//            if(SB[i][j]!=0 or ST[i][j]!=0 or SR[i][j]!=0 or SL[i][j]!=0){
//                qDebug()<<"i="<<i<<" "<<"j="<<j<<"SB="<<SB[i][j];
//                qDebug()<<"i="<<i<<" "<<"j="<<j<<"ST="<<ST[i][j];
//                qDebug()<<"i="<<i<<" "<<"j="<<j<<"SR="<<SR[i][j];
//                qDebug()<<"i="<<i<<" "<<"j="<<j<<"SL="<<SL[i][j];
//            }
//          }
//      }
}

MainWindow::~MainWindow()
{
    delete ui;
}

