#ifndef PIECE_INCLUDED
#define PIECE_INCLUDED

#include <vector>
using namespace std;

//////////// PIECETYPE ENUM

enum PieceType {
    PIECE_I, PIECE_L, PIECE_J, PIECE_T, PIECE_O, PIECE_S, PIECE_Z,
    PIECE_VAPOR, PIECE_FOAM, PIECE_CRAZY, NUM_PIECE_TYPES
};

PieceType chooseRandomPieceType(); // implemented in chetrys.cpp


//////////// COORD CLASS

class Coord
{
private:
    int m_r, m_c;
public:
    Coord(int r, int c) : m_r(r), m_c(c) {}
    Coord() : m_r(0), m_c(0) {}
    
    int r() const { return m_r; }
    int c() const { return m_c; }
    void changeCoord(int r, int c){
        m_r = r;
        m_c = c;
    }
    void changeCoord(const Coord& co){
        m_r = co.r();
        m_c = co.c();
    }
};

//////////// GENERIC PIECE CLASS
class Well;
class Screen;

// this could probably be made into an abstract class

class genericPiece{
public:
    
    vector<Coord> blox; // generally, tetris shapes tend to have four blocks comprising their shapes
                        // vapor bombs and foam bombs will use only the first two blocks and one block respectively
    
    // each piece should ideally have their own constructor, will use default destructor
    virtual ~genericPiece(){}
    
    virtual void drawPieceInWell(Well& well) const;     // prints the piece at its starting position in the well
    virtual void printPiece(Screen&, int, int) const{}  // print the piece at position XY on the scren wrt bounding box
    
    // these three functions perform bounds checking (but don't modify the well)
    virtual bool atLeftYet(const Well& well) const;     // checks if the piece has room to the left
    
    virtual bool atRightYet(const Well& well) const;    // checks if the piece has room to the right
    
    virtual bool atBottomYet(const Well& well) const;   // checks if piece has reached it's bottom bounds
    
    // these three functions perform rotation/L/R movement if it is okay to do so
    virtual void rotateIfOkay(Well&){}  // checks if the piece can rotate, and if yes, performs rotation
                                        // Piece_O, VaporBomb, and Foam Bomb will use this implementation
    
    virtual void leftIfOkay(Well& well);                // moves piece left if it is okay to do so
    
    virtual void rightIfOkay(Well& well);               // moves piece right if it is okay to do so
    
    //these two functions work with the timer to simulate game conditions
    virtual void moveDown(Well& well);                  // moves the piece down one space or adds it to the well if at bottom
    
    virtual void addPieceToWell(Well& well);            // changes '#' to '$' in well (DOES NOT DELETE THE PIECE)
    
    // topRight getter and setter functions - for the top right corner of the piece area
    Coord getTopLeft() const { return topLeft; }
    void setTopLeft(int x, int y){ topLeft.changeCoord(y,x); }
    
    // orientation getter and setter functions
    int getOrientation() const { return orientation; }
    
    void setOrientation(int x) {
        if(x >= 0 && x < 4)
            orientation = x;
    }
    
    // numBlocks getter and setter functions
    int getNumBlocks() const { return numBlocks; }
    
    void setNumBlocks(int x) {
        if(x >= 0 && x <= 4)
            numBlocks = x;
    }
    
private:
    Coord topLeft;   // topRight coordinate of BOUNDS; used to make sure the piece isn't stuck in the top of the well
    int orientation; // 0, 1, 2, or 3 as defined by the spec
    int numBlocks = 0;
    
    // O, VaporBomb, and FoamBomb pieces have only one possible orientation
    // I, S, and Z pieces have two possible orientations
    // T, L, J, and Crazy pieces have four possible orientations
};


// this functions simplifies the rotation of the piece
// just enter the coordinates to be checked and possibly made into the rotated piece
// returns true if rotation was successful
bool fourBlockRotate(Well& well, Coord& bZero, Coord& bOne, Coord& bTwo, Coord& bThree, const Coord& zero, const Coord& one, const Coord& two, const Coord& three);


//////////// DERIVED PIECE CLASSES (10 total)

// ##
// ##
class Piece_O : public genericPiece{
public:
    Piece_O();
    void printPiece(Screen& screen, int x, int y) const;
};

// ####
class Piece_I : public genericPiece{
public:
    Piece_I();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
};

//  ##
// ##
class Piece_S : public genericPiece{
public:
    Piece_S();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
};

// ##
//  ##
class Piece_Z : public genericPiece{
public:
    Piece_Z();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
};

//  #
// ###
class Piece_T : public genericPiece{
public:
    Piece_T();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
};

//   #
// ###
class Piece_L : public genericPiece{
public:
    Piece_L();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
};

// #
// ###
class Piece_J : public genericPiece{
public:
    Piece_J();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
};

// #  #
//  ##
class Piece_Crazy : public genericPiece{
public:
    Piece_Crazy();
    void rotateIfOkay(Well& well);
    void printPiece(Screen& screen, int x, int y) const;
    
    void leftIfOkay(Well& well);  //changed to move right
    void rightIfOkay(Well& well); //changed to move left
};

// ##
class Piece_Vapor : public genericPiece{
public:
    Piece_Vapor();
    void drawPieceInWell(Well& well) const;
    void printPiece(Screen& screen, int x, int y) const;
    void leftIfOkay(Well& well);
    void rightIfOkay(Well& well);
    bool atBottomYet(const Well& well);
    void moveDown(Well& well);
    void addPieceToWell(Well& well);
private:
    bool m_exploded;
};

// #
class Piece_Foam : public genericPiece{
public:
    Piece_Foam();
    void drawPieceInWell(Well& well) const;
    void printPiece(Screen& screen, int x, int y) const;
    void leftIfOkay(Well& well);
    void rightIfOkay(Well& well);
    void moveDown(Well& well);
    void addPieceToWell(Well& well);
    bool fillWithFoam(Well& well, Coord pos); // recursively foams the surrounding area after landing
private:
    bool m_exploded;
};


#endif // PIECE_INCLUDED

