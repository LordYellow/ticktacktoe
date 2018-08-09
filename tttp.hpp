#ifndef TTP_HPP
#define TTP_HPP

#include <array>
#include <iostream>
#include <algorithm>
#include <vector>

int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};

int didSomebodyWin(std::array<int, 9> field){
    for(int u = 0; u < 8; u++){
        if(field[wins[u][0]] == field[wins[u][1]] && field[wins[u][0]] == field[wins[u][2]] && field[wins[u][0]] != 0){
            return field[wins[u][0]];
        }
    }
}

bool isItATie(std::array<int, 9> field){
    for(int i = 0; i < 9; i++){
        if(!field[i]) return false;
    }
    return true;
}

int iseven(int i){
    int a = i/2;
    if(i == a*2) return 1;
    return -1;
}

bool compsmaller(std::pair<int, int> a, std::pair<int, int> b){
    return (a.first < b.first);
}

bool compbigger(std::pair<int, int> a, std::pair<int, int> b){
    return (a.first > b.first);
}

int bestTurn(std::array<int, 9> field, bool turn, bool giveMeTheTurn){
    if(didSomebodyWin(field)){return iseven(turn);}
    if(isItATie(field)){return 0;}
    std::vector<std::pair<int, int>> possibleturns;
    for(int i = 0; i < 9; i++){
        if(field[i] == 0){
            field[i] = turn+1;
            possibleturns.push_back(std::pair<int, int>(bestTurn(field, !turn, false),i));
            field[i] = 0;
        }
    }
    
    if(turn){
        std::sort(possibleturns.begin(), possibleturns.end(), compbigger);
    }else{
        std::sort(possibleturns.begin(), possibleturns.end(), compsmaller);
    }
    
    if(!giveMeTheTurn){
        return possibleturns[0].first;
    }else{
        return possibleturns[0].second;
    }
}

int getAiMove(std::array<int, 9> field, int aiNumber, int humanNumber){
    int t = bestTurn(field, true, true);
    return t;
}

#endif 
