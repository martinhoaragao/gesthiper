typedef struct {
  // Saves the product code, ex. AB2345
  char code[6];
  // Normal sales distributed by months
  float normal[12];
  // Promotion sales distributed by months
  float promotion[12];
} ProductSales;

int init_accounting();
int insert_product(char *);
