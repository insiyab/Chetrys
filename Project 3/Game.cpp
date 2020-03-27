#include "Game.h"
#include "Well.h"
#include "Piece.h"
#include "UserInterface.h"
#include <string>
#include <iostream>

// define some important constants
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 25;

const int WELL_X = 0;
const int WELL_Y = 0;

const int PROMPT_Y = 20;
const int PROMPT_X = 0;

const int SCORE_X = 16;
const int SCORE_Y = 8;

const int ROWS_LEFT_X = 16;
const int ROWS_LEFT_Y = 9;

const int LEVEL_X = 16;
const int LEVEL_Y = 10;

const int NEXT_PIECE_TITLE_X = 16;
const int NEXT_PIECE_TITLE_Y = 3;

const int NEXT_PIECE_X = 16;
const int NEXT_PIECE_Y = 4;

// create a game using defined constants & given parameters, then display the game
Game::Game(int width, int height)
: m_screen(SCREEN_WIDTH, SCREEN_HEIGHT), m_well(width, height), m_level(1), m_score(0), m_rowsCleared(0), m_piece(nullptr), m_nextPiece(nullptr), m_space(true) {
    
    m_well.display(m_screen, WELL_X, WELL_Y);
    
}

// delete dynamically allocated pieces
Game::~Game(){
    delete m_piece;
    delete m_nextPiece;
}

// play a game
void Game::play(){
    m_screen.refresh();
    m_well.display(m_screen, WELL_X, WELL_Y);
    displayStatus();  //  score, rows left, level
    displayPrompt("Press the Enter key to begin playing Chetyris!");
    waitForEnter();  // initialized in in UserInterface.h
    displayPrompt("");
    
    for(;;){
        if (!playOneLevel())
            break;
        displayPrompt("Good job!  Press the Enter key to start next level!");
        waitForEnter();
        displayPrompt("");
    }
    displayPrompt("Game Over!  Press the Enter key to exit!");
    waitForEnter();
}

// display messages underneath the game display
void Game::displayPrompt(std::string s){
    m_screen.gotoXY(PROMPT_X, PROMPT_Y);
    m_screen.printStringClearLine(s);   // overwrites previous text
    m_screen.refresh();
}

// display the score, rows left until next level, and current level next to the game display
void Game::displayStatus(){
    
    m_screen.refresh();
    
    m_screen.gotoXY(NEXT_PIECE_TITLE_X, NEXT_PIECE_TITLE_Y);
    m_screen.printString("Next piece:");
    
    if(m_nextPiece != nullptr)
        m_nextPiece->printPiece(m_screen, NEXT_PIECE_X, NEXT_PIECE_Y);           //this prints out the upcoming piece
    
    size_t spaceCnt = 7 - to_string(m_score).length();
    std::string space = "";
    for(int i = 0; i <= spaceCnt; i++){
        space+=" ";
    }
    
    m_screen.gotoXY(SCORE_X, SCORE_Y);
    m_screen.printStringClearLine("Score:     " + space + to_string(m_score)); //figure out how to make this right justified, 7 characters
    
    
    spaceCnt = 7 - to_string(5*m_level - m_rowsCleared).length();
    space = "";
    for(int i = 0; i <= spaceCnt; i++){
        space+=" ";
    }
    
    m_screen.gotoXY(ROWS_LEFT_X, ROWS_LEFT_Y);
    m_screen.printStringClearLine("Rows left: " + space + to_string(5*m_level - m_rowsCleared));
    
    spaceCnt = 7 - to_string(m_level).length();
    space = "";
    for(int i = 0; i <= spaceCnt; i++){
        space+=" ";
    }
    
    m_screen.gotoXY(LEVEL_X, LEVEL_Y);
    m_screen.printStringClearLine("Level:     " + space + to_string(m_level));
}

// play one level of the game
bool Game::playOneLevel(){

    bool levelPass = false;
    m_rowsCleared = 0;
    int rowClr = 0;
    int maxTime = max(1000-(100*(m_level-1)), 100);                         // calculate this once
    
    m_well.clearWell();
    enterNewPiece();
    Timer t;
    
    int forever = 1;
    
    while(forever == 1){  //keep playing until user either makes it to next level or quits
        m_piece = m_nextPiece;                                              //start off with making a current and next piece
        m_piece->drawPieceInWell(m_well);
        enterNewPiece();                                                    //decide on the next piece
        displayStatus();                                                    //update display accordingly
        m_well.display(m_screen, WELL_X, WELL_Y);
        
        if(m_piece->getTopLeft().r() == 0 && m_piece->atBottomYet(m_well))  // checks to make sure well isn't already full
            return false;
        t.start();                                                          // timed motion of single row drop
        while(!m_piece->atBottomYet(m_well)){
            t.start();                                                      // start the timer if there's still room to fall
            while(t.elapsed() < maxTime){                                   // if the piece is not moving down, the user
                if(!userInput(m_piece))                                     // can manipulate the piece (move or rotate)
                    return false;
                if(!m_space)
                    break;
                m_well.display(m_screen, WELL_X, WELL_Y);
            }
            discardPendingKeys();                                           // ignore any more keypresses
            m_piece->moveDown(m_well);                                      // move down piece once time is up
            m_well.display(m_screen, 0, 0);
        }
        
        if(m_space){                                                        // only if user did not use spacebar or pound the down arrow
            t.start();                                                      // at bottommost point, allow one last move before piece sets
            while(t.elapsed() < maxTime){
                if(!userInput(m_piece))
                    return false;
            }
        }
        else
            m_space = true;                                                 // reset value for each new piece
        
        m_piece->addPieceToWell(m_well);                                    // add piece to well
        delete m_piece;                                                     // free piece, so we can move on to the next one
        
        rowClr = m_well.numRowsCompleted();                                 // check if any rows have been cleared and if so, how many
        m_rowsCleared+=rowClr;                                              // update class's row counter
        
        m_well.display(m_screen, WELL_X, WELL_Y);
        displayStatus();
        
        switch(rowClr){                                                     // update score
            case 0:
                break;
            case 1:
                m_score+=100;
                break;
            case 2:
                m_score+=200;
                break;
            case 3:
                m_score+=400;
                break;
            case 4:
                m_score+=800;
                break;
            case 5:
                m_score+=1600;
                break;
        }
        
        if(m_rowsCleared >= 5*m_level){                                     // check if 5*n rows have been cleared overall
            m_rowsCleared = m_rowsCleared - 5;
            levelPass = true;
        }
        
        if(levelPass){                                                      // check if it's time to start a new level
            displayStatus();
            m_level++;
            return true;
        }
        
    }
    return true;

}

// choose the next piece at random
void Game::enterNewPiece(){
    PieceType type = chooseRandomPieceType(); // returns an enum
    
    if(m_nextPiece != m_piece)
        delete m_nextPiece;
    
    switch(type){
        case NUM_PIECE_TYPES:
        case PIECE_I:
            m_nextPiece = new Piece_I;
            break;
        case PIECE_L:
            m_nextPiece = new Piece_L;
            break;
        case PIECE_J:
            m_nextPiece = new Piece_J;
            break;
        case PIECE_T:
            m_nextPiece = new Piece_T;
            break;
        case PIECE_O:
            m_nextPiece = new Piece_O;
            break;
        case PIECE_S:
            m_nextPiece = new Piece_S;
            break;
        case PIECE_Z:
            m_nextPiece = new Piece_Z;
            break;
        case PIECE_VAPOR:
            m_nextPiece = new Piece_Vapor;
            break;
        case PIECE_FOAM:
            m_nextPiece = new Piece_Foam;
            break;
        case PIECE_CRAZY:
            m_nextPiece = new Piece_Crazy;
            break;
    }
    
    // m_nextPiece = new Piece_Foam();  // use to test one piece at a time
}

// responds to keyboard input
bool Game::userInput(genericPiece* p){              // returns false if user quits or well fills up
    char keyPress;
    
    if(getCharIfAny(keyPress)){
        
        switch(keyPress){  //react to user inputs to modify piece or quit game
            case ARROW_UP: //same as '8'
            case 'w':
                p->rotateIfOkay(m_well);                // rotate
                break;
                
            case ARROW_LEFT: //same as '4'
            case 'a':
                p->leftIfOkay(m_well);                  // move left
                break;
                
            case ARROW_RIGHT: //same as '6'
            case 'd':
                p->rightIfOkay(m_well);                 // move right
                break;
                
            case ARROW_DOWN: //same as '8'
            case 's':                                   // move down once
                p->moveDown(m_well);
                if(p->atBottomYet(m_well))
                    m_space = false;
                break;
                
            case ' ':                                   // set piece at "bottom" of well (may be on another piece)
                while(!p->atBottomYet(m_well)){
                    p->moveDown(m_well);
                }
                m_space = false;
                break;
                
            case 'q':                                   // quit game
            case 'Q':
                return false;                           // quit function
                
        }
    }
    return true;
}

