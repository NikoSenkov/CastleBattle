#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

#include <QMessageBox>
#include <QInputDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tile.h"
#include "player.h"
#include "castle.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // setup ui
    ui->setupUi(this);

    QGraphicsView * view = ui->graphicsView;

        // scene is a pointer field of main window
    scene = new QGraphicsScene;

    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(), view->frameSize().height());
    connect(ui->BuildCastle, &QAbstractButton::clicked, this, &MainWindow::on_BuildCastleButton_clicked);

    srand(time(0));

    int boardSize = 4;          // 6 tiles on each side

    for(int i = 0; i < x_dim; i++){                 // initialize board
        for(int j = 0; j < y_dim; j++){
            board[i][j] = nullptr;
        }
    }

    int x = 0;          // width and height
    int y = 0;

    int count = 0;
    for (int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
              count++;
              QColor white (255, 255, 255);
              Tile * t = new Tile(white, x, y, view->frameSize().width()/boardSize, view->frameSize().height()/boardSize);
              board[i][j] = t;
              scene->addItem(t);
              connect(t, &Tile::TileClicked, this, &MainWindow::TileClickedSlot);
              connect(t, &Tile::TileShiftClicked, this, &MainWindow::TileShiftClickedSlot);
              t->tileAlias += count;                                    // set tile alias (1-16)
              x += view->frameSize().width() / boardSize;
        }
        y += view->frameSize().height() / boardSize;
        x = 0;
    }

    Player * p1 =  new Player(100,100,100,13, 0);       // create player objects
    Player * p2 = new Player(100,100,100,4, 0);

    playerOne = p1;
    playerTwo = p2;
    tileInVectorP1 = false;
    tileInVectorP2 = false;

    tileClickCount = 0;

    // program flow

    hideGraphics();

    tileResources(playerOne, playerTwo);
   // getNames();
    setEndVP();
    whoseTurn = true;
    takeTurn(playerOne, playerTwo);

}

void MainWindow::getNames(){

    QString player1Name;
    QString player2Name;

    bool ok;
     // Ask for name as a string.
     QString text = QInputDialog::getText(nullptr, "Input dialog",
                                          "Player name:", QLineEdit::Normal,
                                          "", &ok);
     if (ok && !text.isEmpty()) {
         player1Name = text;
         QMessageBox::information (nullptr, "Name",
                                   QString("Welcome to CastleBattle %1").arg(player1Name));
    }

     bool ok2;
      // Ask for name as a string.
         QString text2 = QInputDialog::getText(nullptr, "Input dialog",
                                           "Player name:", QLineEdit::Normal,
                                           "", &ok2);
      if (ok2 && !text.isEmpty()) {
          player2Name = text2;
          QMessageBox::information (nullptr, "Name",
                                    QString("Welcome to CastleBattle %1").arg(player2Name));
     }

     ui->Player1Title->setText(player1Name);
     ui->Player2Title->setText(player2Name);
}

void MainWindow::setEndVP(){

    bool ok3;

    int finalVP = 0;

    QString text = QInputDialog::getText(nullptr, "Input dialog",
                                         "Victory Points to Win:", QLineEdit::Normal,
                                         "", &ok3);
    if (ok3 && !text.isEmpty()) {
        finalVP = text.toInt();
        QMessageBox::information (nullptr, "The Age",
                                  QString("Victory Points to Win: %1").arg(finalVP));
   }

    endVP = finalVP;

}


// Light up available tiles around player one's current position
void MainWindow::getAdjacentP1(int curLoc){
    qDebug() << "current location: " << curLoc;

    QColor lightGreen(128, 247, 132);

    if(curLoc == 1){                                //1
            if(board[0][1]->owner == 0) board[0][1]->color_ = lightGreen;
            if(board[1][0]->owner == 0) board[1][0]->color_ = lightGreen;
            if(board[1][1]->owner == 0) board[1][1]->color_ = lightGreen;
            scene->update();
    }

    if(curLoc == 2){
            if(board[0][0]->owner == 0) board[0][0]->color_ = lightGreen;
            if(board[0][2]->owner == 0) board[0][2]->color_ = lightGreen;
            if(board[1][0]->owner == 0) board[1][0]->color_ = lightGreen;
            if(board[1][1]->owner == 0) board[1][1]->color_ = lightGreen;
            if(board[1][2]->owner == 0) board[1][2]->color_ = lightGreen;
            scene->update();
    }

    if(curLoc == 3){
       if(board[0][1]->owner == 0)   board[0][1]->color_ = lightGreen;
       if(board[1][1]->owner == 0)   board[1][1]->color_ = lightGreen;
       if(board[1][2]->owner == 0)   board[1][2]->color_ = lightGreen;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightGreen;
       if(board[0][3]->owner == 0)  board[0][3]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 4){                            //4
       if(board[0][2]->owner == 0)  board[0][2]->color_ = lightGreen;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightGreen;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 5){                                //5
            if(board[0][0]->owner == 0) board[0][0]->color_ = lightGreen;
            if(board[0][1]->owner == 0) board[0][1]->color_ = lightGreen;
            if(board[1][1]->owner == 0) board[1][1]->color_ = lightGreen;
            if(board[2][0]->owner == 0) board[2][0]->color_ = lightGreen;
            if(board[2][1]->owner == 0) board[2][1]->color_ = lightGreen;
            scene->update();
    }

    if(curLoc == 6){
            if(board[0][0]->owner == 0) board[0][0]->color_ = lightGreen;
            if(board[0][1]->owner == 0) board[0][1]->color_ = lightGreen;
            if(board[0][2]->owner == 0) board[0][2]->color_ = lightGreen;
            if(board[1][0]->owner == 0) board[1][0]->color_ = lightGreen;
            if(board[2][0]->owner == 0) board[2][0]->color_ = lightGreen;
            if(board[2][1]->owner == 0) board[2][1]->color_ = lightGreen;
            if(board[1][2]->owner == 0) board[1][2]->color_ = lightGreen;
            if(board[2][2]->owner == 0) board[2][2]->color_ = lightGreen;
            scene->update();
    }

    if(curLoc == 7){
       if(board[0][1]->owner == 0)  board[0][1]->color_ = lightGreen;
       if(board[0][2]->owner == 0)  board[0][2]->color_ = lightGreen;
       if(board[0][3]->owner == 0)  board[0][3]->color_ = lightGreen;
       if(board[1][1]->owner == 0)  board[1][1]->color_ = lightGreen;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightGreen;
       if(board[2][1]->owner == 0)  board[2][1]->color_ = lightGreen;
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightGreen;
       scene->update();
    }

    if(curLoc == 8){                            //8
       if(board[0][2]->owner == 0)  board[0][2]->color_ = lightGreen;
       if(board[0][3]->owner == 0)  board[0][3]->color_ = lightGreen;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightGreen;
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightGreen;
       scene->update();
    }

    if(curLoc == 9){                            //9
        if(board[1][0]->owner == 0)  board[1][0]->color_ = lightGreen;
        if(board[1][1]->owner == 0)  board[1][1]->color_ = lightGreen;
        if(board[2][1]->owner == 0)  board[2][1]->color_ = lightGreen;
        if(board[3][0]->owner == 0)  board[3][0]->color_ = lightGreen;
        if(board[3][1]->owner == 0)  board[3][1]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 10){       //10
       if(board[1][0]->owner == 0)  board[1][0]->color_ = lightGreen;
       if(board[1][1]->owner == 0)  board[1][1]->color_ = lightGreen;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightGreen;
       if(board[2][0]->owner == 0)  board[2][0]->color_ = lightGreen;
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
       if(board[3][0]->owner == 0)  board[3][0]->color_ = lightGreen;
       if(board[3][1]->owner == 0)  board[3][1]->color_ = lightGreen;
       if(board[3][2]->owner == 0)  board[3][2]->color_ = lightGreen;
       scene->update();
    }

    if(curLoc == 11){       //11
       if(board[1][1]->owner == 0)  board[1][1]->color_ = lightGreen;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightGreen;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightGreen;
       if(board[2][1]->owner == 0)  board[2][1]->color_ = lightGreen;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightGreen;
       if(board[3][1]->owner == 0)  board[3][1]->color_ = lightGreen;
       if(board[3][2]->owner == 0)  board[3][2]->color_ = lightGreen;
       if(board[3][3]->owner == 0)  board[3][3]->color_ = lightGreen;
       scene->update();
    }

    if(curLoc == 12){                            //12
        if(board[1][2]->owner == 0)  board[1][2]->color_ = lightGreen;
        if(board[1][3]->owner == 0)  board[1][3]->color_ = lightGreen;
        if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
        if(board[3][2]->owner == 0)  board[3][2]->color_ = lightGreen;
        if(board[3][3]->owner == 0)  board[3][3]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 13){                            //13
       if(board[2][0]->owner == 0)  board[2][0]->color_ = lightGreen;
       if(board[2][1]->owner == 0)  board[2][1]->color_ = lightGreen;
       if(board[3][1]->owner == 0)  board[3][1]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 14){                          //14
        if(board[2][0]->owner == 0)  board[2][0]->color_ = lightGreen;
        if(board[2][1]->owner == 0)  board[2][1]->color_ = lightGreen;
        if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
        if(board[3][0]->owner == 0)  board[3][0]->color_ = lightGreen;
        if(board[3][2]->owner == 0)  board[3][2]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 15){                          //15
        if(board[2][1]->owner == 0)  board[2][1]->color_ = lightGreen;
        if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
        if(board[2][3]->owner == 0)  board[2][3]->color_ = lightGreen;
        if(board[3][1]->owner == 0)  board[3][1]->color_ = lightGreen;
        if(board[3][3]->owner == 0)  board[3][3]->color_ = lightGreen;
        scene->update();
    }

    if(curLoc == 16){                            //16
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightGreen;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightGreen;
       if(board[3][2]->owner == 0)  board[3][2]->color_ = lightGreen;
        scene->update();
    }

  /*  for (int i = 0; i < x_dim; i++){                // this may cause problems .. look here first if there's a bug!
        for (int j = 0; j < y_dim; j++){
            if(board[i][j]->color_ == lightGreen) { p1AdjVec.push_back(board[i][j]); }
        }
    } */

}

// Light up available tiles around player 2's current position
void MainWindow::getAdjacentP2(int curLoc){
    qDebug() << "current location: " << curLoc;

    QColor lightRed(244, 151, 170);

    if(curLoc == 1){                                //1
            if(board[0][1]->owner == 0) board[0][1]->color_ = lightRed;
            if(board[1][0]->owner == 0) board[1][0]->color_ = lightRed;
            if(board[1][1]->owner == 0) board[1][1]->color_ = lightRed;
            scene->update();
    }

    if(curLoc == 2){
            if(board[0][0]->owner == 0) board[0][0]->color_ = lightRed;
            if(board[0][2]->owner == 0) board[0][2]->color_ = lightRed;
            if(board[1][0]->owner == 0) board[1][0]->color_ = lightRed;
            if(board[1][1]->owner == 0) board[1][1]->color_ = lightRed;
            if(board[1][2]->owner == 0) board[1][2]->color_ = lightRed;
            scene->update();
    }

    if(curLoc == 3){
       if(board[0][1]->owner == 0)   board[0][1]->color_ = lightRed;
       if(board[1][1]->owner == 0)   board[1][1]->color_ = lightRed;
       if(board[1][2]->owner == 0)   board[1][2]->color_ = lightRed;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightRed;
       if(board[0][3]->owner == 0)  board[0][3]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 4){                            //4
       if(board[0][2]->owner == 0)  board[0][2]->color_ = lightRed;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightRed;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 5){                                //5
            if(board[0][0]->owner == 0) board[0][0]->color_ = lightRed;
            if(board[0][1]->owner == 0) board[0][1]->color_ = lightRed;
            if(board[1][1]->owner == 0) board[1][1]->color_ = lightRed;
            if(board[2][0]->owner == 0) board[2][0]->color_ = lightRed;
            if(board[2][1]->owner == 0) board[2][1]->color_ = lightRed;
            scene->update();
    }

    if(curLoc == 6){
            if(board[0][0]->owner == 0) board[0][0]->color_ = lightRed;
            if(board[0][1]->owner == 0) board[0][1]->color_ = lightRed;
            if(board[0][2]->owner == 0) board[0][2]->color_ = lightRed;
            if(board[1][0]->owner == 0) board[1][0]->color_ = lightRed;
            if(board[2][0]->owner == 0) board[2][0]->color_ = lightRed;
            if(board[2][1]->owner == 0) board[2][1]->color_ = lightRed;
            if(board[1][2]->owner == 0) board[1][2]->color_ = lightRed;
            if(board[2][2]->owner == 0) board[2][2]->color_ = lightRed;
            scene->update();
    }

    if(curLoc == 7){
       if(board[0][1]->owner == 0)  board[0][1]->color_ = lightRed;
       if(board[0][2]->owner == 0)  board[0][2]->color_ = lightRed;
       if(board[0][3]->owner == 0)  board[0][3]->color_ = lightRed;
       if(board[1][1]->owner == 0)  board[1][1]->color_ = lightRed;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightRed;
       if(board[2][1]->owner == 0)  board[2][1]->color_ = lightRed;
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightRed;
       scene->update();
    }

    if(curLoc == 8){                            //8
       if(board[0][2]->owner == 0)  board[0][2]->color_ = lightRed;
       if(board[0][3]->owner == 0)  board[0][3]->color_ = lightRed;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightRed;
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightRed;
       scene->update();
    }

    if(curLoc == 9){                            //9
        if(board[1][0]->owner == 0)  board[1][0]->color_ = lightRed;
        if(board[1][1]->owner == 0)  board[1][1]->color_ = lightRed;
        if(board[2][1]->owner == 0)  board[2][1]->color_ = lightRed;
        if(board[3][0]->owner == 0)  board[3][0]->color_ = lightRed;
        if(board[3][1]->owner == 0)  board[3][1]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 10){       //10
       if(board[1][0]->owner == 0)  board[1][0]->color_ = lightRed;
       if(board[1][1]->owner == 0)  board[1][1]->color_ = lightRed;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightRed;
       if(board[2][0]->owner == 0)  board[2][0]->color_ = lightRed;
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
       if(board[3][0]->owner == 0)  board[3][0]->color_ = lightRed;
       if(board[3][1]->owner == 0)  board[3][1]->color_ = lightRed;
       if(board[3][2]->owner == 0)  board[3][2]->color_ = lightRed;
       scene->update();
    }

    if(curLoc == 11){       //11
       if(board[1][1]->owner == 0)  board[1][1]->color_ = lightRed;
       if(board[1][2]->owner == 0)  board[1][2]->color_ = lightRed;
       if(board[1][3]->owner == 0)  board[1][3]->color_ = lightRed;
       if(board[2][1]->owner == 0)  board[2][1]->color_ = lightRed;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightRed;
       if(board[3][1]->owner == 0)  board[3][1]->color_ = lightRed;
       if(board[3][2]->owner == 0)  board[3][2]->color_ = lightRed;
       if(board[3][3]->owner == 0)  board[3][3]->color_ = lightRed;
       scene->update();
    }

    if(curLoc == 12){                            //12
        if(board[1][2]->owner == 0)  board[1][2]->color_ = lightRed;
        if(board[1][3]->owner == 0)  board[1][3]->color_ = lightRed;
        if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
        if(board[3][2]->owner == 0)  board[3][2]->color_ = lightRed;
        if(board[3][3]->owner == 0)  board[3][3]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 13){                            //13
       if(board[2][0]->owner == 0)  board[2][0]->color_ = lightRed;
       if(board[2][1]->owner == 0)  board[2][1]->color_ = lightRed;
       if(board[3][1]->owner == 0)  board[3][1]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 14){                          //14
        if(board[2][0]->owner == 0)  board[2][0]->color_ = lightRed;
        if(board[2][1]->owner == 0)  board[2][1]->color_ = lightRed;
        if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
        if(board[3][0]->owner == 0)  board[3][0]->color_ = lightRed;
        if(board[3][2]->owner == 0)  board[3][2]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 15){                          //15
        if(board[2][1]->owner == 0)  board[2][1]->color_ = lightRed;
        if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
        if(board[2][3]->owner == 0)  board[2][3]->color_ = lightRed;
        if(board[3][1]->owner == 0)  board[3][1]->color_ = lightRed;
        if(board[3][3]->owner == 0)  board[3][3]->color_ = lightRed;
        scene->update();
    }

    if(curLoc == 16){                            //16
       if(board[2][2]->owner == 0)  board[2][2]->color_ = lightRed;
       if(board[2][3]->owner == 0)  board[2][3]->color_ = lightRed;
       if(board[3][2]->owner == 0)  board[3][2]->color_ = lightRed;
        scene->update();
    }

 /*   for (int i = 0; i < x_dim; i++){                // this may cause problems .. look here first if there's a bug!
        for (int j = 0; j < y_dim; j++){
            if(board[i][j]->color_ == lightRed) { p2AdjVec.push_back(board[i][j]); }
        }
    } */

}

// Light up player's owned tiles
void MainWindow::takeTurn(Player *p1, Player *p2){

    QColor Green(37, 165, 28);
    QColor Red(255, 0, 51);

    for (int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
           if(board[i][j]->owner == 1) board[i][j]->color_ = Green;
           else if (board[i][j]->owner == 2) board[i][j]->color_ = Red;
        }
     }

    if (whoseTurn){

        getAdjacentP1(p1->curLoc);


    }

    else {

        getAdjacentP2(p2->curLoc);

   }

}

// Sets game board's tile resources
void MainWindow::tileResources(Player *p1, Player *p2){

    // set tile resources between 50 and 100
    for (int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
            board[i][j]->stone = rand() % 50 + 50;
            board[i][j]->army = rand() % 50 + 50;
            board[i][j]->gold = rand() % 50 + 50;
        }
    }

    // set starting tile resources to 100
    board[0][3]->stone = 100;
    board[0][3]->army = 100;
    board[0][3]->gold = 100;

    board[3][0]->stone = 100;
    board[3][0]->army = 100;
    board[3][0]->gold = 100;


    ui->Tile1_Stone->setText(QString::number(board[0][0]->stone));      //1
    ui->Tile1_Army->setText(QString::number(board[0][0]->army));
    ui->Tile1_Gold->setText(QString::number(board[0][0]->gold));

    ui->Tile2_Stone->setText(QString::number(board[0][1]->stone));      //2
    ui->Tile2_Army->setText(QString::number(board[0][1]->army));
    ui->Tile2_Gold->setText(QString::number(board[0][1]->gold));

    ui->Tile3_Stone->setText(QString::number(board[0][2]->stone));  //3
    ui->Tile3_Army->setText(QString::number(board[0][2]->army));
    ui->Tile3_Gold->setText(QString::number(board[0][2]->gold));

    ui->Tile4_Stone->setText(QString::number(board[0][3]->stone));  //4
    ui->Tile4_Army->setText(QString::number(board[0][3]->army));
    ui->Tile4_Gold->setText(QString::number(board[0][3]->gold));

    ui->Tile5_Stone->setText(QString::number(board[1][0]->stone));  //5
    ui->Tile5_Army->setText(QString::number(board[1][0]->army));
    ui->Tile5_Gold->setText(QString::number(board[1][0]->gold));

    ui->Tile6_Stone->setText(QString::number(board[1][1]->stone));  //6
    ui->Tile6_Army->setText(QString::number(board[1][1]->army));
    ui->Tile6_Gold->setText(QString::number(board[1][1]->gold));

    ui->Tile7_Stone->setText(QString::number(board[1][2]->stone));  //7
    ui->Tile7_Army->setText(QString::number(board[1][2]->army));
    ui->Tile7_Gold->setText(QString::number(board[1][2]->gold));

    ui->Tile8_Stone->setText(QString::number(board[1][3]->stone));  //8
    ui->Tile8_Army->setText(QString::number(board[1][3]->army));
    ui->Tile8_Gold->setText(QString::number(board[1][3]->gold));

    ui->Tile9_Stone->setText(QString::number(board[2][0]->stone));  //9
    ui->Tile9_Army->setText(QString::number(board[2][0]->army));
    ui->Tile9_Gold->setText(QString::number(board[2][0]->gold));

    ui->Tile10_Stone->setText(QString::number(board[2][1]->stone)); //10
    ui->Tile10_Army->setText(QString::number(board[2][1]->army));
    ui->Tile10_Gold->setText(QString::number(board[2][1]->gold));

    ui->Tile11_Stone->setText(QString::number(board[2][2]->stone)); //11
    ui->Tile11_Army->setText(QString::number(board[2][2]->army));
    ui->Tile11_Gold->setText(QString::number(board[2][2]->gold));

    ui->Tile12_Stone->setText(QString::number(board[2][3]->stone)); //12
    ui->Tile12_Army->setText(QString::number(board[2][3]->army));
    ui->Tile12_Gold->setText(QString::number(board[2][3]->gold));

    ui->Tile13_Stone->setText(QString::number(board[3][0]->stone));  //13
    ui->Tile13_Army->setText(QString::number(board[3][0]->army));
    ui->Tile13_Gold->setText(QString::number(board[3][0]->gold));

    ui->Tile14_Stone->setText(QString::number(board[3][1]->stone)); //14
    ui->Tile14_Army->setText(QString::number(board[3][1]->army));
    ui->Tile14_Gold->setText(QString::number(board[3][1]->gold));

    ui->Tile15_Stone->setText(QString::number(board[3][2]->stone)); //15
    ui->Tile15_Army->setText(QString::number(board[3][2]->army));
    ui->Tile15_Gold->setText(QString::number(board[3][2]->gold));

    ui->Tile16_Stone->setText(QString::number(board[3][3]->stone)); //16
    ui->Tile16_Army->setText(QString::number(board[3][3]->army));
    ui->Tile16_Gold->setText(QString::number(board[3][3]->gold));

    board[3][0]->owner = 1;
    board[0][3]->owner = 2;

}

void MainWindow::setResources(Player *p1, Player *p2, Tile * t){

    int curStone, curArmy, curGold;

    if (whoseTurn){
        curStone = p1->stone_;          // temp variables for player's current resources
        curArmy = p1->army_;
        curGold = p1->gold_;

        ui->P1_Stone->setText(QString::number(curStone));        // update player's resources
        ui->P1_Army->setText(QString::number(curArmy));
        ui->P1_Gold->setText(QString::number(curGold));
    }
    else {
        curStone = p2->stone_;      // temp variables for player's current resources
        curArmy = p2->army_;
        curGold = p2->gold_;

        ui->P2_Stone->setText(QString::number(curStone));        // update player's resources
        ui->P2_Army->setText(QString::number(curArmy));
        ui->P2_Gold->setText(QString::number(curGold));
    }
}

// Actions for when a tile is clicked (A player acquirs tile and its resources)
void MainWindow::TileClickedSlot(Tile *t, Qt::MouseButton button) {

    qDebug() << "Tile Clicked: " << t->tileAlias;

    QColor Green(37, 165, 28);
    QColor Red(255, 0, 51);
    QColor lightGreen(128, 247, 132);
    QColor lightRed(244, 151, 170);
    QColor White(255,255,255);

 /*    for (int i = 0; i < p1AdjVec.size(); i++){
        if (p1AdjVec[i]->tileAlias == t->tileAlias) { tileInVectorP1 = true; }
    }

    for (int i = 0; i < p2AdjVec.size(); i++){
        if (p2AdjVec[i]->tileAlias == t->tileAlias) { tileInVectorP2 = true; }
    }

    if(whoseTurn && !tileInVectorP1){
        QMessageBox msgBox;
        msgBox.setText("Please click a tile in your influence!");
        msgBox.exec();
        takeTurn(playerOne, playerTwo);
    }

    if(!whoseTurn && !tileInVectorP2){
        QMessageBox msgBox;
        msgBox.setText("Please click a tile in your influence!");
        msgBox.exec();
        takeTurn(playerOne, playerTwo);
    } */

    // update player resources when tile acquired, and ensure only one tile is clicked
    if(tileClickCount < 1 && whoseTurn && button == Qt::LeftButton){

        t->color_ = Green;
        t->owner = 1;
        playerOne->stone_ += t->stone;
        playerOne->army_ += t->army;
        playerOne->gold_ += t->gold;
        playerOne->curLoc = t->tileAlias;
        hideEmojiP1();
        showLocEmojiP1(playerOne);
        tileClickCount++;

        for (int i = 0; i < x_dim; i++){
            for (int j = 0; j < y_dim; j++){
                if (board[i][j]->color_ == lightGreen) board[i][j]->color_ = White;
            }
        }

        setResources(playerOne, playerTwo, t);
        qDebug() << "Whose Turn: " << whoseTurn;
        qDebug() << "P1 Cur Loc: " << playerOne->curLoc;
        qDebug() << "Player 1 Gold: << " << playerOne->gold_;
        qDebug() << "Player 1 Stone: << " << playerOne->stone_;
        scene->update();
     }

    // update player resources when tile acquired, ensure only one tile is clicked, update tile owner
    else if(tileClickCount < 1 && !whoseTurn && button == Qt::LeftButton){

        t->color_ = Red;
        t->owner = 2;
        playerTwo->stone_ += t->stone;
        playerTwo->army_ += t->army;
        playerTwo->gold_ += t->gold;
        playerTwo->curLoc = t->tileAlias;
        hideEmojiP2();
        showLocEmojiP2(playerTwo);
        tileClickCount++;

        qDebug() << "tile Alias clicked: " << t->tileAlias;
        qDebug() << "P1 CurLoc: " << playerTwo->curLoc;

        for (int i = 0; i < x_dim; i++){
            for (int j = 0; j < y_dim; j++){
                if (board[i][j]->color_ == lightRed) board[i][j]->color_ = White;
            }
        }

        setResources(playerOne, playerTwo, t);
        qDebug() << "Whose Turn: " << whoseTurn;
        qDebug() << "P2 Cur Loc: " << playerTwo->curLoc;
        qDebug() << "Player 2 Gold: << " << playerTwo->gold_;
        qDebug() << "Player 2 Stone: << " << playerTwo->stone_;
        scene->update();
     }
}

void MainWindow::buildCastle(Player *p1, Player *p2)
{

    // build a castle
    if(whoseTurn && p1->gold_ >= 100 && p1->stone_ >= 100){

        qDebug() << "Whose Turn: " << whoseTurn;
        qDebug() << "P1 Cur Loc: " << p1->curLoc;
        qDebug() << "Player 1 Gold: << " << p1->gold_;
        qDebug() << "Player 1 Stone: << " << p1->stone_;

        for (int i = 0; i < x_dim; i++){
            for (int j = 0; j < y_dim; j++){
                if (board[i][j]->tileAlias == p1->curLoc){          // find current cell to build castle on
                                                            // inheriance here to upgrade cell to castle
                //    int tempAlias = board[i][j]->tileAlias;
                //   Tile * c = new Castle(tempAlias);
                //  board[i][j] = c;    // upgrade to castle
                //    c->owner = 1;
                    qDebug() << "Do I get here??";

                    if(board[i][j]->tileAlias == 1) { ui->Castle1->show(); }            // display castle
                    if(board[i][j]->tileAlias == 2) { ui->Castle2->show(); }
                    if(board[i][j]->tileAlias == 3) { ui->Castle3->show(); }
                    if(board[i][j]->tileAlias == 4) { ui->Castle4->show(); }
                    if(board[i][j]->tileAlias == 5) { ui->Castle5->show(); }
                    if(board[i][j]->tileAlias == 6) { ui->Castle6->show(); }
                    if(board[i][j]->tileAlias == 7) { ui->Castle7->show(); }
                    if(board[i][j]->tileAlias == 8) { ui->Castle8->show(); }
                    if(board[i][j]->tileAlias == 9) { ui->Castle9->show(); }
                    if(board[i][j]->tileAlias == 10) { ui->Castle10->show(); }
                    if(board[i][j]->tileAlias == 11) { ui->Castle11->show(); }
                    if(board[i][j]->tileAlias == 12) { ui->Castle12->show(); }
                    if(board[i][j]->tileAlias == 13) { ui->Castle13->show(); }
                    if(board[i][j]->tileAlias == 14) { ui->Castle14->show(); }
                    if(board[i][j]->tileAlias == 15) { ui->Castle15->show(); }
                    if(board[i][j]->tileAlias == 16) { ui->Castle16->show(); }
                    p1->gold_ -= 100;                                                   // update player resources
                    p1->stone_ -= 100;
                    p1->victoryPoints_ += 1;
                    ui->P1_Gold->setText(QString::number(p1->gold_));
                    ui->P1_Stone->setText(QString::number(p1->stone_));
                    ui->P1_VPoints->setText(QString::number(p1->victoryPoints_));
                    // check winner
                    checkWinner(p1, p2);
                    scene->update();
                    break;
                }
            }
        }
    }

    if(!whoseTurn && p2->gold_ >= 100 && p2->stone_ >= 100){

        qDebug() << "Whose Turn: " << whoseTurn;
        qDebug() << "P2 Cur Loc: " << p2->curLoc;
        qDebug() << "P2 Gold: << " << p2->gold_;
        qDebug() << "P2 Stone: << " << p2->stone_;

        for (int i = 0; i < x_dim; i++){
            for (int j = 0; j < y_dim; j++){
                if (board[i][j]->tileAlias == p2->curLoc){
                 //   Tile * c = new Castle;                              // inheritance here to upgrade cell to castle
                 //   board[i][j] = c;    // upgrade to castle
                 //   c->owner = 2;

                    qDebug() << "Do I get here??";

                    if(board[i][j]->tileAlias == 1) { ui->Castle1->show(); }
                    if(board[i][j]->tileAlias == 2) { ui->Castle2->show(); }
                    if(board[i][j]->tileAlias == 3) { ui->Castle3->show(); }
                    if(board[i][j]->tileAlias == 4) { ui->Castle4->show(); }
                    if(board[i][j]->tileAlias == 5) { ui->Castle5->show(); }
                    if(board[i][j]->tileAlias == 6) { ui->Castle6->show(); }
                    if(board[i][j]->tileAlias == 7) { ui->Castle7->show(); }
                    if(board[i][j]->tileAlias == 8) { ui->Castle8->show(); }
                    if(board[i][j]->tileAlias == 9) { ui->Castle9->show(); }
                    if(board[i][j]->tileAlias == 10) { ui->Castle10->show(); }
                    if(board[i][j]->tileAlias == 11) { ui->Castle11->show(); }
                    if(board[i][j]->tileAlias == 12) { ui->Castle12->show(); }
                    if(board[i][j]->tileAlias == 13) { ui->Castle13->show(); }
                    if(board[i][j]->tileAlias == 14) { ui->Castle14->show(); }
                    if(board[i][j]->tileAlias == 15) { ui->Castle15->show(); }
                    if(board[i][j]->tileAlias == 16) { ui->Castle16->show(); }
                    p2->gold_ -= 100;
                    p2->stone_ -= 100;
                    p2-> victoryPoints_ += 1;                                       // update player resources
                    ui->P2_Gold->setText(QString::number(p2->gold_));
                    ui->P2_Stone->setText(QString::number(p2->stone_));
                    ui->P2_VPoints->setText(QString::number(p2->victoryPoints_));
                    // check winner
                    checkWinner(p1, p2);
                    scene->update();
                    break;
                }
            }
        }
    }
}

void MainWindow::buyVPGold(Player *p1, Player *p2)
{
        qDebug() << "Buy VP Gold Function Called";

    if (whoseTurn && p1->gold_ >= 200){

        qDebug() << "Whose Turn: " << whoseTurn;
        qDebug() << "P1 Gold: << " << p1->gold_;

        p1->gold_ -= 200;
        p1->victoryPoints_ += 1;                                       // update player resources
        ui->P1_Gold->setText(QString::number(p1->gold_));
        ui->P1_VPoints->setText(QString::number(p1->victoryPoints_));
        // check winner
        checkWinner(p1, p2);
    }
    else if (!whoseTurn && p2->gold_ >= 200){   // update player resources

        qDebug() << "Whose Turn: " << whoseTurn;
        qDebug() << "P2 Gold: << " << p2->gold_;

        p2->gold_ -= 200;
        p2->victoryPoints_ += 1;
        ui->P2_Gold->setText(QString::number(p2->gold_));
        ui->P2_VPoints->setText(QString::number(p2->victoryPoints_));
        // check winner
        checkWinner(p1, p2);
    }
}

void MainWindow::buyVPArmy(Player *p1, Player *p2)
{

    if (whoseTurn && p1->army_ >= 200){
        p1->army_ -= 200;
        p1->victoryPoints_ += 1;                                       // update player resources
        ui->P1_Army->setText(QString::number(p1->army_));
        ui->P1_VPoints->setText(QString::number(p1->victoryPoints_));
        scene->update();
        // check winner
        checkWinner(p1, p2);
    }
    else if (!whoseTurn && p2->army_ >= 200){
        p2->army_ -= 200;
        p2->victoryPoints_ += 1;
        ui->P2_Army->setText(QString::number(p2->army_));
        ui->P2_VPoints->setText(QString::number(p2->victoryPoints_));
        scene->update();
        // check winner
        checkWinner(p1, p2);
    }
}

void MainWindow::checkWinner(Player *p1, Player *p2)
{
    if(whoseTurn && p1->victoryPoints_ >= endVP){
        if(p2->victoryPoints_ <= endVP-2){
            QMessageBox msgBox;
            msgBox.setText("Player 1 Has Won");
            msgBox.exec();
            QCoreApplication::quit();
        }
    }

    else if(!whoseTurn && p2->victoryPoints_ >= endVP){
        if(p1->victoryPoints_ <= endVP-2){
            QMessageBox msgBox;
            msgBox.setText("Player 2 Has Won");
            msgBox.exec();
            QCoreApplication::quit();
        }
    }
}

// Enable Computer Play Player 1
void MainWindow::stepPlayer1(Player *p1, Player *p2){

    qDebug() << "step Player 1 function called";

    QColor Green(37, 165, 28);
    QColor Red(255, 0, 51);
    QColor lightGreen(128, 247, 132);
    QColor lightRed(244, 151, 170);
    QColor White(255,255,255);

    getAdjacentP1(p1->curLoc);

    // fill p1 Adjacency Vector with possible tiles to claim
    for (int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
            if (board[i][j]->color_ == lightGreen) { p1AdjVec.push_back(board[i][j]); }
        }
    }

    if (!p1AdjVec.empty()) {
        std::random_shuffle(p1AdjVec.begin(), p1AdjVec.end());          // shuffle vector
        TileClickedSlot(p1AdjVec[0], Qt::LeftButton);           // take first element, for pseudo-random selection
    }

    if( p1->gold_ >= 100 && p1->stone_ >= 100) ui->BuildCastle->animateClick();
    if( p1->gold_ >= 200) ui->BuyVPGoldButton->animateClick();
    if( p1->army_ >= 200) ui->BuyVPArmyButton->animateClick();

    checkWinner(p1, p2);            // check if winner
    p1AdjVec.clear();               // clear vector for next turn
    ui->EndTurnButton->animateClick(); // end turn when no more options to gain Victory Point

}

// Enable Computer Play Player 2
void MainWindow::stepPlayer2(Player *p1, Player *p2){

    QColor Green(37, 165, 28);
    QColor Red(255, 0, 51);
    QColor lightGreen(128, 247, 132);
    QColor lightRed(244, 151, 170);
    QColor White(255,255,255);

    getAdjacentP2(p2->curLoc);

    // fill p2 Adjacency Vector with possible tiles to claim
    for (int i = 0; i < x_dim; i++){
        for (int j = 0; j < y_dim; j++){
            if (board[i][j]->color_ == lightRed) { p2AdjVec.push_back(board[i][j]); }
        }
    }

    if (!p2AdjVec.empty()) {
        std::random_shuffle(p2AdjVec.begin(), p2AdjVec.end());          // shuffle vector
        TileClickedSlot(p2AdjVec[0], Qt::LeftButton);           // take first element, for pseudo-random selection
    }

    if( p2->gold_ >= 100 && p2->stone_ >= 100) ui->BuildCastle->animateClick();
    if( p2->gold_ >= 200) ui->BuyVPGoldButton->animateClick();
    if( p2->army_ >= 200) ui->BuyVPArmyButton->animateClick();

    checkWinner(p1, p2);
    p2AdjVec.clear();
    ui->EndTurnButton->animateClick();  // end turn when no more options to gain Victory Point

}

void MainWindow::on_EndTurnButton_clicked()
{
    qDebug() << "End turn clicked";
    whoseTurn = !whoseTurn;             // change bool
    tileClickCount = 0;                 // reset tileClickCount for next turn
    takeTurn(playerOne, playerTwo);     // next player takes turn
}

void MainWindow::on_BuildCastleButton_clicked()
{
    qDebug() << "Build Castle clicked";
    buildCastle(playerOne, playerTwo);
}

void MainWindow::on_BuyVPGoldButton_clicked()
{
    qDebug() << "Buy VP Gold Button clicked";
    buyVPGold(playerOne, playerTwo);

}

void MainWindow::on_BuyVPArmyButton_clicked()
{
    qDebug() << "Buy VP Army Button clicked";
    buyVPArmy(playerOne, playerTwo);
}

void MainWindow::on_StepPlayer1Button_clicked()
{
    qDebug() << "Step P1 Clicked";
    stepPlayer1(playerOne, playerTwo);
}

void MainWindow::on_StepPlayer2Button_clicked()
{
    qDebug() << "Step P2 Clicked";
    stepPlayer2(playerOne, playerTwo);
}


void MainWindow::TileShiftClickedSlot(Tile * t, Qt::MouseButton button)
{
    if (whoseTurn)  {
        playerOne->curLoc = t->tileAlias;
        qDebug() << "Player One is now located at tile: " << playerOne->curLoc;
        hideEmojiP1();
        showLocEmojiP1(playerOne);
    }
    else if (!whoseTurn) {
        playerTwo->curLoc = t->tileAlias;
        qDebug() << "Player Two is now located at tile: " << playerTwo->curLoc;
        hideEmojiP2();
        showLocEmojiP2(playerTwo);
    }
}

/*
void MainWindow::on_BattleButton_clicked()
{
    qDebug() << "Battle Button Clicked";
    doBattle(playerOne, playerTwo);
}


void MainWindow::doBattle(Player *p1, Player *p2)
{
    if(whoseTurn){
        doBattleP1onP2(p1, p2);
    }
    else if(!whoseTurn){
        doBattleP2onP1(p1, p2);
    }

}

void MainWindow::doBattleP1onP2(Player *p1, Player *p2)
{
    showAdjacentP1Battle(p1->curLoc);
}

void MainWindow::doBattleP2onP1(Player *p1, Player *p2)
{

}

/*
void MainWindow::showAdjacentP1Battle(int curLoc)
{

    QColor Black(0,0,0);

    if(curLoc == 1){                                //1
            if(board[0][1]->owner == 2) board[0][1]->color_ = Black;
            if(board[1][0]->owner == 2) board[1][0]->color_ = Black;
            if(board[1][1]->owner == 2) board[1][1]->color_ = Black;
            scene->update();
    }

    if(curLoc == 2){
            if(board[0][0]->owner == 2) board[0][0]->color_ = Black;
            if(board[0][2]->owner == 2) board[0][2]->color_ = Black;
            if(board[1][0]->owner == 2) board[1][0]->color_ = Black;
            if(board[1][1]->owner == 2) board[1][1]->color_ = Black;
            if(board[1][2]->owner == 2) board[1][2]->color_ = Black;
            scene->update();
    }

    if(curLoc == 3){
       if(board[0][1]->owner == 2)   board[0][1]->color_ = Black;
       if(board[1][1]->owner == 2)   board[1][1]->color_ = Black;
       if(board[1][2]->owner == 2)   board[1][2]->color_ = Black;
       if(board[1][3]->owner == 2)  board[1][3]->color_ = Black;
       if(board[0][3]->owner == 2)  board[0][3]->color_ = Black;
        scene->update();
    }

    if(curLoc == 4){                            //4
       if(board[0][2]->owner == 2)  board[0][2]->color_ = Black;
       if(board[1][2]->owner == 2)  board[1][2]->color_ = Black;
       if(board[1][3]->owner == 2)  board[1][3]->color_ = Black;
        scene->update();
    }

    if(curLoc == 5){                                //5
            if(board[0][0]->owner == 2) board[0][0]->color_ = Black;
            if(board[0][1]->owner == 2) board[0][1]->color_ = Black;
            if(board[1][1]->owner == 2) board[1][1]->color_ = Black;
            if(board[2][0]->owner == 2) board[2][0]->color_ = Black;
            if(board[2][1]->owner == 2) board[2][1]->color_ = Black;
            scene->update();
    }

    if(curLoc == 6){
            if(board[0][0]->owner == 2) board[0][0]->color_ = Black;
            if(board[0][1]->owner == 2) board[0][1]->color_ = Black;
            if(board[0][2]->owner == 2) board[0][2]->color_ = Black;
            if(board[1][0]->owner == 2) board[1][0]->color_ = Black;
            if(board[2][0]->owner == 2) board[2][0]->color_ = Black;
            if(board[2][1]->owner == 2) board[2][1]->color_ = Black;
            if(board[1][2]->owner == 2) board[1][2]->color_ = Black;
            if(board[2][2]->owner == 2) board[2][2]->color_ = Black;
            scene->update();
    }

    if(curLoc == 7){
       if(board[0][1]->owner == 2)  board[0][1]->color_ = Black;
       if(board[0][2]->owner == 2)  board[0][2]->color_ = Black;
       if(board[0][3]->owner == 2)  board[0][3]->color_ = Black;
       if(board[1][1]->owner == 2)  board[1][1]->color_ = Black;
       if(board[1][3]->owner == 2)  board[1][3]->color_ = Black;
       if(board[2][1]->owner == 2)  board[2][1]->color_ = Black;
       if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
       if(board[2][3]->owner == 2)  board[2][3]->color_ = Black;
       scene->update();
    }

    if(curLoc == 8){                            //8
       if(board[0][2]->owner == 2)  board[0][2]->color_ = Black;
       if(board[0][3]->owner == 2)  board[0][3]->color_ = Black;
       if(board[1][2]->owner == 2)  board[1][2]->color_ = Black;
       if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
       if(board[2][3]->owner == 2)  board[2][3]->color_ = Black;
       scene->update();
    }

    if(curLoc == 9){                            //9
        if(board[1][0]->owner == 2)  board[1][0]->color_ = Black;
        if(board[1][1]->owner == 2)  board[1][1]->color_ = Black;
        if(board[2][1]->owner == 2)  board[2][1]->color_ = Black;
        if(board[3][0]->owner == 2)  board[3][0]->color_ = Black;
        if(board[3][1]->owner == 2)  board[3][1]->color_ = Black;
        scene->update();
    }

    if(curLoc == 10){       //10
       if(board[1][0]->owner == 2)  board[1][0]->color_ = Black;
       if(board[1][1]->owner == 2)  board[1][1]->color_ = Black;
       if(board[1][2]->owner == 2)  board[1][2]->color_ = Black;
       if(board[2][0]->owner == 2)  board[2][0]->color_ = Black;
       if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
       if(board[3][0]->owner == 2)  board[3][0]->color_ = Black;
       if(board[3][1]->owner == 2)  board[3][1]->color_ = Black;
       if(board[3][2]->owner == 2)  board[3][2]->color_ = Black;
       scene->update();
    }

    if(curLoc == 11){       //11
       if(board[1][1]->owner == 2)  board[1][1]->color_ = Black;
       if(board[1][2]->owner == 2)  board[1][2]->color_ = Black;
       if(board[1][3]->owner == 2)  board[1][3]->color_ = Black;
       if(board[2][1]->owner == 2)  board[2][1]->color_ = Black;
       if(board[2][3]->owner == 2)  board[2][3]->color_ = Black;
       if(board[3][1]->owner == 2)  board[3][1]->color_ = Black;
       if(board[3][2]->owner == 2)  board[3][2]->color_ = Black;
       if(board[3][3]->owner == 2)  board[3][3]->color_ = Black;
       scene->update();
    }

    if(curLoc == 12){                            //12
        if(board[1][2]->owner == 2)  board[1][2]->color_ = Black;
        if(board[1][3]->owner == 2)  board[1][3]->color_ = Black;
        if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
        if(board[3][2]->owner == 2)  board[3][2]->color_ = Black;
        if(board[3][3]->owner == 2)  board[3][3]->color_ = Black;
        scene->update();
    }

    if(curLoc == 13){                            //13
       if(board[2][0]->owner == 2)  board[2][0]->color_ = Black;
       if(board[2][1]->owner == 2)  board[2][1]->color_ = Black;
       if(board[3][1]->owner == 2)  board[3][1]->color_ = Black;
        scene->update();
    }

    if(curLoc == 14){                          //14
        if(board[2][0]->owner == 2)  board[2][0]->color_ = Black;
        if(board[2][1]->owner == 2)  board[2][1]->color_ = Black;
        if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
        if(board[3][0]->owner == 2)  board[3][0]->color_ = Black;
        if(board[3][2]->owner == 2)  board[3][2]->color_ = Black;
        scene->update();
    }

    if(curLoc == 15){                          //15
        if(board[2][1]->owner == 2)  board[2][1]->color_ = Black;
        if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
        if(board[2][3]->owner == 2)  board[2][3]->color_ = Black;
        if(board[3][1]->owner == 2)  board[3][1]->color_ = Black;
        if(board[3][3]->owner == 2)  board[3][3]->color_ = Black;
        scene->update();
    }

    if(curLoc == 16){                            //16
       if(board[2][2]->owner == 2)  board[2][2]->color_ = Black;
       if(board[2][3]->owner == 2)  board[2][3]->color_ = Black;
       if(board[3][2]->owner == 2)  board[3][2]->color_ = Black;
        scene->update();
    }
}

*/

void MainWindow::hideGraphics(){

    ui->Castle1->hide();
    ui->Castle2->hide();
    ui->Castle3->hide();
    ui->Castle4->hide();
    ui->Castle5->hide();
    ui->Castle6->hide();
    ui->Castle7->hide();
    ui->Castle8->hide();
    ui->Castle9->hide();
    ui->Castle10->hide();
    ui->Castle11->hide();
    ui->Castle12->hide();
    ui->Castle13->hide();
    ui->Castle14->hide();
    ui->Castle15->hide();
    ui->Castle16->hide();

    ui->P1Loc_1->hide();
    ui->P1Loc_2->hide();
    ui->P1Loc_3->hide();
    ui->P1Loc_4->hide();
    ui->P1Loc_5->hide();
    ui->P1Loc_6->hide();
    ui->P1Loc_7->hide();
    ui->P1Loc_8->hide();
    ui->P1Loc_9->hide();
    ui->P1Loc_10->hide();
    ui->P1Loc_11->hide();
    ui->P1Loc_12->hide();
   // ui->P1Loc_13->hide();
    ui->P1Loc_14->hide();
    ui->P1Loc_15->hide();
    ui->P1Loc_16->hide();

    ui->P2Loc_1->hide();
    ui->P2Loc_2->hide();
    ui->P2Loc_3->hide();
   // ui->P2Loc_4->hide();
    ui->P2Loc_5->hide();
    ui->P2Loc_6->hide();
    ui->P2Loc_7->hide();
    ui->P2Loc_8->hide();
    ui->P2Loc_9->hide();
    ui->P2Loc_10->hide();
    ui->P2Loc_11->hide();
    ui->P2Loc_12->hide();
    ui->P2Loc_13->hide();
    ui->P2Loc_14->hide();
    ui->P2Loc_15->hide();
    ui->P2Loc_16->hide();
}

void MainWindow::showLocEmojiP1(Player *p1)
{
    if(p1->curLoc == 1) { ui->P1Loc_1->show(); }
    if(p1->curLoc == 2) { ui->P1Loc_2->show(); }
    if(p1->curLoc == 3) { ui->P1Loc_3->show(); }
    if(p1->curLoc == 4) { ui->P1Loc_4->show(); }
    if(p1->curLoc == 5) { ui->P1Loc_5->show(); }
    if(p1->curLoc == 6) { ui->P1Loc_6->show(); }
    if(p1->curLoc == 7) { ui->P1Loc_7->show(); }
    if(p1->curLoc == 8) { ui->P1Loc_8->show(); }
    if(p1->curLoc == 9) { ui->P1Loc_9->show(); }
    if(p1->curLoc == 10) { ui->P1Loc_10->show(); }
    if(p1->curLoc == 11) { ui->P1Loc_11->show(); }
    if(p1->curLoc == 12) { ui->P1Loc_12->show(); }
    if(p1->curLoc == 13) { ui->P1Loc_13->show(); }
    if(p1->curLoc == 14) { ui->P1Loc_14->show(); }
    if(p1->curLoc == 15) { ui->P1Loc_15->show(); }
    if(p1->curLoc == 16) { ui->P1Loc_16->show(); }
}

void MainWindow::showLocEmojiP2(Player *p2)
{
    if(p2->curLoc == 1) { ui->P2Loc_1->show(); }
    if(p2->curLoc == 2) { ui->P2Loc_2->show(); }
    if(p2->curLoc == 3) { ui->P2Loc_3->show(); }
    if(p2->curLoc == 4) { ui->P2Loc_4->show(); }
    if(p2->curLoc == 5) { ui->P2Loc_5->show(); }
    if(p2->curLoc == 6) { ui->P2Loc_6->show(); }
    if(p2->curLoc == 7) { ui->P2Loc_7->show(); }
    if(p2->curLoc == 8) { ui->P2Loc_8->show(); }
    if(p2->curLoc == 9) { ui->P2Loc_9->show(); }
    if(p2->curLoc == 10) { ui->P2Loc_10->show(); }
    if(p2->curLoc == 11) { ui->P2Loc_11->show(); }
    if(p2->curLoc == 12) { ui->P2Loc_12->show(); }
    if(p2->curLoc == 13) { ui->P2Loc_13->show(); }
    if(p2->curLoc == 14) { ui->P2Loc_14->show(); }
    if(p2->curLoc == 15) { ui->P2Loc_15->show(); }
    if(p2->curLoc == 16) { ui->P2Loc_16->show(); }
}

void MainWindow::hideEmojiP1()
{
    ui->P1Loc_1->hide();
    ui->P1Loc_2->hide();
    ui->P1Loc_3->hide();
    ui->P1Loc_4->hide();
    ui->P1Loc_5->hide();
    ui->P1Loc_6->hide();
    ui->P1Loc_7->hide();
    ui->P1Loc_8->hide();
    ui->P1Loc_9->hide();
    ui->P1Loc_10->hide();
    ui->P1Loc_11->hide();
    ui->P1Loc_12->hide();
    ui->P1Loc_13->hide();
    ui->P1Loc_14->hide();
    ui->P1Loc_15->hide();
    ui->P1Loc_16->hide();
}


void MainWindow::hideEmojiP2()
{
    ui->P2Loc_1->hide();
    ui->P2Loc_2->hide();
    ui->P2Loc_3->hide();
    ui->P2Loc_4->hide();
    ui->P2Loc_5->hide();
    ui->P2Loc_6->hide();
    ui->P2Loc_7->hide();
    ui->P2Loc_8->hide();
    ui->P2Loc_9->hide();
    ui->P2Loc_10->hide();
    ui->P2Loc_11->hide();
    ui->P2Loc_12->hide();
    ui->P2Loc_13->hide();
    ui->P2Loc_14->hide();
    ui->P2Loc_15->hide();
    ui->P2Loc_16->hide();
}
MainWindow::~MainWindow()
{
    delete ui;
}
