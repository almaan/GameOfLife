#include <iostream>
#include <ctime>
#include <unistd.h>
#define HEIGHT 10
#define WIDTH 10
#define DELAY 0.5


struct Shape{
public:
  char xCoord;
  char yCoord;
  char height;
  char width;
  char **figure;

};

struct Glider : public Shape {
  // static member shared by all instances
  // const prevents any change
  static const char GLIDER_SIZE = 3;
  // constructor
  Glider( char x, char y);
  //destructr
  ~Glider();

};

// struct Blinker : public Shape{
//   static const char BLINKER_HEIGHT = 3;
//   static const char BLINKER_WIDTH = 3;
//   Blinker( char x , char y);
//   ~Blinker();

// };

class GameOfLife{
public:
  // constructor
  GameOfLife( Shape sh);
  // print state
  void print(int num);
  // update state
  void update();
  // function to get state information
  char getState( char state, char xCoord, char yCoord);
  // we know iterations will always be positive
  void iterate(unsigned int iterations);
private:
  // first world
  char world[HEIGHT][WIDTH][2];
  // toggling between worlds
  bool toggle;
  // shape being used
  Shape shape;

};


GameOfLife::GameOfLife( Shape sh) :
  shape(sh),
  toggle(true)
{

  // fill world initially
  for ( char i = 0; i < HEIGHT; i++)
    {
    for (char j = 0; j < WIDTH; j++)
      {
        world[i][j][int(!toggle)] = '.';
    }
  }

  // add shape to world
  for ( char i = shape.yCoord; i - shape.yCoord < shape.height; i++)
    {
    for (char j = shape.xCoord; j - shape.xCoord < shape.width; j++)
      // make sure we are within bound of world
      if (i < HEIGHT && j < WIDTH)
        {
          {
            world[i][j][0] = shape.figure[i - shape.yCoord][j-shape.xCoord];
          }
        }
  }

};

void GameOfLife::print(int num = 1){
  // check if we are printing for
  // first time or not
  if (num > 0){
    // erase previously printed system
    for (int i = 0; i < HEIGHT+1; i++)
      {
        printf("\033[1A");
      }
  }

  // if first world is the active one
  for (char i = 0; i < HEIGHT; i++)
    {
      for ( char j = 0; j < WIDTH; j++ )
        {
          std::cout << world[i][j][int(!toggle)];
        }
      std::cout << std::endl;
    }
  // print bottom border
  for ( char i = 0; i < WIDTH; i++)
    {
      std::cout << '=';
    }
  std::cout << std::endl;

};


void GameOfLife::update(){
  // if first world is active
  for (char i = 0; i < HEIGHT; i++)
    {
      for (char j = 0; j <WIDTH; j++)
        {
          world[i][j][int(toggle)] = GameOfLife::getState(world[i][j][int(!toggle)],i,j);
        }
    }

  // change active world
  toggle = !toggle;
}

char GameOfLife::getState( char state, char yCoord, char xCoord){
  char neighbors = 0;
  // iterate over neighborhood
  for (char i = yCoord -1; i <= yCoord +1; i++)
    {
      for (char j = xCoord -1; j <= xCoord +1; j++)
        {
          // do not count self
          if (i == yCoord && j == xCoord){
              continue;
          } else {
            // add count if position is "alive"
            if (i > -1 && i < HEIGHT && j > -1 && j < WIDTH ){
              if (world[i][j][int(!toggle)] == 'X') {
                neighbors++;
              }
            }
          }
        }
    }
  // if current cell is alive
  if (state == 'X'){
    return ( neighbors > 1 && neighbors < 4) ? 'X' : '.';
    // if current cell is dead
  } else {
    return (neighbors == 3) ? 'X' : '.';
  }
}

void GameOfLife::iterate( unsigned int iterations){
  // propagate system in time
  for (int i = 0; i < iterations; i++ )
    {
      // print system
      print(i); 
      // update system
      update();
      // delay, for "animation"
      usleep(DELAY * 1000000);
    }
}

// glider character
Glider::Glider( char x, char y) {
  // center coordinates
  xCoord = x;
  yCoord = y;
  // width and height
  height = GLIDER_SIZE;
  width = GLIDER_SIZE;
  // array to hold glider figure
  // first index is y-coordinate
  // second index is x-coordinate
  figure = new char*[GLIDER_SIZE];

  for ( char i = 0; i < GLIDER_SIZE; i++)
    {
      // make dynamic array
      figure[i] = new char[GLIDER_SIZE];
    }

  // fill glider initially
  for ( char i = 0; i < GLIDER_SIZE; i++)
    {
      for ( char j = 0; j < GLIDER_SIZE; j++)
        figure[i][j] = '.';
    }
  // add alive cells to glider
  figure[0][1] = 'X';
  figure[1][2] = 'X';
  figure[2][0] = 'X';
  figure[2][1] = 'X';
  figure[2][2] = 'X';

}
// destructor
Glider::~Glider(){
  // ensure dynamic arrays are deleted
  for (char i = 0; i < GLIDER_SIZE; i++){
    delete[] figure[i];
  }
  delete[] figure;
}

int main(){

  int x,y;
  bool badXstate = true;
  bool badYstate = true;
  int nIter = 0;

  // user defined input
  while (badXstate){
    std::cout << "Please enter x-coordinate >> ";
    std::cin >> x;
    badXstate = (x > 0 && x < WIDTH) ? false : true;
  }
  while (badYstate){
    std::cout << "Please enter y-coordinate >> ";
    std::cin >> y;
    badYstate = (y > 0 && x < HEIGHT) ? false : true;
  }
  while (nIter < 1){
    std::cout << "Please number of iterations >> ";
    std::cin >> nIter;
  }

  std::cout << std::endl;

  Glider glider(x = x,y = y);
  GameOfLife gol(glider);
  gol.iterate(nIter);
  return 0;
}
