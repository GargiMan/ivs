//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue() {
    
    //ukazuje na prazdny zoznam
    m_pHead = NULL;
}

PriorityQueue::~PriorityQueue() {

    //nacita (prvy/dalsi) prvok zo zoznamu a zmaze ho
    Element_t* delNode;
    for (Element_t* Node = GetHead(); Node != NULL; delete delNode) {
        delNode = Node;
        Node = Node->pNext;
    }
}

void PriorityQueue::Insert(int value) {

    //vytvori sa novy prvok a priradi sa mu hodnota
    Element_t* newNode = new Element_t;    
    newNode->value = value; 

    //nacita (prvy/dalsi) prvok zo zoznamu
    for (Element_t* Node = GetHead(); Node != NULL; Node = Node->pNext) {
        
        //nova hodnota ma vacsiu hodnotu ako nacitany prvok
        if (newNode->value > Node->value) {

            //nova hodnota ma vacsiu hodnotu ako dalsi prvok v poradi
            if (Node->pNext != NULL && newNode->value > Node->pNext->value) continue;

            //prvok sa zaradi do zoznamu
            newNode->pPrev = Node;                  
            newNode->pNext = Node->pNext;           
            if (Node->pNext != NULL) newNode->pNext->pPrev = newNode;
            Node->pNext = newNode;

            return;
        
        //nova hodnota je mensia ako prvy prvok v zozname
        } else {                                        

            //prvok sa zaradi do zoznamu a nastavi ako zaciatok zoznamu
            newNode->pPrev = NULL;
            newNode->pNext = Node;
            Node->pPrev = newNode;
            m_pHead = newNode;

            return;
        }
    }

    //neexistuje ziadny prvok v zozname              
    newNode->pPrev = NULL;
    newNode->pNext = NULL;
    m_pHead = newNode;
}    

bool PriorityQueue::Remove(int value) {

    //v zozname sa vyhlada prvok a priradi sa premennej
    Element_t* Node = Find(value);

    //prvok sa nasiel v zozname
    if (Node != NULL) {

        //prvok nie je prvy v zozname
        if (Node->pPrev != NULL) {
            Node->pPrev->pNext = Node->pNext;      
        
        //prvok je prvy v zozname
        } else {
            m_pHead = Node->pNext;
        }
        
        //prvok nie je posledny v zozname
        if (Node->pNext != NULL) {
            Node->pNext->pPrev = Node->pPrev;
        }

        //zmaze sa prvok
        delete Node;
        
        return true;
    }

    //prvok sa nenasiel v zozname
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value) {

    //nacita (prvy/dalsi) prvok zo zoznamu
    for (Element_t* Node = GetHead(); Node != NULL; Node = Node->pNext) {

        //ak sa hladana hodnota zhoduje s hodnotou prvku, vrati sa najdeny prvok
        if (value == Node->value) return Node;
    }

    //prvok sa nenasiel v zozname
    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead() {

    //vrati zaciatok zoznamu
    return m_pHead;
}

/*** Konec souboru tdd_code.cpp ***/