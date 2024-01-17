// compilar: gcc revelar.c -o revelar
// executar: ./revelar output.ppm
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char **argv)
{
  int i, j, l, h;
  unsigned char type, cmax, caractere;
  FILE *fp;

  fp = fopen(argv[1], "r");
  while ((caractere = getc(fp)) == '#')
    while ((caractere = getc(fp)) != '\n')
      ;
  ungetc(caractere, fp);

  fscanf(fp, "P%hhu\n", &type);
  while ((caractere = getc(fp)) == '#')
    while ((caractere = getc(fp)) != '\n')
      ;
  ungetc(caractere, fp);

  fscanf(fp, "%u %u\n%hhu\n", &l, &h, &cmax);

  unsigned char(**imagem)[3];

  j = l * sizeof(char *);
  imagem = malloc(j);

  j = h * 3;
  for (i = 0; i < l; i++)
    imagem[i] = malloc(j);

  if (type == 3)
  {
    for (j = 0; j < h; j++)
      for (i = 0; i < l; i++)
        fscanf(fp, "%hhu %hhu %hhu", &imagem[i][j][0], &imagem[i][j][1], &imagem[i][j][2]);
    fclose(fp);
  }
  else if (type == 6)
  {
    for (j = 0; j < h; j++)
      for (i = 0; i < l; i++)
        fscanf(fp, "%c%c%c", &imagem[i][j][0], &imagem[i][j][1], &imagem[i][j][2]);
    fclose(fp);
  }
  else
  {
    printf("Formato inválido!");
    fclose(fp);
    exit(0);
  }

  char msg[1120];
  char aux = 0;
  int k = 0;
  for (j = 0; j < h; j++)
  {
    for (i = 0; i < l; i++)
    {
      char lsb = imagem[i][j][0] & 1;
      aux <<= 1;
      aux |= lsb;

      if ((k + 1) % 8 == 0)
      {
        if (aux == 0b01011100)
        {
          goto exitLoops;
        }
        msg[(k + 1) / 8] = aux;
        aux = 0;
      }
      k++;
    }
  }
exitLoops:

  printf("Mensagem revelada: ");
  for (int i = 0; i < (k + 1) / 8; i++)
  {
    printf("%c", msg[i]);
  }
  printf("\n");

  for (i = 0; i < l; i++)
    free(imagem[i]);
  free(imagem);
}