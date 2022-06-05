#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *wid = new QWidget(this);

    this->resize(800, 600);

    this->setCentralWidget(wid);
    this->vbox_main = new QVBoxLayout();
    this->hbox_up = new QHBoxLayout();

    this->picture = new QImage();

    wid->setLayout(vbox_main);

    vbox_main->addLayout(hbox_up);

    this->FileD = new QFileDialog();

    l = new QLabel;

    hbox_down = new QHBoxLayout();

    this->vbox_main->addLayout(hbox_down);
    this->hbox_down->addWidget(l);

    this->btn_new= new QPushButton();
    btn_new->setText("New");
    this->hbox_up->addWidget(btn_new);
    connect(btn_new, SIGNAL(clicked()), SLOT(new_picture()));

    this->btn_default = new QPushButton();
    btn_default->setText("Default");
    this->hbox_up->addWidget(btn_default);
    connect(btn_default, SIGNAL(clicked()), SLOT(pictureDefault()));

    this->btn_inv = new QPushButton();
    btn_inv->setText("Inversion");
    this->hbox_up->addWidget(btn_inv);
    connect(btn_inv, SIGNAL(clicked()), SLOT(inversia()));

    this->btn_bw = new QPushButton();
    btn_bw->setText("BlackWhite");
    this->hbox_up->addWidget(btn_bw);
    connect(btn_bw, SIGNAL(clicked()), SLOT(blackwhite()));

    this->btn_threshold = new QPushButton();
    btn_threshold->setText("Threshold");
    this->hbox_up->addWidget(btn_threshold);
    connect(btn_threshold, SIGNAL(clicked()), SLOT(threshold()));

    this->btn_log = new QPushButton();
    btn_log->setText("Log");
    this->hbox_up->addWidget(btn_log);
    connect(btn_log, SIGNAL(clicked()), SLOT(logFunc()));

    this->btn_step = new QPushButton();
    btn_step->setText("Degree");
    this->hbox_up->addWidget(btn_step);
    connect(btn_step, SIGNAL(clicked()), SLOT(stepFunc()));

    this->btn_equal= new QPushButton();
    btn_equal->setText("Equaliz");
    this->hbox_up->addWidget(btn_equal);
    connect(btn_equal, SIGNAL(clicked()), SLOT(equalizationFunc()));

    this->btn_transform= new QPushButton();
    btn_transform->setText("Transform");
    this->hbox_up->addWidget(btn_transform);
    connect(btn_transform, SIGNAL(clicked()), SLOT(tranformHistogram()));

    this->slBwMin = new QSlider();
    this->hbox_up->addWidget(slBwMin);
    slBwMin->setRange(0, 255);
    this->slBwMin->setValue(0);
    this->slBwMin->setOrientation(Qt::Orientation::Horizontal);
    connect(this->slBwMin, SIGNAL(valueChanged(int)), this, SLOT(pBwMin(int)));

    this->slBwMax = new QSlider();
    this->hbox_up->addWidget(slBwMax);
    slBwMax->setRange(0, 255);
    this->slBwMax->setValue(255);
    this->slBwMax->setOrientation(Qt::Orientation::Horizontal);
    connect(this->slBwMax, SIGNAL(valueChanged(int)), this, SLOT(pBwMax(int)));
}

inline void MainWindow::updateHistogram(){
    this->histogramW->repaint();
}

void MainWindow::new_picture(){
    QString line = this->FileD->getOpenFileName();
    this->bw = false;
    this->way = line;
    this->picture->load(line);

    backup_red = new int[this->picture->width() * this->picture->height()];
    backup_green = new int[this->picture->width() * this->picture->height()];
    backup_blue = new int[this->picture->width() * this->picture->height()];

    l->setPixmap(QPixmap::fromImage(*this->picture));
    if (histogramW == nullptr)
    {
        histogramW = new HistogramW(this->picture);
        this->hbox_down->addWidget(histogramW);
    }
    else
        this->histogramW->repaint();

}

void MainWindow::pictureDefault(){
    minLevel = 0;
    maxLevel = 255;
    this->picture->load(way);
    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
    this->slBwMax->setValue(255);
    this->slBwMin->setValue(0);
}

void MainWindow::inversia(){
    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        *bite = 255 - *bite;
        bite++;
        *bite = 255 - *bite;
        bite++;
        *bite = 255 - *bite;
        bite+=2;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();

}

void MainWindow::blackwhite(){
    int y, r, g, b;
    if (backup_u == nullptr)
    {
        backup_u = new int [this->picture->width() * this->picture->height()];
        backup_v = new int [this->picture->width() * this->picture->height()];
    }
    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        if (bw)
        {
            y = *bite;
            r = y + 1.2803*backup_v[i];
            g = y - 0.2148*backup_u[i] - 0.3805*backup_v[i];
            b = y + 2.1279*backup_u[i];

            *bite++ = r;
            *bite++ = g;
            *bite++ = b;
        }
        else
        {
            r = *bite++;
            g = *bite++;
            b = *bite;
            bite-=2;
            y = 0.2126*r + 0.7156*g + 0.0722*b;
            backup_u[i] = -0.0999*r - 0.3360*g + 0.4360*b;
            backup_v[i] = 0.6150*r - 0.5586*g - 0.0563*b;

            r = y;
            g = y;
            b = y;

            *bite++ = r;
            *bite++ = g;
            *bite++ = b;
        }

        bite++;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
    bw = !bw;
}

void MainWindow::pBwMin(int k){
    double y;
    int r, g, b;
    if (k < minLevel)
        return;
    else
        minLevel = k;

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        bite-=2;

        y = 0.2126*r + 0.7156*g + 0.0722*b;

        if (y < k){
            *bite++ = 0;
            *bite++ = 0;
            *bite++ = 0;
            bite++;
        }else
            bite += 4;

    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
}

void MainWindow::pBwMax(int k){
    double y, u, v;
    int r, g, b;
    if (k > maxLevel)
        return;
    else
        maxLevel = k;

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        bite-=2;

        y = 0.2126*r + 0.7156*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        if (y > k){
            *bite++ = 255;
            *bite++ = 255;
            *bite++ = 255;
            bite++;
        }else
            bite += 4;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
}

void MainWindow::threshold(){
    int k = 128; //threshold
    double y, u, v;
    int r, g, b;

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        bite-=2;

        y = 0.2126*r + 0.7156*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        (y < k) ? y = 0 : y = 255;

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;

        *bite++ = r;
        *bite++ = g;
        *bite++ = b;
        bite++;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
}

void MainWindow::logFunc(){
    int c = 1;
    double y, u, v;
    int r, g, b;

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        bite-=2;

        y = 0.2126*r + 0.7156*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        y = (int)(255*c*log2(1 + (double)y/(double)255));

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;


        *bite++ = r;
        *bite++ = g;
        *bite++ = b;
        bite++;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
}

void MainWindow::stepFunc(){
    int c = 1;
    double y, u, v;
    int r, g, b;
    int r_old, r_new;
    double gamma = 0.4;

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        bite-=2;
        r_old = r;
        y = 0.2126*r + 0.7156*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        y = (int)(c * pow(((double)y/255), gamma)*255);

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;
        r_new = r;

//        if (abs(r_new - r_old) > 10)
//            std::cout << "log" << std::endl;

        *bite++ = r;
        *bite++ = g;
        *bite++ = b;
        bite++;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();
}

void MainWindow::equalizationFunc(){
    int y, *u, *v, r, g, b;
    int brightCount[256], summArr[256];
    u = new int[this->picture->width() * this->picture->height()];
    v = new int[this->picture->width() * this->picture->height()];
    memset(brightCount, 0, sizeof(brightCount));
    memset(summArr, 0, sizeof(summArr));

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        y = 0.2126*r + 0.7156*g + 0.0722*b;
        u[i] = -0.0999*r - 0.3360*g + 0.4360*b;
        v[i] = 0.6150*r - 0.5586*g - 0.0563*b;
        bite-=2;
        *bite = y;
        bite++;
        bite++;
        bite++;
        bite++;
        brightCount[y]++;
    }

    summArr[0] = brightCount[0];
    for (int i = 1; i < 256; i++)
        summArr[i] = summArr[i-1] + brightCount[i];
    l->setPixmap(QPixmap::fromImage(*this->picture));

    bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        y = *bite;
        y = (int)(255 * ((double)summArr[y] / (summArr[255])));
        r = y + 1.2803*v[i];
        g = y - 0.2148*u[i] - 0.3805*v[i];
        b = y + 2.1279*u[i];

        *bite++ = r;
        *bite++ = g;
        *bite++ = b;
        bite++;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();

}

void MainWindow::tranformHistogram()
{
    int y, *u, *v, r, g, b;
    int D,M;
    D = 30;
    M = 50;
    double sko = sqrt(D);
    int brightCount[256], summArr[256];
    u = new int[this->picture->width() * this->picture->height()];
    v = new int[this->picture->width() * this->picture->height()];
    memset(brightCount, 0, sizeof(brightCount));
    memset(summArr, 0, sizeof(summArr));

    uchar* bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        r = *bite++;
        g = *bite++;
        b = *bite;
        y = 0.2126*r + 0.7156*g + 0.0722*b;
        u[i] = -0.0999*r - 0.3360*g + 0.4360*b;
        v[i] = 0.6150*r - 0.5586*g - 0.0563*b;
        bite-=2;
        *bite = y;
        bite++;
        bite++;
        bite++;
        bite++;
        brightCount[y]++;
    }

    summArr[0] = brightCount[0];
    for (int i = 1; i < 256; i++)
        summArr[i] = summArr[i-1] + brightCount[i];
    l->setPixmap(QPixmap::fromImage(*this->picture));

    bite = this->picture->bits();
    for (int i = 0; i < this->picture->width() * this->picture->height(); i++)
    {
        y = *bite;
        y = (int)(255 * ((double)summArr[y] / (summArr[255])));
        y = M - sqrt(2 * sko*sko*log(y*sko*sqrt(2*M_PI)));

        r = y + 1.2803*v[i];
        g = y - 0.2148*u[i] - 0.3805*v[i];
        b = y + 2.1279*u[i];

        *bite++ = r;
        *bite++ = g;
        *bite++ = b;
        bite++;
    }

    l->setPixmap(QPixmap::fromImage(*this->picture));
    this->updateHistogram();

}

MainWindow::~MainWindow()
{
    delete ui;
}
