///@file ccdoamove.h
#ifndef CDOAMOVE_G
#define CDOAMOVE_G

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

double value = 0;
int me;
int eneme;

bool itsover(vector<int> feld, int checkfor){
  // this is exactly the same function like over in the .cpp file
  int win[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
  if(checkfor == 3){
    for(int i = 0; i < 9; i++){
      if(feld[i] == 0){return false;}
    }
    return true;
  }else{
    for(int i = 0; i < 8; i++){
      if(feld[win[i][0]] == checkfor && feld[win[i][1]] == checkfor && feld[win[i][2]] == checkfor){return true;}
    }
  }
  return false;
}
  /**
    @brief only does something if game is over. else it just returns true

    @param the field you play on.
    @param inst is complex to explain, so i will skip that [- don't...], but the higher it is, the lower is the effect on value
  */
bool checkit(vector<int> field, int inst){
  if(itsover(field,eneme)){value -= 1.0/inst; return false;} // if the enemy wins value will get smaller
  if(itsover(field,me)){value += 1.0/inst; return false;} // if i win value will get bigger
  if(itsover(field,3)){return false;} // nothing happens at a Draw
  // will return true only if the game can go on
  return true;
}
  /**
    @brief will be called until the all possible games are played. Changes the value if possible win/loose

    @param the field you play on.
    @param inst gets higher every time you call doit from doit. you will see why in checkit
    @param player [- missing]
  */
void doit(vector<int> field, int inst, int player){

  if(player == me){player = eneme;}else{player = me;} //changes what player could do a move
  for(int i = 0; i < 9; i++){
    if(field[i] == 0){ // checks if move is possible
      field[i] = player;
      if(checkit(field, inst)){doit(field, inst*10, player);} // calls checkit. if game could go on it calls doit again
      field[i] = 0;
    }
  }
}

  /**
    @bried here the evaluation starts

    @param field [-you missed that] again the field you play on

    @returns the value of that move
  */
double howgoodisit(vector<int> field){
  value = 0;
 doit(field, 1, me);
 cout << value << endl;
 return value;
}

  /**
    @brief wanna find out the best move

    @param field is the field you play on.
    @param player [- not bool] true for player 1 false for player 2.
    @param newfield is needed for the log

    @returns the field with the new move
  */
vector<int> cmove(vector<int> field, bool player, int* newfield){
  if(player){me = 1; eneme = 2;}else{me = 2; eneme = 1;} // sets me and eneme
  double best = -10000000.0; int bestfield; value = 0;
  for(int i = 0; i < 9; i++){ // goes throught every possible move
    if(field[i] == 0){ // checks if move is possible
      field[i] = me;
      if(itsover(field,me)){return field;}else{if(!itsover(field,eneme)){ // checks if move is an instant win
        cout << i+1 << " "; // cout the field. after that you will see the value in your console
        if(howgoodisit(field) > best){best = howgoodisit(field); bestfield = i; field[i] = 0;}else{field[i] = 0;} // searchs the best field
      }}
      field[i] = 0;
    }
  }
  field[bestfield] = me; // does the best move
  *newfield = bestfield+1; // for the log
  cout << "\033[38;5;18m" << bestfield << "\033[0m" << endl; // couts the best move in a beautiful color
  return field;
}

#endif
