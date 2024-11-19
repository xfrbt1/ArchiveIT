#include "../include/Interface.h"

#define TOOLBAR_FONT 14


Interface::Interface(QWidget *parent) : QMainWindow(parent)
{
    // Создаем центральный виджет
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Инициализация и настройка интерфейса
    setupUI();
}


Interface::~Interface()
{
    // Удалять не нужно, так как используются родительские виджеты
}


void Interface::setupUI()
{
    // Создаем навигационную панель
    toolBar = addToolBar("Navigation");
    setToolBarFont(toolBar);

    // Добавляем кнопки на панель инструментов
    setupNavigationButtons();

    // Создаем метки для отображения пути и выбранного алгоритма
    setupLabels();

    // Основной вертикальный макет для центрального виджета
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(toolBar);           // Навигационная панель
    layout->addWidget(pathLabel);         // Метка пути
    layout->addWidget(algorithmMethodLabel); // Метка алгоритма
    layout->addStretch();
}


// Устанавливает шрифт для панели инструментов
void Interface::setToolBarFont(QToolBar *toolBar)
{
    QFont toolbarFont = toolBar->font();
    toolbarFont.setPointSize(TOOLBAR_FONT);
    toolBar->setFont(toolbarFont);
}


// Создает кнопки навигации и добавляет их на панель инструментов
void Interface::setupNavigationButtons()
{
    // Инициализация кнопок
    viewButton = new QToolButton(this);
    viewButton->setText("View");
    connect(viewButton, &QToolButton::clicked, this, &Interface::selectPath);
    toolBar->addWidget(viewButton);

    compressButton = new QToolButton(this);
    compressButton->setText("Compress");
    connect(compressButton, &QToolButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Сжать", "Запущен процесс сжатия!");
    });
    toolBar->addWidget(compressButton);

    decompressButton = new QToolButton(this);
    decompressButton->setText("Decompress");
    connect(decompressButton, &QToolButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Распаковать", "Запущен процесс распаковки!");
    });
    toolBar->addWidget(decompressButton);

    deleteButton = new QToolButton(this);
    deleteButton->setText("Remove");
    connect(deleteButton, &QToolButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Удалить", "Удаление файла или каталога!");
    });
    toolBar->addWidget(deleteButton);

    // Кнопка выбора алгоритма
    setupAlgorithmMenu();

    infoButton = new QToolButton(this);
    infoButton->setText("Info");
    connect(infoButton, &QToolButton::clicked, this, []() {
        QMessageBox::information(nullptr, "Info", "Информация о приложении.");
    });
    toolBar->addWidget(infoButton);
}


// Создает меню алгоритмов и добавляет его как кнопку на панель инструментов
void Interface::setupAlgorithmMenu()
{
    QToolButton *algorithmButton = new QToolButton(this);
    algorithmButton->setText("Algorithm");

    // Создаем меню и добавляем доступные алгоритмы
    QMenu *compressionMenu = new QMenu("Choose Algorithm", this);
    QStringList algorithms = {"LZ77", "LZ78", "HF"};
    for (const QString &alg : algorithms)
    {
        QAction *algorithmAction = compressionMenu->addAction(alg);
        connect(algorithmAction, &QAction::triggered, this, [this, alg]() {
            algorithmMethodLabel->setText("ALGORITHM: " + alg);
        });
    }

    // Настройка меню для кнопки
    algorithmButton->setMenu(compressionMenu);
    algorithmButton->setPopupMode(QToolButton::InstantPopup);

    // Вставляем кнопку с меню на панель инструментов перед кнопкой "Info"
    toolBar->addWidget(algorithmButton);
}


// Устанавливает метки пути и выбранного алгоритма
void Interface::setupLabels()
{
    pathLabel = new QLabel("PATH: ", this);
    pathLabel->setAlignment(Qt::AlignLeft);

    algorithmMethodLabel = new QLabel("ALGORITHM: ", this);
    algorithmMethodLabel->setAlignment(Qt::AlignLeft);
}


// Обрабатывает выбор пути (файла или каталога)
void Interface::selectPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setOption(QFileDialog::ShowDirsOnly, false);

    if (dialog.exec())
    {
        QStringList selectedFiles = dialog.selectedFiles();
        if (!selectedFiles.isEmpty())
        {
            QString selectedPath = selectedFiles.first();
            if (selectedPath.length() >= 50)
            {
                selectedPath = selectedPath.mid(0, 50) + "...";
            }
            pathLabel->setText("PATH: " + selectedPath);
        }
        else
        {
            pathLabel->setText("PATH: "); // Очищаем путь, если ничего не выбрано
        }
    }
    else
    {
        pathLabel->setText("PATH: "); // Очищаем путь, если диалог был отменен
    }

    updateButtonStyles();
}


// Обновляет стиль кнопок на панели инструментов
void Interface::updateButtonStyles()
{
    // Проверяем, выбран ли путь
    bool pathSelected = !pathLabel->text().isEmpty() && pathLabel->text() != "PATH: ";

    // Устанавливаем стиль для кнопок
    QString buttonStyle = pathSelected ? "background-color: green; color: white;" : "";

    // Применяем стиль к кнопкам
    compressButton->setStyleSheet(buttonStyle);
    decompressButton->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    infoButton->setStyleSheet(buttonStyle);
}
