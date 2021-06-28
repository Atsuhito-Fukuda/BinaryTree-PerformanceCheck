#include <iostream>
#include "BinaryTree_comparetest.h"

/*

    ・書き換え・書き足しが多いので、Treeの登録・消去・更新と衝突処理が機能している
    　かの簡易チェックを用意。(上記のチェックにとどまる。)

*/

/***************************************************************************
    Tree test
****************************************************************************/

int simpletreetest/*main*/() { //Treeの登録・削除・更新を簡易確認(目視確認)

    //BinaryTree<unsigned int>& bin(*new BinaryTree<unsigned int>(3)); //八分木
    BinaryTree_comparetest bin;

    BinaryTree<unsigned int>::NodeInfo info[4];


    bin.testmode();

    bin.Register( //4bit分共通の場合("000.000.111.011")
        info[0], 10,
        std::vector<char>{0b00101101, 0b00110000, 0b00110000},
        std::vector<char>{0b00100100, 0b00110000, 0b00110000}
    );
    bin.Register( //8bit分共通の場合("010.010.110.010.110.110.010.111")
        info[1], 13,
        std::vector<char>{0b00101101, (char)0b11111111, 0b00000001},
        std::vector<char>{0b00101101, (char)0b11111111, 0b00000001}
    );
    bin.Register( //"13"の途中の枝に入れる("010.010.110.010")
        info[2], 2001,
        std::vector<char>{0b00101101, (char)0b11111111, 0b00000001},
        std::vector<char>{0b00100101, (char)0b11111111, 0b00000001}
    );
    bin.Register( //"10"の流れから途中分岐("000.001")
        info[3], 121,
        std::vector<char>{0b00101100, 0b0, (char)0b01100101},
        std::vector<char>{0b00101100, 0b00100000, (char)0b01100101}
    );

    bin.Showresult();

    // 枝の分岐番号を先頭から8進法(8桁まで)で並べてノードを表現する

    /*
    [0073] : 10
    [007] :
    [00] :
    [01] : 121
    [0] :
    [22626627] : 13
    [2262662] :
    [226266] :
    [22626] :
    [2262] : 2001
    [226] :
    [22] :
    [2] :
    [] :
    (whole node number : 14)
    */

    bin.Register( //再登録("100")
        info[0], 10,
        std::vector<char>{(char)0b10101101, 0b01110000, 0b00110000},
        std::vector<char>{(char)0b10100100, 0b00110000, 0b00110000}
    );

    bin.Showresult();

    /*
    [0073] :
    [007] :
    [00] :
    [01] : 121
    [0] :
    [22626627] : 13
    [2262662] :
    [226266] :
    [22626] :
    [2262] : 2001
    [226] :
    [22] :
    [2] :
    [4] : 10
    [] :
    (whole node number : 15)
    */

    bin.Remove(info[0]); //一部のみ削除
    bin.Remove(info[1]);

    bin.Showresult();

    /*
    [0073] :
    [007] :
    [00] :
    [01] : 121
    [0] :
    [22626627] :
    [2262662] :
    [226266] :
    [22626] :
    [2262] : 2001
    [226] :
    [22] :
    [2] :
    [4] :
    [] :
    (whole node number : 15)
    */

    bin.Update(); //伐採

    bin.Showresult();


    if ((mode == Mode::compare2) || (mode == Mode::compare4)) {

        /*
        [0073] :
        [007] :
        [00] :
        [01] : 121
        [0] :
        [22626627] :
        [2262662] :
        [226266] :
        [22626] :
        [2262] : 2001
        [226] :
        [22] :
        [2] :
        [4] :
        [] :
        (whole node number : 15)
        */

        bin.Update();

        bin.Showresult();

        /*
        [01] : 121
        [0] :
        [2262] : 2001
        [226] :
        [22] :
        [2] :
        [] :
        (whole node number : 7)
        */

        bin.Remove(info[2]); //全て削除
        bin.Remove(info[3]);

        bin.Update();
        bin.Update();

        bin.Showresult();

        /*
        (whole node number : 0)
        */

    }

    else if (mode == Mode::compare6) {

        /*
        [0073] :
        [007] :
        [00] :
        [01] : 121
        [0] :
        [22626627] :
        [2262662] :
        [226266] :
        [22626] :
        [2262] : 2001
        [226] :
        [22] :
        2] :
        [4] :
        [] :
        (whole node number : 15)
        */
        bin.actiontree.Collision_compare6();
        bin.Showresult(); //compare6にUpdateはない

        /*
        [01] : 121
        [0] :
        [2262] : 2001
        [226] :
        [22] :
        [2] :
        [] :
        (whole node number : 7)
        */

        bin.Remove(info[2]); //全て削除
        bin.Remove(info[3]);

        bin.actiontree.Collision_compare6();

        bin.Showresult();

        /*
        (whole node number : 0)
        */


    }

    else {

        /*
        [01] : 121
        [0] :
        [2262] : 2001
        [226] :
        [22] :
        [2] :
        [] :
        (whole node number : 7)
        */

        bin.Remove(info[2]); //全て削除
        bin.Remove(info[3]);

        bin.Update();

        bin.Showresult();

        /*
        (whole node number : 0)
        */

    }


    std::cout << "\n-- simple test ends.--\n";

    return 0;

}//collision6はCollision処理時に伐採する


/***************************************************************************
    Collision test
****************************************************************************/

extern int collisioncount;

void simplecollisiontest(void) {

    BinaryTree_comparetest::testmode();


    { // test1 : 同じノードに複数要素がある場合 ----------------------

        BinaryTree_comparetest bin;

        BinaryTree<unsigned int>::NodeInfo info[10]; int ID(0);

        bin.Register( //("000.000.111.011")
            info[0], ++ID,
            std::vector<char>{0b00101101, 0b00110000, 0b00110000},
            std::vector<char>{0b00100100, 0b00110000, 0b00110000}
        );
        bin.Register(
            info[1], ++ID,
            std::vector<char>{0b00101101, 0b00110000, 0b00110000},
            std::vector<char>{0b00100100, 0b00110000, 0b00110000}
        );
        bin.Register(
            info[2], ++ID,
            std::vector<char>{0b00101101, 0b00110000, 0b00110000},
            std::vector<char>{0b00100100, 0b00110000, 0b00110000}
        );
        bin.Register(
            info[3], ++ID,
            std::vector<char>{0b00101101, 0b00110000, 0b00110000},
            std::vector<char>{0b00100100, 0b00110000, 0b00110000}
        );
        bin.Register(
            info[4], ++ID,
            std::vector<char>{0b00101101, 0b00110000, 0b00110000},
            std::vector<char>{0b00100100, 0b00110000, 0b00110000}
        );

        bin.Register( //外れに1つ("000.000.111.000")
            info[5], ++ID,
            std::vector<char>{0b00101101, 0b00100000, 0b00100000},
            std::vector<char>{0b00100100, 0b00100000, 0b00100000}
        );

        bin.Update();
        bin.cCollision();
        bin.Showresult(true);

        std::cout << "test1 : collisioncount " << collisioncount << "\n";

        collisioncount = 0;

    }


    { // test2 : 縦(親ノード)の要素の計算 ----------------------

        BinaryTree_comparetest bin;

        BinaryTree<unsigned int>::NodeInfo info[10]; int ID(0);

        bin.Register( //("010.010.110.010.110.110.010.111")
            info[0], ++ID,
            std::vector<char>{0b00101101, (char)0b11111111, 0b00000001},
            std::vector<char>{0b00101101, (char)0b11111111, 0b00000001}
        );
        bin.Register( //("010.010.110.010")
            info[1], ++ID,
            std::vector<char>{0b00100000, (char)0b11111111, 0b00000001},
            std::vector<char>{0b00101101, (char)0b11111111, 0b00000001}
        );
        bin.Register( //("010")
            info[2], ++ID,
            std::vector<char>{0b01000000, (char)0b11111111, 0b00000001},
            std::vector<char>{0b00101101, (char)0b11111111, 0b00000001}
        );

        bin.Register( //外れに1つ("000")
            info[3], ++ID,
            std::vector<char>{0b01000000, 0b01111111, 0b00000001},
            std::vector<char>{0b00101101, 0b01111111, 0b00000001}
        );

        bin.Update();
        bin.cCollision();
        bin.Showresult(true);

        std::cout << "test2 : collisioncount " << collisioncount << "\n";

        collisioncount = 0;

    }

    std::cout << "\n-- simple collision test ends.--\n";


    /*

    (whole node number : 6)
    test1 : collisioncount 10
    (whole node number : 10)
    test2 : collisioncount 3

    -- simple collision test ends.--

    */

}