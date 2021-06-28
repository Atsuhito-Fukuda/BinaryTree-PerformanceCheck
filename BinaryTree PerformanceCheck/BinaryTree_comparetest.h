#pragma once
#include "BinaryTree.h"

enum class Mode {
    compare0, compare1, compare2, compare3, compare4, compare5, compare6, compare7
};
extern const Mode mode;
extern void Collision(BinaryTree<unsigned int>& actiontree);


class BinaryTree_comparetest {

public:

    BinaryTree<unsigned int> actiontree; //”ª•ª–Ø

    BinaryTree_comparetest() : actiontree(3) {}

    static void testmode() {

        std::cout << "[mode : compare";

        if (mode == Mode::compare0) std::cout << "0";
        else if (mode == Mode::compare1) std::cout << "1";
        else if (mode == Mode::compare2) std::cout << "2";
        else if (mode == Mode::compare3) std::cout << "3";
        else if (mode == Mode::compare4) std::cout << "4";
        else if (mode == Mode::compare5) std::cout << "5";
        else if (mode == Mode::compare6) std::cout << "6";
        else if (mode == Mode::compare7) std::cout << "7";

        std::cout << "]\n";

    }

    void Register(
        BinaryTree<unsigned int>::NodeInfo& nodeinfo,
        const unsigned int& id,
        const std::vector<char>& mincode,
        const std::vector<char>& maxcode
    ) {

        if (mode == Mode::compare0)actiontree.Register(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare1) actiontree.Register_compare1(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare2) actiontree.Register_compare1(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare3) actiontree.Register_compare3(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare4) actiontree.Register_compare4(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare5) actiontree.Register_compare3(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare6) actiontree.Register_compare4(nodeinfo, id, mincode, maxcode);
        if (mode == Mode::compare7) actiontree.Register_compare4(nodeinfo, id, mincode, maxcode);

    }
    void Remove(BinaryTree<unsigned int>::NodeInfo& info) { actiontree.Remove(info); }

    void Showresult(bool wholenumonly = false) { actiontree.Showresult(wholenumonly); }

    void Update() {

        if (mode == Mode::compare0) actiontree.Update();
        if (mode == Mode::compare1) actiontree.Update_compare1();
        if (mode == Mode::compare2) actiontree.Update_compare2();
        if (mode == Mode::compare3) actiontree.Update();
        if (mode == Mode::compare4) actiontree.Update_compare2();
        if (mode == Mode::compare5) actiontree.Update_compare5();
        if (mode == Mode::compare6) {}
        if (mode == Mode::compare7) {}

    }

    void cCollision(void) {

        if (mode == Mode::compare5) actiontree.Collision_compare5();
        if (mode == Mode::compare6) actiontree.Collision_compare6();
        if (mode == Mode::compare7) actiontree.Collision_compare5();
        else  Collision(actiontree);

    }

};