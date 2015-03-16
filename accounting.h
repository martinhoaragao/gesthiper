// AVL tree node
typedef struct node{
  // Saves the product code, ex. AB2345
  char code[6];
  // Normal sales distributed by months
  double normal[12];
  // Promotion sales distributed by months
  double promotion[12];
  // Height of the node
  int height;
  struct node *left;
  struct node *right;
} ProductNode;

int init_accounting();
int insert_product(char *);
int searchProduct(char *);
