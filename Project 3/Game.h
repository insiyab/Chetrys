#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "Well.h"
#include "UserInterface.h"
#include <string>
// [Add other #include directives as necessary.]

class genericPiece;

class Game
{
  public:
    Game(int width, int height);
    ~Game();
    void play();
    bool playOneLevel();
    void displayPrompt(std::string s);
    void displayStatus();
    bool userInput(genericPiece* p);
    void enterNewPiece();

  private:
    Screen  m_screen;
    Well    m_well;
    int     m_level;
    int     m_score;
    int     m_rowsCleared;
    genericPiece*  m_piece;
    genericPiece*  m_nextPiece;
    bool    m_space;
    
};

#endif // GAME_INCLUDED
