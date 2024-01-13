// compilar: gcc ocultar.c -o ocultar -lm
// executar: ./ocultar input.ppm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void charToBinary(char c, int *binary)
{
  int ascii_value = (int)c;

  for (int i = 7; i >= 0; i--)
  {
    binary[i] = ascii_value % 2;
    ascii_value /= 2;
  }
}

void stringToBinary(const char *message, int *binaryArray)
{
  int len = strlen(message);
  int binary[8];

  for (int i = 0; i < len; i++)
  {
    charToBinary(message[i], binary);
    for (int j = 0; j < 8; j++)
    {
      binaryArray[i * 8 + j] = binary[j];
    }
  }
}

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
  int binaryArrayLength = 8 * strlen(message);
  int binaryArray[binaryArrayLength];

  stringToBinary(message, binaryArray);

  printf("binaryArray: ");
  for (int i = 0; i < binaryArrayLength; i++)
  {
    printf("%d", binaryArray[i]);
  }
  printf("\nbinaryArrayLength: %d", binaryArrayLength);
  printf("\n");

  int len = 0;
  for (j = 0; j < h; j++)
  {
    for (i = 0; i < l; i++)
    {
      if (len >= binaryArrayLength)
        goto exitLoops;

      if (binaryArray[len])
      {
        imagem[i][j][0] = imagem[i][j][0] | 0b00000001;
      }
      else
      {
        imagem[i][j][0] = imagem[i][j][0] & 0b11111110;
      }
      len++;
    }
  }
exitLoops:

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