#include "clients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Declara a lista de clientes
static ClientsList * clients;

/*********************************************************/

/* Iniciar estrutura de clientes */
void clients_init ()
{
  clients = (ClientsList *) malloc( sizeof(ClientsList) );

  for (int i = 0; i < 26; i++)
    clients->next[i] = 0;
}

/*********************************************************/

/* Procurar cliente por letra inicial */
Client * clients_search (char c)
{
  if ( isalpha(c) == 0 ) return NULL;
  if ( c >= 'a' && c <= 'z' ) c -= ('a' - 'A');

  static Client list[3000];
  int next = 0, index = c - 'A';
  int i;

  // Nenhum clientes com as iniciais passadas
  if (clients->next[index] == 0) return NULL;

  for (i = 0; i < clients->next[index]; i++){
    strcpy( list[i].code, clients->list[index][i].code );
  }

  list[i].code[0] = '\0';

  return list;
}

/*********************************************************/

/*  Inserir cliente na lista
    Devolve 0 em caso de erro, 1 caso consiga inserir
*/
int clients_insert (char * client)
{
  // Verificar se o código de cliente tem apenas 5 caracteres
  if ( sizeof(client) > 8 ) return 0;

  int index = client[0] - 'A';
  char * dest = (clients->list[index][clients->next[index]]).code;
  // Retirar '\n' da string
  strtok(client, "\n");
  strcpy(dest, client);
  (clients->next[index])++;
  return 1;
}
