#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QTimer>

struct Puzzle {
    QString question;
    QString answer;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startGame();
    void exitGame();
    void onBoxClicked();
    void onDoorClicked();
    void onSubmitPuzzle();
    void updateTimer();
    void gameOver();

private:
    void setupInitialInterface();
    void setupRoom();
    void setupPuzzleInterface();
    void showPuzzle();
    void generatePuzzles(int numPuzzles = 16);
    void setupTimer();

    QPushButton *m_startButton;
    QPushButton *m_exitButton;
    QPushButton *m_boxKey;
    QPushButton *m_boxPuzzles;
    QPushButton *m_door;

    QLabel *m_puzzleLabel;
    QLineEdit *m_puzzleInput;
    QPushButton *m_submitPuzzle;

    QVector<Puzzle> m_allPuzzles;
    QVector<Puzzle> m_currentPuzzles;
    int m_solvedPuzzles;
    bool m_hasKey;

    QTimer *m_timer;
    QLabel *m_timeLabel;
    int m_remainingTime;
};

#endif
