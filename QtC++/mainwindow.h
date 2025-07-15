#ifndef MAINWINDOW_COPY_H
#define MAINWINDOW_COPY_H

#include <QMainWindow>

class QImage;
class QSlider;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_clearButton_clicked();

    void on_flipButton_clicked();
    void on_grayButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage originalImage;

    void setEditedImage( const QImage& );
};
#endif // MAINWINDOW_COPY_H
