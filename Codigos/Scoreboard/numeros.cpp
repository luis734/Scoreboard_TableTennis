#include <MD_MAX72xx.h>
#include "numeros.h"

//Declaraciones
//*********************************************************************************

#define TAM_NUM_COL 5
#define DESPLAZAMIENTO1 1
#define DESPLAZAMIENTO2 3
#define POS_INI 6

//Fila vacía
static uint8_t empty_col[2] = {B00000000,B00000000};

//Mapeado de Números
static uint8_t number_map[10][5][2] = 
{
  { // CERO
    {B00000111,B11100000},
    {B00001111,B11111000},
    {B00010000,B00001000},
    {B00011111,B11110000},
    {B00000111,B11100000}
  },
  { // UNO
    {B00000000,B00001000},
    {B00011111,B11111000},
    {B00011111,B11111000},
    {B00001000,B00001000},
    {B00000000,B00000000}
  },
  { // DOS
    {B00001111,B00001000},
    {B00011111,B10001000},
    {B00010000,B11101000},
    {B00010000,B00111000},
    {B00001100,B00011000}
  },
  { // TRES
    {B00011000,B11110000},
    {B00010101,B11111000},
    {B00010011,B00001000},
    {B00010000,B00001000},
    {B00011000,B00010000}
  },
  { // CUATRO
    {B00000000,B01000000},
    {B00011111,B11111000},
    {B00011111,B11111000},
    {B00001100,B01000000},
    {B00000011,B11000000}
  },
  { // CINCO
    {B00010001,B11110000},
    {B00010011,B11111000},
    {B00010011,B00011000},
    {B00010010,B00001000},
    {B00011110,B00110000}
  },
  { // SEIS
    {B00001000,B11110000},
    {B00010001,B00001000},
    {B00010001,B00001000},
    {B00011111,B11111000},
    {B00001111,B11110000}
  },
  { // SIETE
    {B00011110,B00000000},
    {B00011111,B10000000},
    {B00010001,B11111000},
    {B00010000,B01111000},
    {B00011000,B00000000}
  },
  { // OCHO
    {B00001110,B11110000},
    {B00011111,B11111000},
    {B00010001,B00001000},
    {B00010001,B00001000},
    {B00001110,B11110000}
  },
  { // NUEVE
    {B00001111,B11110000},
    {B00011111,B11111000},
    {B00010000,B10001000},
    {B00010000,B10001000},
    {B00001111,B00010000}
  }
};

//Mapeado de Números
static uint8_t service_map[4][5] = 
{
  //Num_mat, num_fila, bits, num_col, bits 
  
  //Arriba izquierda
  {4, 6, B10000000, 7, B11000000},
  //Arriba derecha
  {7, 6, B00000001, 7, B00000011},
  //Abajo izquierda
  {0, 0, B11000000, 1, B10000000},
  //Abajo derecha
  {3, 0, B00000011, 1, B00000001}
};

//Funciones
//*********************************************************************************

//Función para dibujar un numero en matrices
void Numero(int number, int colMat[2][2], MD_MAX72XX &mxObj)
{
  //Si el numero a escribir es menor a 10
  //-------------------------------------------------------------------------------
  if(number>=0 && number<10)
  {
    //Inicializa contador
    int cont = 0;

    //For para recorrer las columnas de las primeras matrices
    for(int i=0; i<8; i++)
    {
      if(i < POS_INI || i >= POS_INI+TAM_NUM_COL)
      {
        mxObj.setRow(colMat[0][0], i, empty_col[0]);
        mxObj.setRow(colMat[0][1], i, empty_col[1]);
      }
      else
      {
        mxObj.setRow(colMat[0][0], i, number_map[number][cont][0]);
        mxObj.setRow(colMat[0][1], i, number_map[number][cont][1]);
        cont = cont+1;
      }
    }
    
    //For para recorrer las columnas de las segundas matrices
    for(int i=0; i<8; i++)
    {
      if(cont >= TAM_NUM_COL)
      {
        mxObj.setRow(colMat[1][0], i, empty_col[0]);
        mxObj.setRow(colMat[1][1], i, empty_col[1]);
      }
      else
      {
        mxObj.setRow(colMat[1][0], i, number_map[number][cont][0]);
        mxObj.setRow(colMat[1][1], i, number_map[number][cont][1]);
        cont = cont+1;
      }
    }
  }
  //Si el numero a escribir es mayor a 9
  //-------------------------------------------------------------------------------
  else if(number >= 10)
  {
    int decenas = number/10;
    int unidades = number%10;

    //Inicializa contador
    int cont = 0;

    //For para recorrer las columnas de las primeras matrices
    for(int i=0; i<8; i++)
    {
      if(i < DESPLAZAMIENTO2 || i >= DESPLAZAMIENTO2+TAM_NUM_COL)
      {
        mxObj.setRow(colMat[0][0], i, empty_col[0]);
        mxObj.setRow(colMat[0][1], i, empty_col[1]);
      }
      else
      {
        mxObj.setRow(colMat[0][0], i, number_map[unidades][cont][0]);
        mxObj.setRow(colMat[0][1], i, number_map[unidades][cont][1]);
        cont = cont+1;
      }
    }
    //Resetea contador
    cont = 0;
    //For para recorrer las columnas de las segundas matrices
    for(int i=0; i<8; i++)
    {
      if(i < DESPLAZAMIENTO1 || i >= DESPLAZAMIENTO1+TAM_NUM_COL)
      {
        mxObj.setRow(colMat[1][0], i, empty_col[0]);
        mxObj.setRow(colMat[1][1], i, empty_col[1]);
      }
      else
      {
        mxObj.setRow(colMat[1][0], i, number_map[decenas][cont][0]);
        mxObj.setRow(colMat[1][1], i, number_map[decenas][cont][1]);
        cont = cont+1;
      }
    }
  }
}

//Función para dibujar servicio
//*********************************************************************************
void Servicio(uint8_t player, MD_MAX72XX &mxObj)
{
  //Switch para prender leds por jugador
  switch (player)
  {
    //Jugador 1
    case 1:
      mxObj.setColumn(service_map[0][0], service_map[0][1], service_map[0][2]);
      mxObj.setColumn(service_map[0][0], service_map[0][3], service_map[0][4]);
    break;
    //Jugador 2
    case 2:
      mxObj.setColumn(service_map[1][0], service_map[1][1], service_map[1][2]);
      mxObj.setColumn(service_map[1][0], service_map[1][3], service_map[1][4]);
    break;
    //Jugador 3
    case 3:
      mxObj.setColumn(service_map[2][0], service_map[2][1], service_map[2][2]);
      mxObj.setColumn(service_map[2][0], service_map[2][3], service_map[2][4]);
    break;
    //Jugador 4
    case 4:
      mxObj.setColumn(service_map[3][0], service_map[3][1], service_map[3][2]);
      mxObj.setColumn(service_map[3][0], service_map[3][3], service_map[3][4]);
    break;
  }
}

//Función para limpiar servicio
//*********************************************************************************
void LimpiaServ(uint8_t player, MD_MAX72XX &mxObj)
{
  //Switch para prender leds por jugador
  switch (player)
  {
    //Jugador 1
    case 1:
      mxObj.setColumn(service_map[0][0], service_map[0][1], empty_col[0]);
      mxObj.setColumn(service_map[0][0], service_map[0][3], empty_col[1]);
    break;
    //Jugador 2
    case 2:
      mxObj.setColumn(service_map[1][0], service_map[1][1], empty_col[0]);
      mxObj.setColumn(service_map[1][0], service_map[1][3], empty_col[1]);
    break;
    //Jugador 3
    case 3:
      mxObj.setColumn(service_map[2][0], service_map[2][1], empty_col[0]);
      mxObj.setColumn(service_map[2][0], service_map[2][3], empty_col[1]);
    break;
    //Jugador 4
    case 4:
      mxObj.setColumn(service_map[3][0], service_map[3][1], empty_col[0]);
      mxObj.setColumn(service_map[3][0], service_map[3][3], empty_col[1]);
    break;
  }
}