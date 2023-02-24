/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t** tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t* tree, char key, int* value) {
    while (tree != NULL) 
    {
        if (tree->key == key) 
        {
            *value = tree->value;
            return true;
        }
        if (tree->key > key) 
        {
            tree = tree->left;
        } 
        else 
        {
            tree = tree->right;
        }
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t** tree, char key, int value) {
    bst_node_t* new = malloc(sizeof(bst_node_t));
    new->key = key;
    new->value = value;
    new->left = NULL;
    new->right = NULL;
    if (*tree == NULL) 
    {
        *tree = new;
        return;
    }
    bst_node_t* node = *tree;
    while (true) {
        if (node->key == key) 
        {
            node->value = value;
            free(new);
            return;
        }
        if (node->key > key) {
            if (node->left == NULL) 
            {
                node->left = new;
                return;
            }
            node = node->left;
        } 
        else 
        {
            if (node->right == NULL) 
            {
                node->right = new;
                return;
            }
            node = node->right;
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t* target, bst_node_t** tree) {
    bst_node_t* node = *tree;
    bst_node_t* parent = NULL;
    while (node->right != NULL) 
    {
        parent = node;
        node = node->right;
    }
    target->key = node->key;
    target->value = node->value;
    if (parent == NULL) 
    {
        *tree = node->left;
    }
    else 
    {
        parent->right = node->left;
    }
    free(node);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t** tree, char key) {
    bst_node_t* node = *tree;
    bst_node_t* parent = NULL;
    while (node != NULL) 
    {
        if (node->key == key) 
        {
            if (node->left == NULL && node->right == NULL) 
            {
                if (parent == NULL) 
                {
                    *tree = NULL;
                } 
                else 
                {
                    if (parent->left == node) 
                    {
                        parent->left = NULL;
                    } 
                    else 
                    {
                        parent->right = NULL;
                    }
                }
                free(node);
                return;
            }
            if (node->left == NULL) 
            {
                if (parent == NULL) 
                {
                    *tree = node->right;
                } 
                else 
                {
                    if (parent->left == node) 
                    {
                        parent->left = node->right;
                    } 
                    else 
                    {
                        parent->right = node->right;
                    }
                }
                free(node);
                return;
            }
            if (node->right == NULL) 
            {
                if (parent == NULL) 
                {
                    *tree = node->left;
                } 
                else 
                {
                    if (parent->left == node) 
                    {
                        parent->left = node->left;
                    } 
                    else 
                    {
                        parent->right = node->left;
                    }
                }
                free(node);
                return;
            }
            bst_replace_by_rightmost(node, &node->left);
            return;
        }
        parent = node;
        if (node->key > key) 
        {
            node = node->left;
        } 
        else 
        {
            node = node->right;
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t** tree) {
    stack_bst_t to_visit;
    bst_node_t* node = *tree;
    stack_bst_init(&to_visit);
    int top = 0;
    while (node != NULL || top > 0) 
    {
        if (node != NULL) 
        {
            stack_bst_push(&to_visit, node);
            top++;
            node = node->left;
        } 
        else 
        {
            node = stack_bst_pop(&to_visit);
            top--;
            bst_node_t* right = node->right;
            free(node);
            node = right;
        }
    }
    *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t* tree, stack_bst_t* to_visit) {
    bst_node_t* node = tree;
    while (node != NULL) 
    {
        bst_print_node(node);
        stack_bst_push(to_visit, node);
        node = node->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t* tree) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);
    bst_leftmost_preorder(tree, &to_visit);
    while(!(stack_bst_empty(&to_visit))) 
    {
        bst_node_t* node = stack_bst_pop(&to_visit);
        bst_leftmost_preorder(node->right, &to_visit);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t* tree, stack_bst_t* to_visit) {
    bst_node_t* node = tree;
    while (node != NULL) 
    {
        stack_bst_push(to_visit, node);
        node = node->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t* tree) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);
    bst_leftmost_inorder(tree, &to_visit);
    while (!stack_bst_empty(&to_visit)) {
        bst_node_t* node = stack_bst_pop(&to_visit);
        bst_print_node(node);
        bst_leftmost_inorder(node->right, &to_visit);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t* tree, stack_bst_t* to_visit, stack_bool_t* first_visit) {
    bst_node_t* node = tree;
    while (node != NULL) {
        stack_bst_push(to_visit, node);
        stack_bool_push(first_visit, true);
        node = node->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t* tree) {
    stack_bst_t to_visit;
    stack_bst_init(&to_visit);
    stack_bool_t first_visit;
    stack_bool_init(&first_visit);
    bst_leftmost_postorder(tree, &to_visit, &first_visit);
    while (!stack_bst_empty(&to_visit)) {
        bst_node_t* node = stack_bst_top(&to_visit);
        bool first = stack_bool_top(&first_visit);
        if (first) 
        {
            stack_bool_pop(&first_visit);
            stack_bool_push(&first_visit, false);
            bst_leftmost_postorder(node->right, &to_visit, &first_visit);
        } else 
        {
            bst_print_node(node);
            stack_bst_pop(&to_visit);
            stack_bool_pop(&first_visit);
        }
    }
}