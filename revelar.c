// compilar: gcc revelar.c -o revelar -lm
// executar: ./revelar output.ppm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void binaryToString(const int *binaryArray, char *resultString, int arrayLength)
{
  int len = arrayLength / 8;

  for (int i = 0; i < len; i++)
  {
    int decimalValue = 0;
    for (int j = 0; j < 8; j++)
    {
      decimalValue = decimalValue * 2 + binaryArray[i * 8 + j];
    }
    resultString[i] = (char)decimalValue;
  }
  resultString[len] = '\0';
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

  int array[1120];
  unsigned char charBreak = 0;
  int len = 0;
  for (j = 0; j < h; j++)
  {
    for (i = 0; i < l; i++)
    {
      int lsb = imagem[i][j][0] & 0b00000001;
      array[len] = lsb;

      charBreak = charBreak << 1;
      if (lsb)
      {
        charBreak = charBreak | 0b00000001;
      }
      if ((charBreak == 0b01011100) && ((len + 1) % 8 == 0))
      {
        goto exitLoops;
      }
      len++;
    }
  }
exitLoops:

  printf("charBreak: %d\n", charBreak);
  printf("u: %d\n", len);

  char resultString[len - 7];
  binaryToString(array, resultString, len - 7);
  printf("Mensagem original: %s\n", resultString);

  for (i = 0; i < l; i++)
    free(imagem[i]);
  free(imagem);
}