#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QLineEdit>

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
    void onBoxClicked();
    void onDoorClicked();
    void onSubmitPuzzle();

private:
    void setupRoom();
    void setupPuzzleInterface();
    void showPuzzle();
    void generatePuzzles(int numPuzzles = 16); // Update 1: Added parameter to generatePuzzles

    QPushButton *m_boxKey;
    QPushButton *m_boxPuzzles;
    QPushButton *m_door;

    QLabel *m_puzzleLabel;
    QLineEdit *m_puzzleInput;
    QPushButton *m_submitPuzzle;

    QVector<Puzzle> m_allPuzzles; // Update 2: No change needed, already declared correctly
    QVector<Puzzle> m_currentPuzzles;
    int m_solvedPuzzles;
    bool m_hasKey;
};

#endif // MAINWINDOW_H

