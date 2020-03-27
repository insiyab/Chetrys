#include "Piece.h"
#include "Well.h"
#include "UserInterface.h"

#include <string>

#include <iostream>
//#include <vector>
using namespace std;


Well::Well(const int & width, const int & height) : m_height(height+ 1), m_width(width + 2) {                  //constructor
    m_array = new string[m_height];                                //create 2D array for well
    
    for(int i = 0; i < m_height; i++)                              //making sure we're not assigning to an empty string
        m_array[i] = "                ";                           //string is as large as the width of the well
    clearWell();
}

Well::~Well(){                                                  //destructor
    delete [] m_array;                                          //delete empty columns
}

void Well::display(Screen& screen, int x, int y) const {
    
    screen.refresh();
    
    for(int i = 0; i < m_height; i++){
        screen.gotoXY(x, y + i);
        screen.printString(m_array[i]);
    }
    
}

int Well::height() const {
    return m_height;
}

int Well::width() const {
    return m_width;
}

char Well::getChar(const Coord& co) const{
    if(co.c() < 0 || co.c() > m_height - 1)
        return 'x';
    if(co.r() < 0 || co.r() > m_width - 1)
        return 'x';
    
    return m_array[co.c()][co.r()];
}

char Well::getChar(int x, int y) const {
    if(y < 0 || y > m_height - 1)
        return 'x';
    if(x < 0 || x > m_width - 1)
        return 'x';
    
    return m_array[y][x];
}

bool Well::setChar(const Coord& co, char c){
    if(co.c() < 0 || co.c() > m_height - 1)
        return false;
    if(co.r() < 0 || co.r() > m_width - 1)
        return false;
    if(getChar(co) == '@')
        return false;
    
    m_array[co.c()][co.r()] = c;
    return true;
}

bool Well::setChar(int x, int y, char c){
    if(y < 0 || y > m_height - 1)
        return false;
    if(x < 0 || x > m_width - 1)
        return false;
    if(getChar(x, y) == '@')
        return false;
    
    m_array[y][x] = c;
    return true;
}

void Well::modifyPiece(const Coord& zero, const Coord& one, const Coord& two, const Coord& three, char c){
    //this function can be used to redraw the piece by clearing out the old image with ' ', creating new image with '#' or '$'
    m_array[zero.c()][zero.r()] = c;
    m_array[one.c()][one.r()] = c;
    m_array[two.c()][two.r()] = c;
    m_array[three.c()][three.r()] = c;
}

void Well::modifyPiece_2(const Coord& zero, const Coord& one, char c){
    m_array[zero.c()][zero.r()] = c;
    m_array[one.c()][one.r()] = c;
}

void Well::modifyPiece_1(const Coord& zero, char c){
    m_array[zero.c()][zero.r()] = c;
}

void Well::clearWell(){
    for(int i = 0; i < m_height; i++){                          //set up display
        for(int j = 0; j < m_width; j++){
            
            if( j == 0 || j == m_width-1 || i == m_height - 1)  //set up @ on boundaries
                m_array[i][j] = '@';
            else
                m_array[i][j] = ' ';                            //spaces everywhere else
        }
    }
}


//the following functions don't work as expected

void Well::downRow(int intRow){
    
    for(int i = intRow; i > 0; i--){
        for(int j = 1; j < m_width - 1; j++){
            m_array[i][j] = m_array[i-1][j];
        }
    }
    
};

int Well::numRowsCompleted(){
    
    //checks to see if there are completed rows, removes them, then returns the number of rows removed
    
    int clearedCount = 0;
    bool okToDelete = true;

    for(int i = m_height - 2; i > 0 ; i--){                          //set up display
        for(int j = 1; j < m_width - 1; j++){
            if(m_array[i][j] == ' '){
                okToDelete = false;
            }
        }
        
        if(okToDelete){
            clearedCount++;
            downRow(i);
            i++; //don't allow decrementation
        }
        
        okToDelete = true;
    }
    
    return clearedCount;
    
}



