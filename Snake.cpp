#include <NonBlockingGame.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>

//
//  The purpose of this assignment is to learn to 1. The classic game of Snake.
//  Move a "snake" (line of sprites) along a 2D grid attempting to run over a
//  randomly placed object to help it grow. 2. If it runs into itself the game is over
//  and the player has lost. 3. The object of the game is to make the snake as big as possible.
//
using namespace bridges::game;
using namespace std;

// this keeps a list of elements (squares) that represents the snake object
// linked list since it can grow as the game progresses
class Block {
  public:
    Block *next = nullptr;
    int x;
    int y;
    Block() {
      x = 0;
      y = 0;
    }
    Block(int x, int y) {
      this->x = x;
      this->y = y;
    }
};

struct my_game : public NonBlockingGame {
  long frameTime;
  long nextFrameTime;

    //TODO: create a variable to represent an apple, what type should it be?
  Block apple = Block();
    //TODO: create a variable to represent the head, what type should it be?
  Block head = Block();

  enum Direction {East, West, South, North};
    
    //TODO: create a variable to store the last direction and initialize it to east
  Direction lastDir = East;
    //TODO: create a variable to store the current direction and initialize it to east
  Direction currentDir = East;

  int frame = 0;

  my_game(int assID, std::string username, std::string apikey)
    : NonBlockingGame (0, username, apikey, 30, 30) {
  }

  virtual void initialize() override {
    srand(time(NULL));

      //TODO: initialize the head to a new block starting at the point 15, 15
    head = Block(15, 15);

      //TODO: create a pointer to head
    Block * current = &head;

      //This code draws the rest of the snake
      //TODO: what color would you like your snake? The default is silver as you can see
    for (int i = 0; i < 15; i++) {
      setBGColor(15 - i, 15, NamedColor::blueviolet);

      if (i > 0) {
        current->next = new Block(15 - i, 15);
        current = current->next;
      }
    }

      //TODO: set the current direction to east
      currentDir = East;
      //TODO: set the last direction equal to the current direction
      lastDir = currentDir;
      //TODO: initialize an apple to a block at 0,0
      apple = Block(0,0);
      //TODO: plant the apple
      plantApple();
      //TODO: paint the screen
      paint();
  }

  void handleInput() {
      //TODO: test which key was pressed (ex: keyLeftJustPressed())
      //if the user pressed left, change the current direction to west as long as the
      //the last direction does not equal east...the snake can't just flip
      //Do for up, down, left, right
    if (keyUpJustPressed()) {  
      if (lastDir != South) {  //check for last direction, making sure snake cannot turn 180 degrees
        currentDir = North;    //up key sets snake to north
      }
    }
    if (keyDownJustPressed()) {
      if (lastDir != North) {   //check for last direction
        currentDir = South;     //down key sets snake to south
      }
    }
    if (keyLeftJustPressed()) {
      if (lastDir != East) {   //check for last direction
        currentDir = West;     //left key sets snake to west
      }
    }
    if (keyRightJustPressed()) {
      if (lastDir != West) {    //check for last direction
        currentDir = East;      //right key sets snake to east
      }
    }
  }

  // update snake position
  void updatePosition() {
    Block * current = head.next;
    int nextX = head.x;
    int nextY = head.y;
    Block nextPos = head;

    //change the x and y values of the head, depending on the current direction of the snake
    if (currentDir == North) {
      //for some reason, the x and y axis are flipped in my game, so I adjusted the addition/subtraction accordingly
      if (head.x-1  < 0) {
        //if head goes out of frame (up), set head equal to lowest block in frame, looping the snake
        head.x = 29;
      }
      else {
        //otherwise, set head equal to one block up
        head.x -= 1;
      }
    }
    if (currentDir == South) {
      if (head.x+1 >= 30) {
        //if head goes out of frame (down), set head equal to highest block in frame
        head.x = 0;
      }
      else {
        //set head equal to one block down
        head.x += 1;
      }
    }
    if (currentDir == East) {
      if (head.y+1 >= 30) {
        //if head goes out of frame (right), set head equal to left-most block in frame
        head.y = 0;
      }
      else {
        //set head equal to one block to the right
        head.y += 1;
      }
    }
    if (currentDir == West) {
      if (head.y-1 < 0) {
        //if head goes out of frame (left), set head equal to right-most block in frame
        head.y = 29;
      }
      else {
        //set head equal to one block to the left
        head.y -= 1;
      } 
    }
    
      //TODO: loop through the snake's body setting each node's (x,y) to the next node's (x,y)
      //TODO: handle edge cases
      //hint: a switch statement on the current direction would be really helpful
    while (current != nullptr) {
      nextPos = *current;   //nextPos equals dereferenced current pointer (snake block in front of nextPos)
      current->x = nextX;   //set current x and y values to x and y of snake block in front of current
      current->y = nextY;   
      nextX = nextPos.x;    //set nextX and nextY equal to x and y values of the snake block ahead
      nextY = nextPos.y;    
      current = current->next;   //current equals next block in snake
    } 
  }

  // locate apple at new position
  void plantApple() {
    int x;
    int y;

      //TODO: get a new random x and a random y for the apple
      srand(time(NULL));
      x = rand() % 30;
      y = rand() % 30;
      //loop through the snake to ensure that the apple's (x,y) does not
      //intersect with any part of the snake, if it does, get new random values to test
      Block * current = &head;
      while (current != nullptr) {
        if (current->x == x && current->y == y) {  //check if any part of the snake intersects with randomly found values
          plantApple();     //if it does, start process over, finding new values
        }
        current = current->next;
      }
      //if it doesn't collide with the snake, set the apple's x and apple's y to the random
      //values that were identified
      apple.x = x;
      apple.y = y;
  }

  // check if snake has found the apple
  void detectApple() {
      //Tests to see if the snake has hit the apple
      //Erases the apple
    if (head.x == apple.x && head.y == apple.y) {
      drawSymbol(apple.x, apple.y, NamedSymbol::none, NamedColor::red);
        //TODO: Make the snake longer by adding a new block to the end of the snake
      Block * current = head.next;
      int previousX = head.x;
      int previousY = head.y;
      while (current != nullptr) {
        //find x and y values of last block in the snake
        previousX = current->x;
        previousY = current->y;
        //set current equal to nullptr block past the last block in the snake
        current = current->next;
      }
      //current = new Block();
      //check the current direction, setting nullptr block equal to a new block with coordinates 1 past the last block
      if (currentDir == North) {
        if (previousX+1 >= 30) {
          current = new Block(0, previousX);
          //current->x = 0;
          //current->y = previousY;
        }
        else {
          current = new Block(previousX+1, previousY);
          //current->x = previousX+1;
          //current->y = previousY;
        }
      }
      if (currentDir == South) {
        if (previousX-1 < 0) {
          current = new Block(29, previousY);
          //current->x = 29;
          //current->y = previousY;
        }
        else {
          current = new Block(previousX-1, previousY);
          //current->x = previousX-1;
          //current->y = previousY;
        }
      }
      if (currentDir == East) {
        if (previousY-1 < 0) {
          current = new Block(previousX, 29);
          //current->x = previousX;
          //current->y = 29;
        }
        else {
          current = new Block(previousX, previousY-1);
          //current->x = previousX;
          //current->y = previousY-1;
        }
      }
      if (currentDir == West) {
        if (previousY+1 >= 30) {
          current = new Block(previousX, 0);
          //current->x = previousX;
          //current->y = 0;
        }
        else {
          current = new Block(previousX, previousY+1);
          //current->x = previousX;
          //current->y = previousY+1;
        }
      }
      //current = current->next;
      //Plant a new apple
      plantApple();
    }
  }

  // check if snake ate itself! Yuk!
  void detectDeath() {
    Block * current = head.next;
    while (current != nullptr) {
      if (head.x == current->x && head.y == current->y) {
        // TODO: need to deallocate storage!
        Block * deleteCurrent = head.next;
        while (deleteCurrent != nullptr) {
          Block *next = deleteCurrent->next;
          delete deleteCurrent;
          deleteCurrent = next;
        }
        exit(0);
      }
      current = current->next;
    }
  }


  // redraw
  void paint() {
      //Paints the board by looping through the rows and columns
      //Optional TODO: change the color of the board
    for (int i = 0; i < 30; i++) {
      for (int j = 0; j < 30; j++) {
        if (i % 2 == j % 2) {
          setBGColor(i, j, NamedColor::green);
        }
        else {
          setBGColor(i, j, NamedColor::forestgreen);
        }
      }
    }

      //Optional TODO: change the color of the head of the snake
    setBGColor(head.x, head.y, NamedColor::lightblue);

      //Optional TODO: use a different symbol or color
    drawSymbol(apple.x, apple.y, NamedSymbol::apple, NamedColor::red);

      //Paints the snake
      //Optional TODO: change the color of the snake
    Block * current = head.next;
    while (current != nullptr) {
      setBGColor(current->x, current->y, NamedColor::blueviolet);
      current = current->next;
    }
  }

  // handle input, check if apple was detected, update position, redraw,
  // detect if snake ate itself
  virtual void gameLoop() override {
    frame += 1;
    handleInput();
    if (frame >= 5) {
      lastDir = currentDir;
      detectApple();
      updatePosition();
      paint();
      detectDeath();
      frame = 0;
    }
  }
};

// Initialize your game
// Call your game class with your assignment id, username, and api key
int main (int argc, char** argv) {
  my_game g(1, "amforeman", "296437206254");

  g.start();
}
