// compilar: gcc ocultar.c -o ocultar
// executar: ./ocultar input.ppm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char *message = "Mensagem super super super super super secreta\\";
  int messageLength = strlen(message);

  if (8 * messageLength > l * h)
  {
    printf("Mensagem muito grande para a imagem!");
    exit(0);
  }

  int x = 0, y = 0;
  for (int c = 0; c < messageLength; c++)
  {
    for (int b = 7; b >= 0; b--)
    {
      char bit = (message[c] >> b) & 1;
      imagem[x][y][0] = (imagem[x][y][0] & 0xFE) | bit;

      if (x == (l - 1))
      {
        x = 0;
        y += 1;
      }
      else
      {
        x += 1;
      }
    }
  }

  printf("Mensagem ocultada!\n");
  printf("Total de caracteres: %d\n", messageLength);
  printf("Total de bits: %d\n", messageLength * 8);

  fp = fopen("output.ppm", "w");
  fprintf(fp, "P6\n");
  fprintf(fp, "%u %u\n255\n", l, h);
  for (j = 0; j < h; j++)
    for (i = 0; i < l; i++)
      fprintf(fp, "%c%c%c", imagem[i][j][0], imagem[i][j][1], imagem[i][j][2]);
  fclose(fp);

  for (i = 0; i < l; i++)
    free(imagem[i]);
  free(imagem);
}