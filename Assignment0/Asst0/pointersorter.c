#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <ctype.h>

/*
	Author- Devvrat Patel
			  netid- dap299
			  RUID - 169009727
			*/
int myCompare(const char *s1, const char *s2)
{
    for ( ; *s1 == *s2; s1++, s2++){
        if (*s1 == '\0'){
            return 0;
        }
    }
    if(islower(*s1)&&isupper(*s2)){
		return -1;    
    	}
    	
    	if(isupper(*s1)&&islower(*s2)){
		return 1;    	
    	}
    	if ((islower(*s1)&&islower(*s2)) || (isupper(*s1) && isupper(*s2))){
    	return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : 1);
    }
 }


/*Devvrat Patel
CS 214 SPRING 2019 POINTERSORTER
 
This program takes in an argument from the command line, parses it in different sequence of alphabetic characters, and then sorts 
then in a descending order. The strings in the argument will be separated by non-alphabetic characters. For example,
 
"Cat cat zebra Apple":
Output-Cat
   	Apple
   	zebra
   	cat
 
Here the argument is separated by spaces but they also can be sorted by digits and other separators including ,./` and many others.
 
I have included more examples of the program in the testcases.txt.
 
Some problems that i encountered included how to sort the string in descending order. I was using strcmp at first and reversed the 
print order for the descending print but then that would print the uppercases differently as well. Therefore, in the end I just 
decided to create my own compareTo function which worked like strcmp but according to this assignment. There were also many problems that I faced with segmentation fault but i was able to fix it using an online gdb and debugging each line.
 
Malloc was tricky at first and it took me some time understanding that and the pointers.
 
I used Tree for my data structure to store the strings after separating them and that gave me an insertion complexity of log(n).
 
After the rotations were finished, the tree was done balancing and i just printed it out with simple in-order traversal method. This traversal 
also was used to free the noes and its word so that there is no memory leaks. This was tricky at first and took some trials and errors to work with it.
In the end the assignment was fair and was a good introductory assignment for C. I enjoyed working on this assignment. Some things I would do 
differently from before would be to start earlier than I did. I was busy with my other classes and wasn’t able to start until the last day. 


*/

enum sachai {yeswa, nowa}; 

typedef struct Node_ Node;
typedef struct Tree_ Tree;

struct Tree_{
  Node *root;
};

struct Node_{
  enum sachai sach;
  char *word;
  Node *left, *right, *parent;
};

Tree *TreeMein(Tree * tree) {
  tree->root = NULL;
  return tree;
}


/*I am including the test cases and read me here as well if the i failed to zip it properly
1 ./pointersorter "aand Aand AAnd aAnd"
AAnd
Aand
aAnd
aand

2 ./pointersorter "testing7is8the8key"
the
testing
key
is

3 ./pointersorter "a very very veyr veyr veyryuea; dosaiuasdjh hadsyu sah asudou oasud dsiaou e asudyo LONGGGGGGGG STRING"
STRING
LONGGGGGGGG
veyr
veyr
veyryuea
very
very
sah
oasud
hadsyu
e
dsiaou
dosaiuasdjh
a
asudyo
asudou

4 ./pointersorter "cat CAT cAT caT CaT zebra ZEEBTA"ZEEBTA
CAT
CaT
zebra
cAT
caT
cat

5 ./pointersorter "a2s35f68868d097d/dsa;[.f[dfhuhdsf-*f-s0f9"
s
s
f
f
f
f
d
d
dsa
dfhuhdsf
a

6 ./pointersorter ""


[dap299@ilab1 assignment1]$ ./pointersorter "a"
a

[dap299@ilab1 assignment1]$ ./pointersorter "addas/0dsa"
dsa
addas

[dap299@ilab1 assignment1]$ ./pointersorter adssdadsadsa dfssfdfs
Incorrect number of arguments!


[dap299@ilab1 assignment1]$ ./pointersorter 'saddas' 'sad'sad
Incorrect number of arguments!

*/
 
Node *NodeMein(Node *node, enum sachai sach, char *word) {
  node->sach = sach;
  node->word = word;
  node->left = NULL;
  node->right = NULL;
  node->parent = NULL;
  return node;
}


void print(Node *node, char *prefix, int tailHein) {
  if (node == NULL) return;
  
  char tmp[strlen(prefix) + 4 + 1]; 
  sprintf(tmp, "%s%s", prefix, (tailHein ? "    " : "│   "));
  printf("%s%s%s (%s)\n", prefix , (tailHein ? "└── " : "├── ") , 
    node->word, (node->sach == yeswa ? "yeswa": "nowa"));
 
  print(node->right, tmp, 0);

  print(node->left, tmp, 1);

}


void OrderMein(Node *node) {
  if (node == NULL) return;
 
  OrderMein(node->right);  

  printf("%s\n", node->word);

  OrderMein(node->left);

}

void TreeChap(Tree *t) {
    print(t->root, "", 1);
}


void MuktNode(Node *node) {
  if (node == NULL) return;
  MuktNode(node->left);
  
  MuktNode(node->right);

  free(node->word);
  free(node);
}

void MuktNodeTreeMein(Tree *t) {
  MuktNode(t->root);
}


void DaloInMein(Tree *tree, Node *node) {

  Node *current = tree->root;
  Node *above = NULL;
  char *str = node->word;
  int diff = myCompare(current->word, str); 
  while(current != NULL) {
    if(diff < 0) {    
      above = current;
      current = current->right;
    }
    else if(diff >= 0) {  
      above = current;
      current = current->left;
    }
    if(current != NULL){
      diff = myCompare(current->word, str);
    }
  }
  diff = myCompare(above->word, str);
  if(diff < 0) {
    above->right = node;
    node->parent = above;
  } 
  else if(diff >= 0){
    above->left = node;
    node->parent = above;
  }
}

void ghumavLeft(Tree *tree, Node *node) {
  Node *asliBacha = node->right;
  node->right = asliBacha->left;
  asliBacha->left = node;
  

  if(node->parent == NULL) { 
    tree->root = asliBacha;
  }
  else { 
    if(node->parent->left == node) { 
      node->parent->left = asliBacha;
    }
    else { 
      node->parent->right = asliBacha;
    }
  }
  Node *temp = node->parent;
  node->parent = asliBacha;
  asliBacha->parent = temp;
  if(node->right != NULL) {
    node->right->parent = node;
  }
}

void ghumavRight(Tree* tree, Node *node) { 
  Node *asliBacha = node->left;
  node->left = asliBacha->right;
  asliBacha->right = node;
  
  if(node->parent == NULL){ 
    tree->root = asliBacha;
  }
  else { 
    if(node->parent->left == node) { 
      node->parent->left = asliBacha;
    }
    else { 
      node->parent->right = asliBacha;
    }
  }
  Node *temp = node->parent;
  node->parent = asliBacha;
  asliBacha->parent = temp;
  if(node->left != NULL) {
    node->left->parent = node;
  }
}

void insert(Tree *tree, Node *node){
  if(tree->root == NULL){  
    tree->root = node;
    tree->root->sach = nowa;
  }
  else {       
    DaloInMein(tree, node);  
    node->sach = yeswa;    

    while((node != tree->root) && (node->parent->sach == yeswa)) {


      if(node->parent == node->parent->parent->left) {  

        Node *kakaBhai = node->parent->parent->right;
        if(kakaBhai == NULL || kakaBhai->sach == nowa) {
          if(node == node->parent->right) {   
            node = node->parent;    
            ghumavLeft(tree, node);     
    
          }           
          node->parent->sach = nowa;
          node->parent->parent->sach = yeswa;
          ghumavRight(tree, node->parent->parent);
        } 
        else {
          kakaBhai->sach = nowa;
          node->parent->parent->sach = yeswa;
          node->parent->sach = nowa;
          node = node->parent->parent;
        } 
      } 
      else {
        Node *kakaBhai = node->parent->parent->left;
        if(kakaBhai == NULL || kakaBhai->sach == nowa) {
          if(node == node->parent->left){
            node = node->parent;
            ghumavRight(tree, node);
          }
          node->parent->sach = nowa;
          node->parent->parent->sach = yeswa;
          ghumavLeft(tree, node->parent->parent);
        } 
        else { 
          kakaBhai->sach = nowa;
          node->parent->parent->sach = yeswa;
          node->parent->sach = nowa;
          node = node->parent->parent;
        } 
      } 
    } 
    }
  tree->root->sach = nowa;
}



int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Incorrect number of arguments!\n");
    return -1;
  }

  Tree t;
  TreeMein(&t);
  char *word;
  Node *node;
  char *curr = argv[1]; 
  long long count = 0; 
  char letter = *curr; 
  long long shuruYahaSe = 0; 
  long long abYaha = 0; 
  
  while (letter != '\0') { 
    if (isalpha(letter)) {
      count++;
    }
    else {
      if (count != 0) { 
        word = (char *) malloc(sizeof(char) * (count + 1)); 
        if (word == NULL) {
          printf("MEMORY NOT ALLOCATED!\n");
          return -1;
        }
        memcpy(word, argv[1] + shuruYahaSe, count);
        word[count] = '\0';
        count = 0;
        node = (Node *) malloc(sizeof(Node));
        if (node == NULL) {
          printf("MEMORY NOT ALLOCATED!\n");
          return -1;
        }
        NodeMein(node, yeswa, word);
        insert(&t, node);
      }
      else { 
      }

      shuruYahaSe = abYaha + 1;
    }
    curr++;
    abYaha++;
    letter = *curr;
  }
  
  if (count > 0) {
    word = (char *) malloc(sizeof(char) * (count + 1)); 
    if (word == NULL) {
      printf("MEMORY  NOT ALLOCATED!\n");
      return -1;
    }
    memcpy(word, argv[1] + shuruYahaSe, count);
    word[count] = '\0';
    count = 0;
    node = (Node *) malloc(sizeof(Node));
 
    if (node == NULL) {
      printf("FATAL ERROR. MEMORY COULD NOT BE ALLOCATED!\n");
      return -1;
    }
    NodeMein(node, yeswa, word);
    insert(&t, node);
  }
  
  OrderMein(t.root);
  //TreeChap(&t);
  MuktNodeTreeMein(&t);
  
  return 0;
}


