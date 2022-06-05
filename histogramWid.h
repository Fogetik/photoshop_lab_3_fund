#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <cstring>
#include <QDebug>
#include <QPainter>
#include <QPicture>
#include <cmath>


class HistogramW : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramW(QImage* cop){
        this->image = new QImage();
        this->image = cop;
        this->picture = new QPicture();
    }

    ~HistogramW(){}
private:
    QImage *image;

    QPicture* picture;

protected:
    void paintEvent(QPaintEvent *);
private slots:

};

#endif // IMAGEWIDGET_H
