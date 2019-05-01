#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QThread>

#include "ui_mainwindow.h"
#include "tile.h"
#include "player.h"
#include <vector>
#include <algorithm>

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

    std::vector<Tile*> p1AdjVec;
    std::vector<Tile*> p2AdjVec;
    bool tileInVectorP1;
    bool tileInVectorP2;


    int endVP;

    void createBoard();
    void tileResources(Player *p1, Player *p2);
    void getNames();
    void setEndVP();
    void takeTurn(Player *p1, Player *p2);
    void getAdjacentP1(int curLoc);
    void getAdjacentP2(int curLoc);
    void setResources(Player *p1, Player *p2, Tile * t);
    void buildCastle(Player *p1, Player *p2);
    void buyVPGold(Player *p1, Player *p2);
    void buyVPArmy(Player *p1, Player *p2);
    void checkWinner(Player *p1, Player *p2);
    void stepPlayer1(Player *p1, Player *p2);
    void stepPlayer2(Player *p1, Player *p2);

    void hideGraphics();
    void showLocEmojiP1(Player *p1);
    void showLocEmojiP2(Player *p2);
    void hideEmojiP1();
    void hideEmojiP2();

    void doBattle(Player *p1, Player *p2);
    void doBattleP1onP2(Player *p1, Player *p2);
    void doBattleP2onP1(Player *p1, Player *p2);
    void showAdjacentP1Battle(int curLoc);



public slots:
    void TileClickedSlot(Tile * t, Qt::MouseButton button);
    void TileShiftClickedSlot(Tile * t, Qt::MouseButton button);

private slots:
    void on_EndTurnButton_clicked();   // end turn
    void on_BuildCastleButton_clicked();
    void on_BuyVPGoldButton_clicked();
    void on_BuyVPArmyButton_clicked();
    void on_StepPlayer1Button_clicked();   // simulute P1 move
    void on_StepPlayer2Button_clicked();   // simulate P2 move
  //  void on_BattleButton_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
