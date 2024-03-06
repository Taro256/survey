//
//  simpleplayer.h
//  PlayingCard
//

#ifndef _EX2_H_
#define _EX2_H_

#include <random>

#include "player.h"
#include "gamestatus.h"

//
// Player クラスを継承して作成するクラスのサンプル
//
class ex2 : public Player {
    CardSet memory;  // 色々覚えておくための作業用
    CardSet trump;   // 同上

public:
    ex2(const char *name = "ex2") : Player(name) {
      
    }
    ~ex2() { }

    // 思考処理の関数：このクラスで実装する
    void ready(void);
    bool follow(const GameStatus &, CardSet &);
    bool approve(const GameStatus &);

    // 比較関数(ソート用)
    static bool myCardCmp(const Card &a, const Card &b) {
      return a.strength() <= b.strength();
    }

};

#endif

