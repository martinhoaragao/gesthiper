#include <stdio.h>
#include <string.h>
#include "../sales.h"

int main ()
{
  Sales s1 = NULL, s2 = NULL;

  /* TODO : Test query 10 */

  s1 = initSales();

  /* Test Sales initialization */
  printf("1: %s\n", (s1->height == 0 ? "Pass" : "Error"));

  /* Test client insertion */
  s1 = insertClient(s1, "FH920");
  printf("2: %s\n", (strcmp(s1->client,"FH920") == 0 ? "Pass" : "Error"));
  printf("3: %s\n", (s1->height == 0 ? "Pass" : "Error"));
  s2 = NULL;
  s2 = insertClient(s2, "FH920");
  printf("4: %s\n", (s2 == NULL ? "Pass" : "Error"));
  s1 = insertClient(s1, "FH930");
  s1 = insertClient(s1, "FH940");
  printf("5: %s\n", (strcmp(s1->client, "FH930") == 0 ? "Pass" : "Error"));

  /* Test Remotion , "FH940" will be next root */
  s1 = removeClient(s1, "FH930");
  printf("6: %s\n", (strcmp(s1->client, "FH930") != 0 ? "Pass" : "Error"));

  /* Test product insertion */
  printf("7: %s\n", ((s1->products) == NULL ? "Pass" : "Error"));
  s1 = insertProduct(s1, "FH940", "JD7753");
  printf("8: %s\n", (strcmp((s1->products)->product, "JD7753") == 0 ? "Pass" : "Error"));
  printf("9: %s\n", ((s1->products)->quant[5] == 0 ? "Pass" : "Error"));

  return 0;
}
