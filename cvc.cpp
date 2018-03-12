#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ccdoamove.h"

using namespace std;

ofstream marius("log"); //writes things in a log
int draw = 0, fwin = 0, swin = 0; //counts what is happening


  /**
    @brief checks if the game is over

    @param feld is the field you wanna check.
    @param checkfor means check for what: 1 -> Player 1 | 2 -> Player 2 | 3 -> Draw [-- why checkfor draw? 9 turns no winner => draw]

    @returns true if game is over
  */
bool over(vector<int> feld, int checkfor){

  int win[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}}; // these are the winconditions
  if(checkfor == 3){ // this will check for a draw
    for(int i = 0; i < 9; i++){
      if(feld[i] == 0){return false;}
    }
    return true;
  }else{
    for(int i = 0; i < 8; i++){ // these will check if anyone wins
      if(feld[win[i][0]] == checkfor && feld[win[i][1]] == checkfor && feld[win[i][2]] == checkfor){return true;}
    }
  }
  return false;
}
  /**
    @bried here the turns happen

    @param feld is the field you wanna play on.
    @param player is the bool pointer i dont really needed [-grammar]

    @returns the field after the turn
  */
vector<int> zug(vector<int> feld, bool* player){

  *player = !*player;
  int* newfield = new int(0); // needed for the log
    feld = cmove(feld, *player, newfield);
    marius << *newfield << "_"; // here the turn is added to the log
    delete newfield; // and its gone
    return feld;
}
  /**
    @brief her the game is played

    @param field is the tick tack toe field
  */
void play(vector<int> field){

  vector<int> feld = field; //now the feld is our field. i think that this step is not needed, but i do it anyways
  string p1 = "X", p2 = "O"; //symbols
  bool* player;
  player = new bool(true);
  /*
   player true if first player false if second player. I just realized, that a pointer is not needed,
   cause i can change its value right here. I will leave it like that.
  */
  for(int i = 0; i < 9; i++){
    feld = zug(feld, player); // here the field gets changed
    if(over(feld,1)){fwin++; marius << "FPW" << endl; break;} // check win for Player 1
    if(over(feld,2)){swin++; marius << "SPW" << endl; break;} // check win for Player 2
    if(over(feld,3)){draw++; marius << "DRW" << endl; break;} // check if it is a Draw
  };
}

int main(int argc, char const *argv[]) {
  srand(time(NULL));
  for(int i = 0; i < 10000; i++){
    vector<int> field = {0,0,0,0,0,0,0,0,0};
    int a = rand() % 9;
    field[a] = 1; // sets first step random
    marius << a+1 << "_";
    play(field);
  }

  marius << endl << endl << "FPW: " << fwin << " DRW: " << draw << " SPW: " << swin;
  return 0;
}
