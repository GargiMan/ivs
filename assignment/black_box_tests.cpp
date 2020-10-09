//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Marek Gergel <xgerge01@stud.fit.vutbr.cz>
// $Date:       $2020-02-21
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Marek Gergel
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public ::testing::Test {
    protected:
        BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test {
    protected:
        virtual void SetUp() {
            int value[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

            for(int i = 0; i < 14; ++i)
                tree.InsertNode(value[i]);
        }

        BinaryTree tree;
};

class TreeAxioms : public ::testing::Test {
    protected:
        virtual void SetUp() {
            int value[] = { 10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 55 };

            for(int i = 0; i < 14; ++i)
                tree.InsertNode(value[i]);
        }

        BinaryTree tree;
};




TEST_F(EmptyTree, InsertNode) {
    //kontrola korena prazdneho stromu
    EXPECT_TRUE(tree.GetRoot() == NULL);

    //vlozenie prvku, kontrola korena s novym prvkom
    tree.InsertNode(100);
    ASSERT_TRUE(tree.GetRoot() != NULL);
    EXPECT_EQ(tree.GetRoot()->key, 100);

    //vlozenie prvku, kontrola korena s novym prvkom
    tree.InsertNode(0);
    ASSERT_TRUE(tree.GetRoot() != NULL);
    EXPECT_NE(tree.GetRoot()->key, 0);
}

TEST_F(EmptyTree, DeleteNode) {
    //vymazavanie prvku z prazdneho stromu
    EXPECT_FALSE(tree.DeleteNode(0));

    //vlozenie prvku a nasledne vymazanie
    tree.InsertNode(100);
    EXPECT_TRUE(tree.DeleteNode(100));
}

TEST_F(EmptyTree, FindNode) {
    //hladanie prvku v prazdnom strome
    EXPECT_TRUE(tree.FindNode(0) == NULL);

    //vlozenie prvku a nasledne vyhladanie
    tree.InsertNode(100);
    EXPECT_TRUE(tree.FindNode(100)->key == 100);
}



TEST_F(NonEmptyTree, InsertNode) {
    //kontrola korena neprazdneho stromu
    EXPECT_TRUE(tree.GetRoot() != NULL);

    Node_t *Exist;
    std::pair<bool, Node_t *> Node;

    //vlozenie existujuceho prvku
    int value[] = { 10, 85, 15, 70, 20, 60, 30 };
    for(int i = 0; i < 7; ++i) {
        Exist = tree.FindNode(value[i]);
        Node = tree.InsertNode(value[i]);
        ASSERT_TRUE(Node.second == Exist);
        EXPECT_FALSE(Node.first);
    }

    //vlozenie noveho prvku
    Exist = tree.FindNode(0);
    Node = tree.InsertNode(0);
    EXPECT_TRUE(Node.second != Exist);
    EXPECT_TRUE(Node.first);
}

TEST_F(NonEmptyTree, DeleteNode) {

    //vymazanie nahodnej hodnoty mimo pociatocnych hodnot
    EXPECT_FALSE(tree.DeleteNode(0));

    //vymazanie z pociatocnych prvkov a nasledna kontrola
    Node_t *Exist;
    int value[] = { 10, 85, 15, 70, 20, 60, 30 };
    for(int i = 0; i < 7; ++i) {
        EXPECT_TRUE(tree.DeleteNode(value[i]));
        Exist = tree.FindNode(value[i]);
        EXPECT_FALSE(Exist);
    }
}

TEST_F(NonEmptyTree, FindNode) {
    //hladanie prvku v prazdnom strome
    EXPECT_TRUE(tree.FindNode(0) == NULL);

    //vlozenie prvku a nasledne vyhladanie
    tree.InsertNode(100);
    EXPECT_TRUE(tree.FindNode(100)->key == 100);

    //hladanie z vlozenych prvkov
    Node_t *Exist;
    int value[] = { 10, 85, 15, 70, 20, 60, 30 };
    for(int i = 0; i < 7; ++i) {
        Exist = tree.FindNode(value[i]);
        EXPECT_TRUE(Exist);
    }
}

TEST_F(TreeAxioms, Axiom1) {

    //nacitanie vsetkych uzlov bez potomkov
    std::vector<Node_t *> Leafes;
    tree.GetLeafNodes(Leafes);

    //kontrola uzlov bez potomkov
    for (Node_t *Leaf : Leafes) {
        EXPECT_TRUE(Leaf->color == tree.BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2) {

    //nacitanie vsetkych uzlov
    std::vector<Node_t *> Nodes;
    tree.GetAllNodes(Nodes);

    //kontrola cervenych uzlov na ciernych potomkov
    for (Node_t *Node : Nodes) {
        if (Node->color == tree.RED) {
            EXPECT_TRUE(Node->pLeft->color == tree.BLACK);
            EXPECT_TRUE(Node->pRight->color == tree.BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3) {

    //nacitanie vsetkych uzlov bez potomkov
    std::vector<Node_t *> Leafes;
    tree.GetLeafNodes(Leafes);

    //kontrola poctu ciernych uzlov ku korenu
    int Prev;
    for (Node_t *Leaf : Leafes) {
        int Now = 0;
        for (Node_t *Loaded = Leaf->pParent; Loaded->pParent != NULL; Loaded = Loaded->pParent) {
            if (Loaded->color == tree.BLACK) Now++;

        }
        if (Prev > 0) EXPECT_EQ(Prev,Now);
        Prev = Now;
    }
}



/*** Konec souboru black_box_tests.cpp ***/
