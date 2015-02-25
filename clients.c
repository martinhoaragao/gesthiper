#include <stdio.h>

int main ()
{
  FILE * fp = fopen("clientsfile.txt", "r");
  char client[6];

  if ( fp == NULL )
  {
    printf("O ficheiro não existe.\n");
  } else {
    while ( fread(client, sizeof(char), 6, fp) != 0 )
    {
      printf("Letras:%c%c\n", client[0], client[1]);
      printf("Número: %d%d%d\n", (client[2] - '0'), (client[3] - '0'), (client[4] - '0'));
    }
  }

  return 0;
}
