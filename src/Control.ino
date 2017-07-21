/*
 * Función para obtener el sensor activo más a la derecha
 */
int sensorDerecha (bool lectura_CNYS_para_PD[8])
{
  if (lectura_CNYS_para_PD[7] == 1) return(8); // Sensor más a la derecha
  if (lectura_CNYS_para_PD[6] == 1) return(7);
  if (lectura_CNYS_para_PD[5] == 1) return(6);
  if (lectura_CNYS_para_PD[4] == 1) return(5);
  if (lectura_CNYS_para_PD[3] == 1) return(4);
  if (lectura_CNYS_para_PD[2] == 1) return(3);
  if (lectura_CNYS_para_PD[1] == 1) return(2);
  if (lectura_CNYS_para_PD[0] == 1) return(1); // Sensor más a la izquierda
  return(0); // No se detecta la línea
}

/*
 * Función para obtener el sensor activo más a la izquierda
 */
int sensorIzquierda (bool lectura_CNYS_para_PD[8])
{
  if (lectura_CNYS_para_PD[0] == 1) return(1); // Sensor más a la izquierda
  if (lectura_CNYS_para_PD[1] == 1) return(2);
  if (lectura_CNYS_para_PD[2] == 1) return(3);
  if (lectura_CNYS_para_PD[3] == 1) return(4);
  if (lectura_CNYS_para_PD[4] == 1) return(5);
  if (lectura_CNYS_para_PD[5] == 1) return(6);
  if (lectura_CNYS_para_PD[6] == 1) return(7);
  if (lectura_CNYS_para_PD[7] == 1) return(8); // Sensor más a la derecha
  return(0); // No se detecta la línea
}

/*
 * Función para obtener el sensor activo más a la derecha
 */
int sensorDerechaReducido (bool lectura_CNYS_para_PD[8])
{
  //if (lectura_CNYS_para_PD[5] == 1) return(6);
  if (lectura_CNYS_para_PD[4] == 1) return(5);
  if (lectura_CNYS_para_PD[3] == 1) return(4);
  //if (lectura_CNYS_para_PD[2] == 1) return(3);
  //if (lectura_CNYS_para_PD[1] == 1) return(2);
  //if (lectura_CNYS_para_PD[0] == 1) return(1); // Sensor más a la izquierda
  return(0); // No se detecta la línea
}

/*
 * Función para obtener el sensor activo más a la izquierda
 */
int sensorIzquierdaReducido (bool lectura_CNYS_para_PD[8])
{
  //if (lectura_CNYS_para_PD[2] == 1) return(3);
  if (lectura_CNYS_para_PD[3] == 1) return(4);
  if (lectura_CNYS_para_PD[4] == 1) return(5);
  //if (lectura_CNYS_para_PD[5] == 1) return(6);
  //if (lectura_CNYS_para_PD[6] == 1) return(7);
  //if (lectura_CNYS_para_PD[7] == 1) return(8); // Sensor más a la derecha
  return(0); // No se detecta la línea
}

/*
 * Función para detectar si se está leyendo negro en todos los sensores, lo que
 * podría indicar una marca de frenada en la pista
 */
void marcaFrenada ()
{
  if (lectura_CNYS_actual[0] == 1
      && lectura_CNYS_actual[1] == 1
      && lectura_CNYS_actual[2] == 1
      && lectura_CNYS_actual[3] == 1
      && lectura_CNYS_actual[4] == 1
      && lectura_CNYS_actual[5] == 1
      && lectura_CNYS_actual[6] == 1
      && lectura_CNYS_actual[7] == 1)
  {
    parar_rasterador_negro++;
  }

}

/*
 * Función para medir la diferencia en distancia entre las lecturas extremas
 */
void anchoLectura (int izquierdo, int derecho)
{
  diferencia = derecho - izquierdo;
}

/*
 * Función para comprobar el número de lineas detectadas a un momento dado
 * Lo que hace es restar la lectura de un sensor a la anterior buscando valores de -1
 */
int comprobarNumeroLineas ()
{
  int numero_lineas = 0;

  /*if ((lectura_CNYS_actual[0] == 1 && lectura_CNYS_actual[3] == 0 && lectura_CNYS_actual[4] == 0 && lectura_CNYS_actual[7] == 1)
      || (lectura_CNYS_actual[0] == 1 && lectura_CNYS_actual[3] == 1 && lectura_CNYS_actual[4] == 1 && lectura_CNYS_actual[7] == 1)) // Caso especial para una T o cruz
  {
    if (lado_marca == DERECHA)
    {
      calculoDireccionDerecha (); // Sólo se lee por la derecha

      control_pwm = controlPD (); // Cálculo del PID

      actuacionMotores (control_pwm); // Actuación sobre los motores
    }

    if (lado_marca == IZQUIERDA)
    {
      calculoDireccionIzquierda (); // Sólo se lee por la izquierda

      control_pwm = controlPD (); // Cálculo del PID

      actuacionMotores (control_pwm); // Actuación sobre los motores
    }

    bifurcacion_pasada = 1;
    return 2;
  }*/

  if (lectura_CNYS_actual[0] == 0) // Comparamos de izquierda a derecha
  {
    for (int i = 0; i < 7; i++)
    {
      if ((lectura_CNYS_actual[i] - lectura_CNYS_actual[i + 1]) == -1)
      {
        numero_lineas++;
      }
    }
  }

  if (lectura_CNYS_actual[0] == 1) // Comparamos de derecha a izquierda
  {
    for (int i = 7; i > 0; i--)
    {
      if ((lectura_CNYS_actual[i] - lectura_CNYS_actual[i - 1]) == -1)
      {
        numero_lineas++;
      }
    }
  }

  return (numero_lineas); // Cero implica ninguna línea
                          // Uno implica una línea,
                          // Dos implica una línea y una marca
                          // Tres implica una línea y dos marcas
}

/*
 * Función para comprobar de qué lado está la marca
 */
int comprobarLadoMarca ()
{
  if ((/*lectura_CNYS_actual[5] == 1 ||*/ lectura_CNYS_actual[6] == 1 || lectura_CNYS_actual[7] == 1) && (lectura_CNYS_actual[0] == 1 || lectura_CNYS_actual[1] == 1 /*|| lectura_CNYS_actual[2] == 1*/))
  {
    return(RECTO);
  }
  if (/*lectura_CNYS_actual[5] == 1 || */lectura_CNYS_actual[6] == 1 || lectura_CNYS_actual[7] == 1)
  {
    return(DERECHA); // Marca a la derecha
  }
  if (lectura_CNYS_actual[0] == 1 || lectura_CNYS_actual[1] == 1 /*|| lectura_CNYS_actual[2] == 1*/)
  {
    return(IZQUIERDA); // Marca a la izquierda
  }
  return EXIT_SUCCESS;
}

/*
 * Comprobamos si estamos sobre una bifurcación
 */
int deteccionBifurcacion ()
{
  numero_lineas = comprobarNumeroLineas ();

  if (numero_lineas == 1) return (0); // No se detecta bifurcación
  if (numero_lineas > 1)  return (1); // Se detecta bifurcación
  return EXIT_SUCCESS;
}

/*
 * Funciones para el cálculo de la dirección
 */
void calculoDireccionNormal () // Para dirección normal
{
  direccion_actual = sensorDerecha(lectura_CNYS_actual) + sensorIzquierda(lectura_CNYS_actual);
}

void calculoDireccionDerecha ()// Para bifurcación derecha
{
  direccion_actual = 2 * sensorDerecha(lectura_CNYS_actual);
}

void calculoDireccionIzquierda ()// Para bifurcación izquierda
{
  direccion_actual = 2 * sensorIzquierda(lectura_CNYS_actual);
}

void calculoDireccionRecto ()
{
  direccion_actual = sensorDerechaReducido(lectura_CNYS_actual) + sensorIzquierdaReducido(lectura_CNYS_actual);
}

/*
 * Función para comprobar la desviación en función del lado de la marca obtenido
 */
int desviacion ()
{
  if (lectura_CNYS_actual[0] == 1 || lectura_CNYS_actual[1] == 1 || lectura_CNYS_actual[2] == 1)
  {
    return(1); // Desviación a la izquierda
  }
  if (lectura_CNYS_actual[5] == 1 || lectura_CNYS_actual[6] == 1 || lectura_CNYS_actual[7] == 1)
  {
    return(2); // Desviación a la derecha
  }
  return EXIT_SUCCESS;
}

/*
 * Función para el control PD
 */
int controlPD ()
{
  int control; // La variable control es la salida del PID

  direccion = direccion_actual;

  // Si perdemos la línea seguimos en la última dirección detectada
  if (direccion == 0) {direccion = direccion_anterior; parar_rasterador_blanco++;}
  else                {direccion_anterior = direccion; parar_rasterador_blanco = 0;}

  error = direccion - REFERENCIA_DIRECCION; // Error respecto a la dirección que se debe tomar

  control = error * KP + (error - error_anterior) * KD; // Control PD

  error_anterior = error; // Parte derivativa

  return(control);
}

/*
 * Función para actuar sobre los motores con la salida del PID
 */
void actuacionMotores (int control_pwm)
{
  int md_pwm; // Señal PWM del motor derecho
  int mi_pwm; // Señal PWM del motor izquierda

  md_pwm = VELOCIDAD_BASE - control_pwm; // Valor del motor derecho
  mi_pwm = VELOCIDAD_BASE + control_pwm; // Valor del motor izquierdo

  controlMotores (mi_pwm, md_pwm); // Actuación de los motores
}

/*
 * Función para limpiar los valores de ciertas variables de control
 */
void limpiezaVariables ()
{
  marca_real = 0; // Se borra la marca real
  bifurcacion = 0; // Se borra la bifurcación
  lado_marca = 0; // Se borra el lado de marca
  bifurcacion_pasada = 0; // Se borra que se ha pasado la bifurcación
  parar_rasterador_blanco = 0; // Se borran las marcas de parada del rastreador
  parar_rasterador_negro = 0;
}
