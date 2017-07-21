/*
 * Función para convertir una entrada analógica en digital
 */
byte analogicoDigital (int entrada_analogica)
{
  byte salida_digital;

  if (entrada_analogica < REFERENCIA_COLOR)
  {
    salida_digital = 1;
  }
  else if (entrada_analogica > REFERENCIA_COLOR)
  {
    salida_digital = 0;
  }

  return (salida_digital);
}

/*
 * Función de calibración de los sensores de suelo
 */
void calibracionCNYS ()
{
  int media_alta = 0;
  int media_baja = 0;
  int lectura_analogica_CNYS[8];

  lectura_analogica_CNYS[0] = analogRead (CNY_1);
  lectura_analogica_CNYS[1] = analogRead (CNY_2);
  lectura_analogica_CNYS[2] = analogRead (CNY_3);
  lectura_analogica_CNYS[3] = analogRead (CNY_4);
  lectura_analogica_CNYS[4] = analogRead (CNY_5);
  lectura_analogica_CNYS[5] = analogRead (CNY_6);
  lectura_analogica_CNYS[6] = analogRead (CNY_7);
  lectura_analogica_CNYS[7] = analogRead (CNY_8);

  for (byte i = 0; i < 8; i++)
  {
    media_alta = media_alta + lectura_analogica_CNYS[i];
  }

  media_alta = media_alta / 8;
  digitalWrite (LED_2, HIGH);

  while (digitalRead (BOTON_2) != HIGH)
  {

  }
  digitalWrite (LED_2, LOW);
  delay (500);

  lectura_analogica_CNYS[0] = analogRead (CNY_1);
  lectura_analogica_CNYS[1] = analogRead (CNY_2);
  lectura_analogica_CNYS[2] = analogRead (CNY_3);
  lectura_analogica_CNYS[3] = analogRead (CNY_4);
  lectura_analogica_CNYS[4] = analogRead (CNY_5);
  lectura_analogica_CNYS[5] = analogRead (CNY_6);
  lectura_analogica_CNYS[6] = analogRead (CNY_7);
  lectura_analogica_CNYS[7] = analogRead (CNY_8);

  for (byte i = 0; i < 8; i++)
  {
    media_baja = media_baja + lectura_analogica_CNYS[i];
  }

  media_baja = media_baja / 8;

  REFERENCIA_COLOR = (media_alta + media_baja) / 2;

  digitalWrite (LED_2, HIGH);
  delay (500);
}

/*
 * Función de lectura de los sensores de suelo en un instante dado
 */
void lecturaCnys ()
{
  lectura_CNYS_actual[0] = analogicoDigital (analogRead (CNY_1));
  lectura_CNYS_actual[1] = analogicoDigital (analogRead (CNY_2));
  lectura_CNYS_actual[2] = analogicoDigital (analogRead (CNY_3));
  lectura_CNYS_actual[3] = analogicoDigital (analogRead (CNY_4));
  lectura_CNYS_actual[4] = analogicoDigital (analogRead (CNY_5));
  lectura_CNYS_actual[5] = analogicoDigital (analogRead (CNY_6));
  lectura_CNYS_actual[6] = analogicoDigital (analogRead (CNY_7));
  lectura_CNYS_actual[7] = analogicoDigital (analogRead (CNY_8));
}

/*
 * Función para guardar la lectura de los sensores del ciclo anterior
 */
void lecturaCNYSanterior ()
{
  for (int i = 0; i < 8; i++)
  {
    lectura_CNYS_anterior[i] = lectura_CNYS_actual[i];
  }
}

/*
 * Función para comparar la lectura de sensores actual con la anterior
 */
void compararLecturas ()
{
  for (int i = 0; i < 8; i++)
  {
    if (lectura_CNYS_actual[i] != lectura_CNYS_anterior[i])
    {
      lectura_CNYS[i] = lectura_CNYS_anterior[i];
    }
    if (lectura_CNYS_actual[i] == lectura_CNYS_anterior[i])
    {
      lectura_CNYS[i] = lectura_CNYS_actual[i];
    }
  }
}

/*
 * Función para contar el número de sensores activos
 */
void sensoresActivos()
{
  sensores_activos_actual = 0;
  for (int i = 0; i < 7; i++)
  {
    if (lectura_CNYS_actual[i] == 1) sensores_activos_actual++;
  }
}
