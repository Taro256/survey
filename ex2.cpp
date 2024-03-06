//
//  ex2.cpp
//  PlayingCard
//

#include <iostream>
#include <string>

#include "card.h"
#include "cardset.h"
#include "player.h"
#include "gamestatus.h"
#include "ex2.h"

void ex2::ready() {
    trump.clear();
    hand.sort(myCardCmp);
}

//大富豪におけるカードの切り方のアルゴリズム
bool ex2::follow(const GameStatus &gstat, CardSet &cs) {
    CardSet pile(gstat.pile);
    Card tmp;
    int leadSize, weak = 0, strong = 0;

    hand.sort(myCardCmp);  // 手札をソート(弱い方から順に)

    leadSize = pile.size();

//　弱いカードの枚数を数える(weakに枚数を保存)
    for(int i = 0; i < hand.size(); i++) {
        if(hand.at(i).rank() < 5){
            if(hand.at(i - 1).rank() != hand.at(i).rank() || hand.at(i).rank() != hand.at(i + 1).rank()) {
                  weak++;
            } 
        }
    }

//　強いカードの枚数を数える(strongに枚数を保存、ジョーカーによる複数枚出しには非対応です。
    for(int i = 0; i < hand.size(); i++) {
        if(hand.at(i - 1).rank() != hand.at(i).rank() || hand.at(i).rank() != hand.at(i + 1).rank()) { // 1枚出しは「2」以上で強いカードと認識
            if(hand.at(i - 1).rank() == hand.at(i).rank()) { // 2枚出しは「A」以上で強いカードと認識
                if(hand.at(i).rank() == hand.at(i + 1).rank()) { // 3枚出しは「８」以上で強いカードと認識
                    if(hand.at(i).rank() == hand.at(i + 2).rank()) { // 4枚出しはなんの数字でも強いカードと認識
                      i+=2;                  
                      strong++;
                      break;
                    }
                    
                    i++;
                    if(hand.at(i).rank() > 6) {
                      strong++;
                      break;
                    }
                }
                if(hand.at(i).rank() > 12) {
                  strong++;
                  break;
                }
            }
           if(hand.at(i).rank() > 13) {
              strong++;
              break;
            }
         }
     }

    if (leadSize == 0) { // 自分がリーダなら
      for(int i = 0; i < hand.size(); i++) {
        if(weak < strong) {
	    	  for (int j = 1; i + j < hand.size(); j++) {
		        if (hand.at(i).rank() == hand.at(i + j).rank()) {
               cs.insert(hand.at(i + j));
	          }
          }
         cs.insert(hand.at(0));
	      }
      }

      if(hand.size() == 3 && hand.at(0).rank() == hand.at(1).rank()){ 
	      tmp = hand.at(1);
	      cs.insert(tmp);
	      hand.remove(tmp);
	      return true;
	    }
      for (int i = 0; i < hand.size(); i++) {
        tmp = hand.at(i);
	      if(tmp.rank() != 2 && tmp.rank() != 1) {
	        cs.clear();
          cs.insert(tmp);
	        if(hand.at(i).rank() < 7){
            for (int j = 1; i + j < hand.size(); j++) {
              if (hand.at(i).rank() == hand.at(i + j).rank()) {
                cs.insert(hand.at(i + j));
              }
            }
            if(cs.size() > 1) {
              hand.remove(cs);
              return true;
            }
          } else {
            for(int j = 1; i + j < hand.size(); j++) {
              if(hand.at(i).rank() == hand.at(i + j).rank() || hand.at(i + j).rank() == 15) {
                cs.insert(hand.at(i + j));
              }
            }
            if(cs.size() > 1) {
              hand.remove(cs);
              return true;
            } 
          }
	      }
      }
	
      if(hand.size() == 2) {
        tmp = hand.at(1);
        cs.insert(tmp);
        hand.remove(tmp);
        return true;
      } else {
        cs.clear();
        tmp = hand.at(0); // 一番弱いカードを1枚出す
        cs.insert(tmp);
        hand.remove(tmp);
        return true; 
      }
	
    } else if (leadSize == 1) { // リードが1枚の場合
      for(int i = 0; i < hand.size(); i++) {
        if(weak < strong) {
          cs.insert(hand.at(hand.size() - 1));
	      }
      }
      for(int i = 0; i < hand.size(); i++) {
        cs.clear();
        tmp = hand.at(i);
        if(tmp.isGreaterThan(pile.at(0)) && tmp.rank() != 15) {
          cs.insert(tmp);
          if(hand.size() > 8) {
            if(i != hand.size() - 1) {
              if(hand.at(i).rank() != hand.at(i - 1).rank() && hand.at(i).rank() != hand.at(i + 1).rank()) {
                hand.remove(tmp);
                return true;
              }
            }
          } else if(hand.size() == 3 && hand.at(0).rank() == hand.at(1).rank()) {
            tmp = hand.at(2);
            if(tmp.isGreaterThan(pile.at(0))) {
              hand.remove(tmp);
              return true;
            }
          } else if(hand.size() == 3 && hand.at(1).rank() == hand.at(2).rank()) {
            hand.remove(tmp);
            return true;
          } else if(hand.size() > 2){
            if(i != hand.size() - 1) {
              hand.remove(tmp);
              return true; 
            }
          } else {
            if(i == hand.size() - 1) {
              hand.remove(tmp);
              return true;
            }
          }
        }
      }
      
    } else { // 2枚以上の場合
      for (int i = 0; i < hand.size(); i++) {
        tmp = hand.at(i);
        if (tmp.isGreaterThan(pile.at(0)) || tmp.isGreaterThan(pile.at(1))) { 
          cs.clear();
          cs.insert(tmp);
          for (int j = 1; j < leadSize && i + j < hand.size(); j++) {
            if (hand.at(i).rank() == hand.at(i + j).rank() || hand.at(i + j).rank() == 15)
              cs.insert(hand.at(i + j));
          }
          if (cs.size() == leadSize) { // 出せるカードが揃った
            hand.remove(cs); // カード集合を remove 
            return true;
          }
        }
      }
    }
  // 出せるカードがないのでパス
  cs.clear();
  return true;
}

bool ex2::approve(const GameStatus &gstat) {


    CardSet pile(gstat.pile);
    Card tmp;

    for(int i = 0; i < pile.size(); i++) {
      tmp = pile.at(i);

      if(trump.size() == 0) {
        trump.insert(tmp);
      } else {
        if(!trump.includes(tmp)) {
         trump.insert(tmp);
        }
      }
    }
    
    return true;
}
