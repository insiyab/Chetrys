#ifndef WELL_INCLUDED
#define WELL_INCLUDED

#include <string>

class Screen;
class Coord;

class Well
{
public:
    
    Well(const int & width, const int & height);
    ~Well();
    void display(Screen& screen, int x, int y) const;
    int height() const;
    int width() const;
    char getChar(int x, int y) const;
    char getChar(const Coord& p) const;
    bool setChar(const Coord& co, char c);
    bool setChar(int x, int y, char c);
    
    void modifyPiece(const Coord& zero, const Coord& one, const Coord& two, const Coord& three, char c);
        //clear a single piece and redraw it or make it part of the stack ($)
        //will need to make special functions for vapor and foam bombs (2 and 1 arg modify pieces)
    void modifyPiece_2(const Coord& zero, const Coord& one, char c);
    void modifyPiece_1(const Coord& zero, char c);
    
    void clearWell(); //clears well entirely
    
    void downRow(int intRow); //clears a row and moves down rest of image
    
    int numRowsCompleted();
    
    
private:
    std::string* m_array; //each string is a row (strings are arrays! they're also included in other parts of the project)
    int m_height; // one plus height of open space
    int m_width;  // two plus height of open space
    
};

#endif // WELL_INCLUDED



