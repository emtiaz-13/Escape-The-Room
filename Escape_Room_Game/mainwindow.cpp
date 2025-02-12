#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QApplication>
#include <algorithm>
#include <numeric>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_solvedPuzzles(0), m_hasKey(false)
{
    setupInitialInterface();
    setupRoom();
    setupPuzzleInterface();
    generatePuzzles(16);
    setupTimer();

    showFullScreen();
    setWindowTitle("Escape the Room");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupInitialInterface()
{
    setCentralWidget(new QWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget());

    m_startButton = new QPushButton("Start Game", this);
    m_exitButton = new QPushButton("Exit Game", this);
    connect(m_startButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(m_exitButton, &QPushButton::clicked, this, &MainWindow::exitGame);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(m_exitButton);

    mainLayout->addLayout(buttonLayout);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    pal.setColor(QPalette::ButtonText, Qt::white);
    pal.setColor(QPalette::Text, Qt::white);
    pal.setColor(QPalette::WindowText, Qt::white);
    setPalette(pal);

    m_startButton->setStyleSheet("background-color: #4CAF50; color: white; font-size: 18px; padding: 10px;");
    m_exitButton->setStyleSheet("background-color: #f44336; color: white; font-size: 18px; padding: 10px;");
}

void MainWindow::setupRoom()
{
    m_boxKey = new QPushButton("Box with Key", this);
    m_boxPuzzles = new QPushButton("Box with Riddles", this);
    m_door = new QPushButton("DOOR", this);

    m_boxKey->setFixedSize(150, 150);
    m_boxPuzzles->setFixedSize(150, 150);
    m_door->setFixedSize(200, 300);
    m_door->setStyleSheet("background-color: #8B4513; color: white; font-size: 20px; font-weight: bold;");   // change end

    connect(m_boxKey, &QPushButton::clicked, this, &MainWindow::onBoxClicked);
    connect(m_boxPuzzles, &QPushButton::clicked, this, &MainWindow::onBoxClicked);
    connect(m_door, &QPushButton::clicked, this, &MainWindow::onDoorClicked);

    m_boxKey->hide();
    m_boxPuzzles->hide();
    m_door->hide();

    m_boxKey->setStyleSheet("background-color: #2196F3; color: white; font-size: 16px;");
    m_boxPuzzles->setStyleSheet("background-color: #FF9800; color: white; font-size: 16px;");
    m_door->setStyleSheet("background-color: #8B4513; color: white; font-size: 20px; font-weight: bold;");

}

void MainWindow::setupPuzzleInterface()
{
    m_puzzleLabel = new QLabel(this);
    m_puzzleInput = new QLineEdit(this);
    m_submitPuzzle = new QPushButton("Submit Answer", this);

    connect(m_submitPuzzle, &QPushButton::clicked, this, &MainWindow::onSubmitPuzzle);


    m_puzzleLabel->hide();
    m_puzzleInput->hide();
    m_submitPuzzle->hide();

    m_puzzleLabel->setStyleSheet("color: white; font-size: 18px;");
    m_puzzleInput->setStyleSheet("background-color: white; color: black; font-size: 16px; padding: 5px;");
    m_submitPuzzle->setStyleSheet("background-color: #4CAF50; color: white; font-size: 16px; padding: 5px;");

}

void MainWindow::generatePuzzles(int numPuzzles)
{
    m_allPuzzles = {
        {"What has keys, but no locks; space, but no room; you can enter, but not go in?", "Keyboard"},
        {"I am not alive, but I grow; I don't have lungs, but I need air; I don't have a mouth, but water kills me. What am I?", "Fire"},
        {"What has cities, but no houses; forests, but no trees; and rivers, but no water?", "Map"},
        {"What starts with an E, ends with an E, but only contains one letter?", "Envelope"},
        {"I have branches, but no fruit, trunk or leaves. What am I?", "Bank"},
        {"What has a head and a tail that are only made of digits?", "Coin"},
        {"What has 13 hearts, but no other organs?", "Deck of cards"},
        {"What building has the most stories?", "Library"},
        {"I am taken from a mine and shut up in a wooden case, from which I am never released, and yet I am used by everyone. What am I?", "Pencil lead"},
        {"What comes once in a minute, twice in a moment, but never in a thousand years?", "The letter M"},
        {"What has legs, but doesn't walk?", "Table"},
        {"What can travel around the world while staying in a corner?", "Stamp"},
        {"I am always hungry; I must always be fed. The finger I touch, will soon turn red. What am I?", "Fire"},
        {"What gets wet while drying?", "Towel"},
        {"What has a neck but no head?", "Bottle"},
        {"What invention lets you look right through a wall?", "Window"}
    };

    std::random_shuffle(m_allPuzzles.begin(), m_allPuzzles.end());

    while (m_allPuzzles.size() > numPuzzles) {
        m_allPuzzles.removeLast();
    }
}

void MainWindow::startGame()
{
    m_startButton->hide();
    m_exitButton->hide();

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(centralWidget()->layout());
    QHBoxLayout *roomLayout = new QHBoxLayout;
    roomLayout->setAlignment(Qt::AlignCenter);
    roomLayout->addWidget(m_boxPuzzles);
    roomLayout->addWidget(m_door);
    roomLayout->addWidget(m_boxKey);

    mainLayout->addSpacing(20);
    mainLayout->addLayout(roomLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_puzzleLabel);
    mainLayout->addWidget(m_puzzleInput);
    mainLayout->addWidget(m_submitPuzzle);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_timeLabel);

    m_boxKey->show();
    m_boxPuzzles->show();
    m_door->show();

    m_remainingTime = 5 * 60;
    m_timer->start(1000);
    updateTimer();
}

void MainWindow::exitGame()
{
    QApplication::quit();
}

void MainWindow::onBoxClicked()
{
    QPushButton *clickedBox = qobject_cast<QPushButton*>(sender());

    if (clickedBox == m_boxKey) {
        if (m_solvedPuzzles < 3) {
            QMessageBox::information(this, "Box with Key", "You need to solve the riddles first!");
        } else {
            m_hasKey = true;
            QMessageBox::information(this, "Box with Key", "You found the key!");
        }
    } else if (clickedBox == m_boxPuzzles) {
        m_currentPuzzles.clear();
        QVector<int> indices(m_allPuzzles.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::random_shuffle(indices.begin(), indices.end());

        for (int i = 0; i < 3; ++i) {
            m_currentPuzzles.append(m_allPuzzles[indices[i]]);
        }

        m_solvedPuzzles = 0;
        showPuzzle();
    }
}

void MainWindow::onDoorClicked()
{
    if (m_hasKey) {
        m_timer->stop();
        QMessageBox::information(this, "Congratulations!", "You escaped the room with " + m_timeLabel->text() + " remaining!");
        QTimer::singleShot(0, this, &QWidget::close);
    } else {
        QMessageBox::information(this, "Door", "The door is locked. You need to find the key!");
    }
}

void MainWindow::showPuzzle()
{
    if (m_solvedPuzzles < 3) {
        m_puzzleLabel->setText(m_currentPuzzles[m_solvedPuzzles].question);
        m_puzzleLabel->show();
        m_puzzleInput->show();
        m_submitPuzzle->show();
        m_puzzleInput->clear();
        m_puzzleInput->setFocus();
    } else {
        m_puzzleLabel->hide();
        m_puzzleInput->hide();
        m_submitPuzzle->hide();
        QMessageBox::information(this, "Riddles Completed", "You have solved all the riddles. You can now get the key!");
    }
}

void MainWindow::onSubmitPuzzle()
{
    QString userAnswer = m_puzzleInput->text().trimmed().toLower();
    QString correctAnswer = m_currentPuzzles[m_solvedPuzzles].answer.toLower();

    if (userAnswer == correctAnswer) {
        m_solvedPuzzles++;
        if (m_solvedPuzzles < 3) {
            QMessageBox::information(this, "Correct", "That's correct! Here's the next riddle.");
        }
        showPuzzle();
    } else {
        QMessageBox::warning(this, "Incorrect", "Sorry, that's not the correct answer. Try again!");
    }
}

void MainWindow::setupTimer()
{
    m_remainingTime = 5 * 60;
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    m_timeLabel = new QLabel(this);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setStyleSheet("font-size: 24px; color: white; background-color: black; padding: 10px;");
    updateTimer();
}

void MainWindow::updateTimer()
{
    if (m_remainingTime > 0) {
        m_remainingTime--;
        int minutes = m_remainingTime / 60;
        int seconds = m_remainingTime % 60;
        m_timeLabel->setText(QString("Time left: %1:%2")
                                 .arg(minutes, 2, 10, QChar('0'))
                                 .arg(seconds, 2, 10, QChar('0')));
    } else {
        gameOver();
    }
}

void MainWindow::gameOver()
{
    m_timer->stop();
    QMessageBox::warning(this, "Game Over", "Time's up! You couldn't escape the room in time.");
    QTimer::singleShot(0, this, &QWidget::close);
}
