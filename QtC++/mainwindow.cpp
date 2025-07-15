#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QMetaEnum>
#include <QTransform>
#include <QComboBox>

namespace {

    const QString               defaultDir = "D:/apps/Qt6/yc/images";   
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(1024, 800);
    ui->clearButton->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
    auto imagePath = QFileDialog::getOpenFileName(this, "Load Image", defaultDir, "Images (*.png *.jpg *.bmp *.jpeg)");
    if(imagePath.isEmpty())
    {
        QMessageBox::information(this,"NULL"," No Image is selected");
        return;
    }

    originalImage = QImage( imagePath );
    if ( originalImage.hasAlphaChannel() )
        originalImage = originalImage.convertToFormat( QImage::Format_ARGB32);
    else
        originalImage = originalImage.convertToFormat( QImage::Format_RGB32);

    //Show image info in info box
    QMetaEnum metaEnum = QMetaEnum::fromType<QImage::Format>();    
    QString msg = "Image size( " + QString::number(originalImage.width()) + ", " + QString::number(originalImage.height()) + " )\n";
    msg.append("Format: " + QString(metaEnum.valueToKey(originalImage.format())) + "\n");    
    ui->logInfo->setText( msg );

    const int labelw = ui->originalImg->width();
    const int labelh = ui->originalImg->height();
    ui->originalImg->setPixmap( QPixmap::fromImage( originalImage ).scaled(labelw, labelh) );

    ui->editedImg->setPixmap( QPixmap() ); //Empty edited if load a new image
}

void MainWindow::on_saveButton_clicked()
{
    if (!ui->editedImg->pixmap())
    {
        QMessageBox::warning(this, "Error", "No edited image to save.");
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(this, "Save Image", defaultDir, "Images (*.png *.jpg *.bmp *.jpeg)");
    if ( savePath.isEmpty() )
        return;

    QPixmap savePixmap = ui->editedImg->pixmap();
    if (savePixmap.save(savePath))
        QMessageBox::information(this, "Success", "Image saved successfully.");
    else
        QMessageBox::warning(this, "Error", "Failed to save the image.");
}


void MainWindow::on_clearButton_clicked()
{
    ui->editedImg->setPixmap( QPixmap() );
}

void MainWindow::setEditedImage( const QImage& image )
{
    const int labelw = ui->editedImg->width();
    const int labelh = ui->editedImg->height();
    ui->editedImg->setPixmap( QPixmap::fromImage( image ).scaled(labelw, labelh) );
}

void MainWindow::on_flipButton_clicked()
{
    QImage image = ui->editedImg->pixmap().isNull() ? originalImage.copy() : ui->editedImg->pixmap().toImage();
    if ( image.isNull() )
    {
        QMessageBox::warning(this, "Error", "Load a valid image first.");
        return;
    }

    QImage icopy = image.copy();

    const int height = image.height();
    const int width = image.width();
    long r_sum = 0, g_sum = 0, b_sum = 0;

    for (int y = 0; y < height; ++y)
    {
        QRgb *line = (QRgb*)icopy.scanLine(y);
        for (int x = 0; x < width; ++x )
        {
            image.setPixel( width-1-x, height-1-y, line[x] );

            r_sum += qRed( line[x] );
            g_sum += qGreen( line[x] );
            b_sum += qBlue( line[x] );
        }
    }

    setEditedImage( image );

    long r_avg = r_sum / (height * width);
    long g_avg = g_sum / (height * width);
    long b_avg = b_sum / (height * width);
    QString msg = "Pixel RGB chanels average are " + QString::number(r_avg) + ", " + QString::number(g_avg) + ", " + QString::number(b_avg);
    ui->logInfo->setText( msg );
}


void MainWindow::on_grayButton_clicked()
{
    QImage image =  ui->editedImg->pixmap().isNull() ? originalImage.copy() : ui->editedImg->pixmap().toImage();
    if ( image.isNull() )
    {
        QMessageBox::warning(this, "Error", "Load a valid image first.");
        return;
    }

    long sum = 0;
    for ( int h = 0; h < image.height(); ++h )
    {
        QRgb* pixel = (QRgb*)image.scanLine( h );
        QRgb* endp = pixel + image.width();
        int gray;
        for ( ; pixel != endp; pixel++ )
        {
            gray = qGray(*pixel);
            *pixel = qRgba(gray, gray, gray, qAlpha(*pixel));
            sum += gray;
        }
    }
    long avg = sum /(image.height() * image.width() );
    QString msg = "Gray scale image pixels average is " + QString::number(avg);
    ui->logInfo->setText( msg );

    //Method 0
    // for ( int h = 0; h < image.height(); ++h ) {
    //     uchar* scan = image.scanLine( h );
    //     int depth =4;
    //     for (int c = 0; c < image.width(); c++)
    //     {
    //         QRgb* rgbpixel = reinterpret_cast<QRgb*>(scan + c*depth);
    //         int gray = qGray(*rgbpixel);
    //         *rgbpixel = qRgba(gray, gray, gray, qAlpha(*rgbpixel));
    //     }
    // }

    /* //Method 1
    unsigned int *data = (unsigned int*)image.bits();
    int pixelCount = image.width() * image.height();

    // Convert each pixel to grayscale
    for(int i = 0; i < pixelCount; ++i) {
        int val = qGray(*data);
        *data = qRgba(val, val, val, qAlpha(*data));
        ++data;
    }*/

    setEditedImage( image );
}

