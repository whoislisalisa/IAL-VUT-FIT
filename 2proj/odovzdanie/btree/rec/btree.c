/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  (*tree)= NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree == NULL)
  {
    return false;
  }

  if(tree->key == key)
  {
    *value = tree->value;
    return true;
  }

  if(tree->key > key){
    return bst_search(tree->left, key, value);
  }
  return bst_search(tree->right, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if(*tree == NULL)
  {
    (*tree)= malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return;
  }
 
  if((*tree)->key == key)
  {
    (*tree)->value = value;
    return;
  }
  if((*tree)->key > key)
  {
    bst_insert(&(*tree)->left, key, value);
    return;
  }
  if((*tree)->key < key)
  {
    bst_insert(&(*tree)->right, key, value);
    return;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if((*tree)->right != NULL)
  {
    bst_replace_by_rightmost(target, &(*tree)->right);
  }
  else
  {
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    bst_node_t *node = *tree;
    *tree = (*tree)->left;
    free(node);
  }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  if((*tree) == NULL)
  {
    return;
  }

  if((*tree)->key == key)
  {
    if((*tree)->left == NULL && (*tree)->right == NULL)
    {
      free(*tree);
      *tree = NULL;
    }
    else if ((*tree)->left == NULL)
    {
      bst_node_t *node = *tree;
      *tree = (*tree)->right;
      free(node);
    }
    else if((*tree)->right == NULL)
    {
      bst_node_t *node = *tree;
      *tree = (*tree)->left;
      free(node);
    }
    else
    {
      bst_replace_by_rightmost(*tree, &(*tree)->left);
    }
  }
  else if((*tree)->key > key)
  {
    bst_delete(&(*tree)->left, key);
  }
  else
  {
    bst_delete(&(*tree)->right, key);
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if(*tree == NULL)
  {
    return;
  }
  bst_dispose(&(*tree)->left);
  bst_dispose(&(*tree)->right);
  free(*tree);
  *tree = NULL;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  if(tree != NULL)
  {
    bst_print_node(tree);
    if(tree->left != NULL)
    {
      bst_preorder(tree->left);
    }
    if(tree->right != NULL)
    {
      bst_preorder(tree->right);
    }
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  if(tree != NULL)
  {
    if(tree->left != NULL)
    {
      bst_inorder(tree->left);
    }
    bst_print_node(tree);
    if(tree->right != NULL)
    {
      bst_inorder(tree->right);
    }
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  if(tree != NULL)
  {
    if(tree->left != NULL)
    {
      bst_postorder(tree->left);
    }
    if(tree->right != NULL)
    {
      bst_postorder(tree->right);
    }
    bst_print_node(tree);
  }
}
