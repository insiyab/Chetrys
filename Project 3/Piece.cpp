#include "Piece.h"
#include "Well.h"
#include "UserInterface.h"
using namespace std;


PieceType chooseRandomPieceType(); //defined in chetyris.cpp


//////////// GENERIC PIECE IMPLEMENTATIONS

// move left functions

bool genericPiece::atLeftYet(const Well& well) const {
    for(int i = 0; i < getNumBlocks(); i++){
        char c = well.getChar(blox[i].r() - 1, blox[i].c()); //checking space left of #
        if(c == '@' || c == '$' || c == '*')
            return true;
    }
    return false;
}

void genericPiece::leftIfOkay(Well& well){
    if(!atLeftYet(well)){
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], ' '); //clear current piece
        for(int i = 0; i < 4; i++){
            int r = blox[i].r() - 1; //move each block once to the left
            int c = blox[i].c();
            blox[i].changeCoord(r, c);
        }
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], '#'); //reset current piece
        setTopLeft(getTopLeft().r(), getTopLeft().c() - 1);
    }
}


// move right functions

bool genericPiece::atRightYet(const Well& well) const{
    for(int i = 0; i < getNumBlocks(); i++){
        char c = well.getChar(blox[i].r() + 1, blox[i].c()); //checking space left of #
        if(c == '@' || c == '$' || c == '*')
            return true;
    }
    return false;
}

void genericPiece::rightIfOkay(Well& well){
    if(!atRightYet(well)){
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], ' '); //clear current piece
        for(int i = 0; i < 4; i++){
            int r = blox[i].r() + 1; //move each block once to the left
            int c = blox[i].c();
            blox[i].changeCoord(r, c);
        }
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], '#'); //reset current piece
        setTopLeft(getTopLeft().r(), getTopLeft().c() + 1);
    }
}


// move down functions

bool genericPiece::atBottomYet(const Well& well) const{
    for(int i = 0; i < getNumBlocks(); i++){
        char c = well.getChar(blox[i].r(), blox[i].c() + 1); //checking space below #
        if(c == '@' || c == '$' || c == '*')
            return true;
    }
    return false;
}

void genericPiece::moveDown(Well& well){
    
    if(!atBottomYet(well)){
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], ' '); //clear current piece
    
        for(int i = 0; i < 4; i++){
            int r = blox[i].r(); //move each block one row down
            int c = blox[i].c() + 1;
            blox[i].changeCoord(r, c);
        }
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], '#'); //reset current piece
        setTopLeft(getTopLeft().r() + 1, getTopLeft().c());
    }
}

void genericPiece::addPieceToWell(Well& well){
    if(atBottomYet(well)){
        well.modifyPiece(blox[0], blox[1], blox[2], blox[3], '$');
    }
}

void genericPiece::drawPieceInWell(Well& well) const {
    well.modifyPiece(blox[0], blox[1], blox[2], blox[3], '#');
}

// rotation check functions
// in the rotation functions of each piece, r is like x, and c is like y (as on an XY plane)

bool fourBlockRotate(Well& well, Coord& bZero, Coord& bOne, Coord& bTwo, Coord& bThree, const Coord& zero, const Coord& one, const Coord& two, const Coord& three){

    if(well.getChar(zero) == '$' || well.getChar(zero) == '@' || well.getChar(zero) == '*')
        return false;
    if(well.getChar(one) == '$' || well.getChar(one) == '@' || well.getChar(one) == '*')
        return false;
    if(well.getChar(two) == '$' || well.getChar(two) == '@' || well.getChar(two) == '*')
        return false;
    if(well.getChar(three) == '$' || well.getChar(three) == '@' || well.getChar(three) == '*')
        return false;
    
    well.modifyPiece(bZero, bOne, bTwo, bThree, ' ');  //update the well
    bZero.changeCoord(zero);                                //update coordinates
    bOne.changeCoord(one);
    bTwo.changeCoord(two);
    bThree.changeCoord(three);
    well.modifyPiece(bZero, bOne, bTwo, bThree, '#');
    
    return true;
}


/////////// SPECIFIC PIECE IMPLEMENTATIONS

/////////// NORMAL PIECE IMPLEMENTATIONS (including Crazy piece)

/////O piece

Piece_O::Piece_O(){
    
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x,y));
    blox.push_back(Coord(x+1,y));
    blox.push_back(Coord(x,y+1));
    blox.push_back(Coord(x+1,y+1));
}

void Piece_O::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine("##");
    screen.gotoXY(x, y+1); screen.printStringClearLine("##");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////I piece

Piece_I::Piece_I(){
    
    int x = 4;
    int y = 1;                  //since this shape is a row, set y to appropriate row number
    
    setTopLeft(x,y-1);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x,y));
    blox.push_back(Coord(x+1,y));
    blox.push_back(Coord(x+2,y));
    blox.push_back(Coord(x+3,y));
}

void Piece_I::rotateIfOkay(Well& well){
    
    int r = blox[1].r(); //this block's position doesn't change at all
    int c = blox[1].c();
    // bool okToRotate = true;
     
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c-1), Coord(r, c),Coord(r, c+1), Coord(r, c+2)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 0
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r-1,c), Coord(r, c),Coord(r+1, c), Coord(r+2, c)))
                setOrientation(0);
            break;
    }
}

void Piece_I::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine("");
    screen.gotoXY(x, y+1); screen.printStringClearLine("####");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////S piece

Piece_S::Piece_S(){
    
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x,y+2));
    blox.push_back(Coord(x+1,y+2));
    blox.push_back(Coord(x+1,y+1));
    blox.push_back(Coord(x+2,y+1));
}

void Piece_S::rotateIfOkay(Well& well){
    
    int r = blox[2].r(); //this block's position doesn't change at all
    int c = blox[2].c();
    
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c-1), Coord(r+1, c),Coord(r, c), Coord(r+1, c+1)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 0
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1,c), Coord(r, c+1),Coord(r, c), Coord(r-1, c+1)))
                setOrientation(0);
            break;
    }
}

void Piece_S::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine("");
    screen.gotoXY(x, y+1); screen.printStringClearLine(" ##");
    screen.gotoXY(x, y+2); screen.printStringClearLine("##");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


// Z piece

Piece_Z::Piece_Z(){
    
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x,y+1));
    blox.push_back(Coord(x+1,y+1));
    blox.push_back(Coord(x+1,y+2));
    blox.push_back(Coord(x+2,y+2));
}

void Piece_Z::rotateIfOkay(Well& well){
    
    int r = blox[2].r(); //this block's position doesn't change at all
    int c = blox[2].c();
    
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1,c), Coord(r, c+1),Coord(r, c), Coord(r+1, c-1)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 0
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r-1,c), Coord(r, c+1),Coord(r, c), Coord(r+1, c+1)))
                setOrientation(0);
            break;
    }
}

void Piece_Z::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine("");
    screen.gotoXY(x, y+1); screen.printStringClearLine("##");
    screen.gotoXY(x, y+2); screen.printStringClearLine(" ##");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////T piece

Piece_T::Piece_T(){
    
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x+1,y));
    blox.push_back(Coord(x,y+1));
    blox.push_back(Coord(x+1,y+1));
    blox.push_back(Coord(x+2,y+1));
}

void Piece_T::rotateIfOkay(Well& well){
    
    int r = blox[2].r(); // this block's position doesn't change at all
    int c = blox[2].c();
    
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c-1), Coord(r+1, c),Coord(r, c), Coord(r, c+1)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 2
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r-1,c), Coord(r, c+1),Coord(r, c), Coord(r+1, c)))
                setOrientation(2);
            break;
            
        case 2:
            // attempting to go from orientation 2 to 3
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r-1,c), Coord(r, c-1),Coord(r, c), Coord(r, c+1)))
                setOrientation(3);
            break;
            
        case 3:
            // attempting to go from orientation 3 to 0
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c-1), Coord(r-1, c),Coord(r, c), Coord(r+1, c)))
                setOrientation(0);
            break;
    }
}

void Piece_T::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine(" #");
    screen.gotoXY(x, y+1); screen.printStringClearLine("###");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////L piece

Piece_L::Piece_L(){
    
    int x = 4;
    int y = 1; // this piece starts on the second row
    
    setTopLeft(x,y-1);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x,y));
    blox.push_back(Coord(x+1,y));
    blox.push_back(Coord(x+2,y));
    blox.push_back(Coord(x,y+1));
}

void Piece_L::rotateIfOkay(Well& well){
    
    int r;
    int c;
    
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            r = blox[0].r(); //going to top left corner of bounding box
            c = blox[0].c() - 1;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1,c), Coord(r+2, c),Coord(r+2, c+1), Coord(r+2, c+2)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 2
            r = blox[0].r() - 1; //going to top left corner of bounding box
            c = blox[0].c();
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c+2), Coord(r+1, c+2),Coord(r+2, c+2), Coord(r+2, c+1)))
                setOrientation(2);
            break;
            
        case 2:
            // attempting to go from orientation 2 to 3
            r = blox[0].r(); //going to top left corner of bounding box
            c = blox[0].c() - 2;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1, c+1), Coord(r+1, c+2),Coord(r+1, c+3), Coord(r+2, c+3)))
                setOrientation(3);
            break;
            
        case 3:
            // attempting to go from orientation 3 to 0
            r = blox[0].r() - 1; //going to top left corner of bounding box
            c = blox[0].c() - 1;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c+1), Coord(r, c+2),Coord(r+1, c+1), Coord(r+2, c+1)))
                setOrientation(0);
            break;
    }
}

void Piece_L::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine("###");
    screen.gotoXY(x, y+1); screen.printStringClearLine("#");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////J piece

Piece_J::Piece_J(){
    
    int x = 4;
    int y = 1; // this piece starts on the second row
    
    setTopLeft(x,y-1);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x+1,y));
    blox.push_back(Coord(x+2,y));
    blox.push_back(Coord(x+3,y));
    blox.push_back(Coord(x+3,y+1));
}

void Piece_J::rotateIfOkay(Well& well){
    
    int r;
    int c;
    
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            r = blox[0].r() - 1; // going to top left corner of bounding box
            c = blox[0].c() - 1;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+2,c+1), Coord(r+2, c+2),Coord(r+2, c+3), Coord(r+1, c+3)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 2
            r = blox[0].r() - 2; // going to top left corner of bounding box
            c = blox[0].c() - 1;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1,c+1), Coord(r+1, c+2),Coord(r+2, c+2), Coord(r+3, c+2)))
                setOrientation(2);
            break;
            
        case 2:
            // attempting to go from orientation 2 to 3
            r = blox[0].r() - 1; // going to top left corner of bounding box
            c = blox[0].c() - 1;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1, c), Coord(r+1, c+1),Coord(r+1, c+2), Coord(r+2, c)))
                setOrientation(3);
            break;
            
        case 3:
            // attempting to go from orientation 3 to 0
            r = blox[0].r() - 1; // going to top left corner of bounding box
            c = blox[0].c();
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+1,c+1), Coord(r+2, c+1),Coord(r+3, c+1), Coord(r+3, c+2)))
                setOrientation(0);
            break;
    }
}

void Piece_J::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine(" ###");
    screen.gotoXY(x, y+1); screen.printStringClearLine("   #");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////Crazy piece

Piece_Crazy::Piece_Crazy(){
    
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(4);
    
    blox.push_back(Coord(x,y));
    blox.push_back(Coord(x+3,y));
    blox.push_back(Coord(x+1,y+1));
    blox.push_back(Coord(x+2,y+1));
}

void Piece_Crazy::rotateIfOkay(Well& well){
    
    int r;
    int c;
    
    switch(getOrientation()){
        case 0:
            // attempting to go from orientation 0 to 1
            r = blox[0].r(); // going to top left corner of bounding box
            c = blox[0].c();
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r+3,c), Coord(r+2, c+1),Coord(r+2, c+2), Coord(r+3, c+3)))
                setOrientation(1);
            break;
            
        case 1:
            // attempting to go from orientation 1 to 2
            r = blox[0].r() - 3; // going to top left corner of bounding box
            c = blox[0].c();
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c+3), Coord(r+1, c+2),Coord(r+2, c+2), Coord(r+3, c+3)))
                setOrientation(2);
            break;
            
        case 2:
            // attempting to go from orientation 2 to 3
            r = blox[0].r(); // going to top left corner of bounding box
            c = blox[0].c() - 3;
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c), Coord(r+1, c+1),Coord(r+1, c+2), Coord(r, c+3)))
                setOrientation(3);
            break;
            
        case 3:
            // attempting to go from orientation 3 to 0
            r = blox[0].r(); // going to top left corner of bounding box
            c = blox[0].c();
            if(fourBlockRotate(well, blox[0], blox[1], blox[2], blox[3], Coord(r,c), Coord(r+1, c+1),Coord(r+2, c+1), Coord(r+3, c)))
                setOrientation(0);
            break;
    }
}

void Piece_Crazy::leftIfOkay(Well& well){ // goes right instead of left
    genericPiece::rightIfOkay(well);
}

void Piece_Crazy::rightIfOkay(Well& well){ // goes left instead of right
    genericPiece::leftIfOkay(well);
}

void Piece_Crazy::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine("#  #");
    screen.gotoXY(x, y+1); screen.printStringClearLine(" ##");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}


/////////// VAPORBOMB AND FOAMBOMB PIECE IMPLEMENTATIONS

/////vapor bomb

Piece_Vapor::Piece_Vapor(){
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(2);
    m_exploded = false;
    
    blox.push_back(Coord(x+1,y));
    blox.push_back(Coord(x+2,y));
}

void Piece_Vapor::drawPieceInWell(Well& well) const{
    well.modifyPiece_2(blox[0], blox[1], '#');
}

void Piece_Vapor::printPiece(Screen& screen, int x, int y) const{
    screen.gotoXY(x, y); screen.printStringClearLine(" ##");
    screen.gotoXY(x, y+1); screen.printStringClearLine("");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}

void Piece_Vapor::leftIfOkay(Well& well){
    if(!genericPiece::atLeftYet(well)){
        well.modifyPiece_2(blox[0], blox[1], ' '); // clear current piece
        for(int i = 0; i < 2; i++){
            int r = blox[i].r() - 1; // move each block once to the left
            int c = blox[i].c();
            blox[i].changeCoord(r, c);
        }
        well.modifyPiece_2(blox[0], blox[1], '#'); // reset current piece
        setTopLeft(getTopLeft().r(), getTopLeft().c() - 1);
    }
}

void Piece_Vapor::rightIfOkay(Well& well){
    if(!genericPiece::atRightYet(well)){
        well.modifyPiece_2(blox[0], blox[1], ' '); // clear current piece
        for(int i = 0; i < 2; i++){
            int r = blox[i].r() + 1; // move each block once to the left
            int c = blox[i].c();
            blox[i].changeCoord(r, c);
        }
        well.modifyPiece_2(blox[0], blox[1], '#'); // reset current piece
        setTopLeft(getTopLeft().r(), getTopLeft().c() + 1);
    }
}

bool Piece_Vapor::atBottomYet(const Well& well){
    
    if(genericPiece::atBottomYet(well) || m_exploded == true){
        m_exploded = true;
        return true;
    }
    return false;
}

void Piece_Vapor::moveDown(Well& well){
    if(!Piece_Vapor::atBottomYet(well)){
        well.modifyPiece_2(blox[0], blox[1], ' '); // clear current piece
        
        for(int i = 0; i < 2; i++){
            int r = blox[i].r(); // move each block one row down
            int c = blox[i].c() + 1;
            blox[i].changeCoord(r, c);
        }
        well.modifyPiece_2(blox[0], blox[1], '#'); // reset current piece
        setTopLeft(getTopLeft().r() + 1, getTopLeft().c());
    }
}

void Piece_Vapor::addPieceToWell(Well& well){
    
    if(m_exploded){
        int r_v = blox[0].r();
        int c_v = blox[0].c();
        
        for(int i = c_v - 2; i <= c_v + 2; i++){ // remove blocks above and below (up to two rows)
            well.setChar(Coord(r_v, i), ' ');  // set char makes sure '@' boundary is not erased
            well.setChar(Coord(r_v + 1, i), ' ');
        }
    }
    well.modifyPiece_2(blox[0], blox[1], ' ');
}


/////foam bomb

Piece_Foam::Piece_Foam(){
    int x = 4;
    int y = 0;
    
    setTopLeft(x,y);
    setOrientation(0);
    setNumBlocks(1);
    m_exploded = false;
    
    blox.push_back(Coord(x+1,y+1));
}

void Piece_Foam::drawPieceInWell(Well& well) const{
    well.modifyPiece_1(blox[0], '#');
}

void Piece_Foam::printPiece(Screen& screen, int x, int y) const{
    
    screen.gotoXY(x, y); screen.printStringClearLine("");
    screen.gotoXY(x, y+1); screen.printStringClearLine(" #");
    screen.gotoXY(x, y+2); screen.printStringClearLine("");
    screen.gotoXY(x, y+3); screen.printStringClearLine("");
}

void Piece_Foam::leftIfOkay(Well& well){
    
    if(!genericPiece::atLeftYet(well)){
        well.modifyPiece_1(blox[0], ' '); // clear current piece
        
        int r = blox[0].r() - 1; // move each block once to the left
        int c = blox[0].c();
        blox[0].changeCoord(r, c);
        
        well.modifyPiece_1(blox[0], '#'); // reset current piece
        setTopLeft(getTopLeft().r(), getTopLeft().c() - 1);
    }
    
}

void Piece_Foam::rightIfOkay(Well& well){
    if(!genericPiece::atRightYet(well)){
        well.modifyPiece_1(blox[0], ' '); // clear current piece
        
        int r = blox[0].r() + 1; // move each block once to the left
        int c = blox[0].c();
        blox[0].changeCoord(r, c);
        
        well.modifyPiece_1(blox[0], '#'); // reset current piece
        setTopLeft(getTopLeft().r(), getTopLeft().c() + 1);
    }
}

void Piece_Foam::moveDown(Well& well){
    if(!genericPiece::atBottomYet(well)){
        well.modifyPiece_1(blox[0], ' '); // clear current piece
        
        int r = blox[0].r(); // move each block one row down
        int c = blox[0].c() + 1;
        blox[0].changeCoord(r, c);
        
        well.modifyPiece_1(blox[0], '#'); // reset current piece
        setTopLeft(getTopLeft().r() + 1, getTopLeft().c());
    }
}

void Piece_Foam::addPieceToWell(Well& well){
        fillWithFoam(well, blox[0]);
}

bool Piece_Foam::fillWithFoam(Well& well, Coord pos){
    
    if(abs(blox[0].r() - pos.r()) <= 2 && abs(blox[0].c() - pos.c()) <= 2){ // no more than 2 rows and 2 columns away from landing spot
        
        
        if(well.getChar(pos) == ' ' || well.getChar(pos) == '#' ) // filling empty space, as well as where bomb landed
            well.modifyPiece_1(pos, '*'); // first, modify current location since it's in the bounding box
        // getChar will return 'x' if trying to access values outside of well, so no worries about undefined behavior
        else
            return false;
        // current location is not modifiable, foam bomb cannot access areas adjacent to the obstructing block directly
        
        
        if(fillWithFoam(well, Coord(pos.r(), pos.c() + 1)))
            return true;
        if(fillWithFoam(well, Coord(pos.r(), pos.c() - 1)))
            return true;
        if(fillWithFoam(well, Coord(pos.r() + 1, pos.c())))
            return true;
        if(fillWithFoam(well, Coord(pos.r() - 1, pos.c())))
            return true;
        return false; // returns false if no more adjacent areas are accessible/foamable
    }
    
    return false;
}




