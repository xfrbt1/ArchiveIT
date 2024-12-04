#include "Interface.h"


#define TOOLBAR_FONT 14


Interface::Interface(Adapter *adapter, QWidget *parent)
: QMainWindow(parent), adapter(adapter)
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


// Устанавливаем шрифт и начальные размеры кнопок в setupNavigationButtons
void Interface::setupNavigationButtons()
{
    QFont buttonFont;
    buttonFont.setPointSize(TOOLBAR_FONT); // Задаем размер шрифта для всех кнопок

    viewButton = new QToolButton(this);
    viewButton->setText("View");
    viewButton->setFont(buttonFont);
    connect(viewButton, &QToolButton::clicked, this, &Interface::selectPath);
    toolBar->addWidget(viewButton);

    compressButton = new QToolButton(this);
    compressButton->setText("Compress");
    compressButton->setFont(buttonFont);
    connect(compressButton, &QToolButton::clicked, this, &Interface::Compress);
    toolBar->addWidget(compressButton);

    decompressButton = new QToolButton(this);
    decompressButton->setText("Decompress");
    decompressButton->setFont(buttonFont);
    connect(decompressButton, &QToolButton::clicked, this, &Interface::Decompress);
    toolBar->addWidget(decompressButton);

    deleteButton = new QToolButton(this);
    deleteButton->setText("Remove");
    deleteButton->setFont(buttonFont);
    connect(deleteButton, &QToolButton::clicked, this, &Interface::removePathObject);
    toolBar->addWidget(deleteButton);

    // Кнопка выбора алгоритма
    setupAlgorithmMenu();
    infoButton = new QToolButton(this);
    infoButton->setText("Info");
    infoButton->setFont(buttonFont);
    connect(infoButton, &QToolButton::clicked, this, []()
    {
        QMessageBox::information(nullptr, "Info", "Информация о приложении.");
    });
    toolBar->addWidget(infoButton);
}


// Создает меню алгоритмов и добавляет его как кнопку на панель инструментов
void Interface::setupAlgorithmMenu()
{
    QToolButton *algorithmButton = new QToolButton(this);
    algorithmButton->setText("Algorithm");

    QFont buttonFont = algorithmButton->font();
    buttonFont.setPointSize(TOOLBAR_FONT);
    algorithmButton->setFont(buttonFont);

    QMenu *compressionMenu = new QMenu("Choose Algorithm", this);
    QStringList algorithms = {"huff", "lz77", "lz78"};
    for (const QString &alg : algorithms)
    {
        QAction *algorithmAction = compressionMenu->addAction(alg);
        connect(algorithmAction, &QAction::triggered, this, [this, alg]()
        {
            algorithmMethodLabel->setText("ALGORITHM: " + alg);
            adapter->setCompressMethod(alg.toStdString());
        });
    }

    algorithmButton->setMenu(compressionMenu);
    algorithmButton->setPopupMode(QToolButton::InstantPopup);

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
            std::string new_path = selectedPath.toStdString();
            adapter->updatePath(new_path);
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


void Interface::removePathObject()
{
    adapter->remove();
    pathLabel->setText("PATH: "); // Очищаем путь, если диалог был отменен
}


void Interface::Compress()
{
    adapter->createArchive();
}


void Interface::Decompress()
{
    adapter->unpackArchive();
}


// Обновляет стиль кнопок на панели инструментов
void Interface::updateButtonStyles()
{
    // Проверяем, выбран ли путь
    bool pathSelected = !pathLabel->text().isEmpty() && pathLabel->text() != "PATH: ";

    // Устанавливаем только цвет фона для активных кнопок
    QString activeStyle = "background-color: green; color: white;";
    QString defaultStyle = ""; // Сбрасываем стиль, если путь не выбран

    // Применяем стиль к кнопкам, не трогая шрифт и размер
    compressButton->setStyleSheet(pathSelected ? activeStyle : defaultStyle);
    decompressButton->setStyleSheet(pathSelected ? activeStyle : defaultStyle);
    deleteButton->setStyleSheet(pathSelected ? activeStyle : defaultStyle);
    infoButton->setStyleSheet(defaultStyle); // Информация всегда доступна
}
