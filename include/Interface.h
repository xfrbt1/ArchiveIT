#ifndef COURSE_PROJECT_INTERFACE_H
#define COURSE_PROJECT_INTERFACE_H


#include <QMainWindow>
#include <QToolBar>
#include <QVBoxLayout>
#include <QToolButton>
#include <QMessageBox>
#include <QLabel>
#include <QFileDialog>
#include <QMenu>

class Interface : public QMainWindow
{
Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr);
    ~Interface();

private:
    QWidget *centralWidget;
    QToolBar *toolBar;

    // Кнопки для навигации
    QToolButton *viewButton;
    QToolButton *compressButton;
    QToolButton *decompressButton;
    QToolButton *deleteButton;
    QToolButton *infoButton;

    QLabel *pathLabel;
    QLabel *algorithmMethodLabel;

    void setupUI();
    void setToolBarFont(QToolBar *toolBar);
    void setupNavigationButtons();
    void setupAlgorithmMenu();
    void setupLabels();
    void updateButtonStyles();

private slots:
    void selectPath();
};

#endif //COURSE_PROJECT_INTERFACE_H
