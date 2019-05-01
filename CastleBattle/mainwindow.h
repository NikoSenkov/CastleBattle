#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "ui_mainwindow.h"
#include "tile.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static const int x_dim = 4;
    static const int y_dim = 4;
    static const int totalTiles = x_dim * y_dim;

    int tileClickCount;

    Tile * board [x_dim][y_dim];        // game board
    Player *playerOne;
    Player *playerTwo;
    bool whoseTurn;

    void createBoard();
    void tileResources(Player *p1, Player *p2);
    void getNames();
    void takeTurn(Player *p1, Player *p2);
    void getAdjacentP1(int curLoc);
    void getAdjacentP2(int curLoc);
    void setResources(Player *p1, Player *p2, Tile * t);
    void buildCastle(Player *p1, Player *p2);
    void buyVPGold(Player *p1, Player *p2);
    void buyVPArmy(Player *p1, Player *p2);


public slots:
    void TileClickedSlot(Tile * t, Qt::MouseButton button);

private slots:
    void on_EndTurnButton_clicked();   // end turn
    void on_BuildCastleButton_clicked();
    void on_BuyVPGoldButton_clicked();
    void on_BuyVPArmyButton_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
