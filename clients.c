#include "clients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ClientsList * clients_init ()
{
  ClientsList * clients = (ClientsList *) malloc( sizeof(ClientsList) );

  for (int i = 0; i < 26; i++)
    clients->next[i] = 0;

  return clients;
}

Client * clients_search (ClientsList * clients, char c)
{
  if ( c >= 'a' && c <= 'z' ) c -= ('a' - 'A');

  static Client list[3000];
  int next = 0, index = c - 'A';
  int i;

  for (i = 0; i < clients->next[index]; i++){
    strcpy( list[i].code, clients->list[index][i].code );
  }

  list[i].code[0] = '\0';

  return list;
}
