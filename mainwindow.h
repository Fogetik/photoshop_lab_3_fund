#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QPainter>
#include <QPicture>
#include <cmath>
#include "histogramWid.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVBoxLayout* vbox_main;
    QHBoxLayout* hbox_up;
    QHBoxLayout* hbox_down;
    QFileDialog* FileD;

    QPushButton* btn_inv;
    QPushButton* btn_new;
    QPushButton* btn_default;
    QPushButton* btn_bw;
    QPushButton* btn_threshold;
    QPushButton* btn_log;
    QPushButton* btn_step;
    QPushButton* btn_equal;
    QPushButton* btn_transform;

    QPicture histogramPicture;

    QLabel *l;
    QImage *picture;

    int *backup_red;
    int *backup_blue;
    int *backup_green;

    bool bw = false;
    int *backup_u = nullptr, *backup_v = nullptr;
    QString way;

    int minLevel = 0, maxLevel = 255;

    QSlider* slBwMin;//black white
    QSlider* slBwMax;

    HistogramW *histogramW = nullptr;

    inline void updateHistogram();
    void makeBackup();
private slots:
    void new_picture();

    void pictureDefault();

    void inversia();

    void blackwhite();

    void pBwMin(int k);

    void pBwMax(int k);

    void threshold();

    void logFunc();

    void stepFunc();

    void equalizationFunc();

    void tranformHistogram();
};

#endif // MAINWINDOW_H
