/***********************************************************/
/* Rastreador Fenrir                                       */
/*                                                         */
/* Proyecto de robot rastreador basado en el Azog de AMUVa */
/*                                                         */
/* Creador: Raúl Hurtado Gavilán                           */
/* 2016-06-20 / 2017-01-29                                 */
/***********************************************************/

/*
 * DECLACIÓN DE PINES Y COMPONENTES
 */
// LEDS y BOTONES
#define LED_1 12 // Led azul
#define LED_2 11 // Led verde
#define LED_3 10 // Led rojo

#define BOTON_1 2 // Botón 1
#define BOTON_2 9 // Botón 2

// CNYS
#define CNY_1 A7 // Izquierdo
#define CNY_2 A6
#define CNY_3 A5
#define CNY_4 A4 // Central
#define CNY_5 A3 // Central
#define CNY_6 A2
#define CNY_7 A1
#define CNY_8 A0 // Derecho

// MOTORES
#define PWM_MOTOR_IZQUIERDO 5 // Motor izquierdo 1 cable azul hacia atrás
#define ENTRADA_A_MOTOR_IZQUIERDO 3
#define ENTRADA_B_MOTOR_IZQUIERDO 4

#define PWM_MOTOR_DERECHO 6 // Motor derecho 2 cable azul hacia atrás
#define ENTRADA_A_MOTOR_DERECHO 8
#define ENTRADA_B_MOTOR_DERECHO 7

/*
 * LECTURA SENSORES
 */
bool lectura_CNYS[8]; // Lectura de los sensores para el controlador PD
bool lectura_CNYS_actual[8]; // Lectura actual de los sensores
bool lectura_CNYS_anterior[8]; // Lectura del ciclo anterior de los sensores
int sensores_activos_actual = 0; // Cantidad de sensores activos actuales
int sensores_activos_anterior = 0; /// Cantitad de sensores activos en el ciclo anterior

/*
 *CONTROL PID
 */
float error = 0; // La variable error es la parte proporcional
float error_acumulado = 0; // La variable error_acumulado es la parte integral
float error_anterior = 0; // La variable error_anterior es la parte derivativa

int direccion; // Dirección que se va a tomar en el PD
int direccion_actual; // Dirección actual de la lectura de los sensores activos
int direccion_anterior; // Dirección anterior de la lectura de los sensores activos
int REFERENCIA_DIRECCION = 9; // Dirección dde referencia

// Para 150:  y
// Para 100: 20 y 50
// Para 50:  y
//int KP = 35; // Constante proporcional #40
//int KD = 20; // Constante derivativa #2500

int KP = 20; // Constante proporcional #40
int KD = 50; // Constante derivativa #2500

int VELOCIDAD_BASE = 75; // Velocidad base

int control_pwm; // Señal del PID para los motores
int md_pwm; // Señal para motor derecho
int mi_pwm; // Señal par motor izquierdo

int parar_rasterador_blanco = 0; // Variable para parar el rastreador si lleva mucho tiempo sin leer nada
int parar_rasterador_negro = 0; // Variable para parar el rastreador si lleva mucho tiempo en una marca de parada
/*
 * COMPROBACIÓN DE LÍNEAS Y MARCAS
 */
int numero_lineas; // Número de líneas detectadas en un momento dado
int media; // Media actual de los sensores activos
int media_anterior; // Media anterior de los sensores activos

int lado_marca; // Lado donde se situa la marca respecto de la línea
int marca_real; // Valor mínimo para aceptar una marca como real y no falso positivo
int MARCA_MINIMA = 30; // Veces mínimas a detectar la marca para que sea real
int MARCA_PARADA_BLANCO = 800; //
int MARCA_PARADA_NEGRO = 350;//150;

int DERECHA = 1; // Marca de giro a la derecha
int IZQUIERDA = 2; // Marca de giro a la izquierda
int RECTO = 3; // Marca de giro recta

int bifurcacion = 0; // Para detectar una bifurcación
int bifurcacion_pasada = 0; // Para indicar que se ha pasado la bifurcación

int diferencia = 0; // Variable que marca la diferencia en distancia entre lecturas extremas

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

unsigned long tiempo_inicial = 0;
unsigned long tiempo_final = 0;
unsigned long tiempo_PD = 3;

/*
 * OTRAS VARIABLES
 */
int RETARDO_REGLAMENTARIO = 500;
int REFERENCIA_COLOR = 950;
bool pausa = 0; // Variable para definir el estado de pausa del rastreador

/*
 * ESTADOS DE LA MÁQUINA DE ESTADOS FINITOS (FSM)
 */
#define REPOSO 1
#define CALIBRACION 2
#define SIGUELINEAS 3
#define ELECCION_CAMINO 4

byte estado_finito = REPOSO;

/*
 * FUNCIÓN SETUP
 */
void setup()
{
  // PINES DE ENTRADA
  pinMode (BOTON_1, INPUT);
  pinMode (BOTON_2, INPUT);

  pinMode (CNY_1, INPUT);
  pinMode (CNY_2, INPUT);
  pinMode (CNY_3, INPUT);
  pinMode (CNY_4, INPUT);
  pinMode (CNY_5, INPUT);
  pinMode (CNY_6, INPUT);
  pinMode (CNY_7, INPUT);
  pinMode (CNY_8, INPUT);

  // PINES DE SALIDA
  pinMode (LED_1, OUTPUT); // LED azul
  pinMode (LED_2, OUTPUT); // LED verde
  pinMode (LED_3, OUTPUT); // LED rojo

  pinMode (PWM_MOTOR_IZQUIERDO,OUTPUT); // Motor izquierdo 1
  pinMode (ENTRADA_A_MOTOR_IZQUIERDO,OUTPUT);
  pinMode (ENTRADA_B_MOTOR_IZQUIERDO,OUTPUT);

  pinMode (PWM_MOTOR_DERECHO,OUTPUT); // Motor derecho 2
  pinMode (ENTRADA_A_MOTOR_DERECHO,OUTPUT);
  pinMode (ENTRADA_B_MOTOR_DERECHO,OUTPUT);

  //configurarBluethoot(); // Función para configurar el Bluethoot

  Serial.begin (9600); // Inicia comunicaciones serie a 9600 bps
}

/*
 * FUNCIÓN LOOP //
 */
void loop()
{
  // Máquina de estados finitos
  switch (estado_finito)
  {
    /*
     * 1: Estado de reposo del rastreador
     */
    case REPOSO:

      controlMotores (0, 0); // Motores parados
      pausa = 0; // Para que al pasar al estado SIGUELÍNEAS comience la cuenta atrás

      digitalWrite (LED_1, LOW);
      digitalWrite (LED_2, LOW); // Señal de estado REPOSO
      digitalWrite (LED_3, HIGH);

      // Elección de estado SIGUELINEAS
      if (digitalRead (BOTON_1) == HIGH)
      {
        delay (200);
        estado_finito = SIGUELINEAS;
      }
      // Elección de estado CALIBRACION
      else if (digitalRead (BOTON_2) == HIGH)
      {
        delay (200);
        estado_finito = CALIBRACION;
      }
      break;

    /*
     * 2: Estado de calibración de los sensores
     */
    case CALIBRACION:

      digitalWrite (LED_1, HIGH); // Señal de estado CALIBRACION
      digitalWrite (LED_2, LOW);
      digitalWrite (LED_3, LOW);

      // Activado de la calibración
      if (digitalRead (BOTON_2) == HIGH)
      {
        delay (200);
        calibracionCNYS ();
        //Serial.println(REFERENCIA_COLOR);
      }
      // Elección de estado SIGUELINEAS
      else if (digitalRead (BOTON_1) == HIGH)
      {
        delay (200);
        estado_finito = SIGUELINEAS;
      }

      break;

    /*
     * 3: Estado normal de siguimiento de línea
     */
    case SIGUELINEAS:

      // Si está a 0, realiza la cuenta atrás, si está a 1 la obvia
      if (pausa == 0)
      {
        pausa = 1;
        digitalWrite (LED_1, LOW);
        digitalWrite (LED_2, HIGH);
        digitalWrite (LED_3, LOW); // Señal de estado SIGUELINEAS

        for (byte i = 0; i < 5; i++)
        {
          digitalWrite (LED_1, HIGH);
          delay (RETARDO_REGLAMENTARIO/10);
          digitalWrite (LED_1, LOW);
          delay (RETARDO_REGLAMENTARIO/10);
        }
      }

      VELOCIDAD_BASE = 100;

      lecturaCnys (); // Lectura de los sensores

      sensoresActivos ();

      /*tiempo1 = millis();
      if (tiempo1 - tiempo2 > 1)
      {
        //Serial.println(sensores_activos_actual);
        Serial.print(lectura_CNYS_actual[0]);
        Serial.print(lectura_CNYS_actual[1]);
        Serial.print(lectura_CNYS_actual[2]);
        Serial.print(lectura_CNYS_actual[3]);
        Serial.print(lectura_CNYS_actual[4]);
        Serial.print(lectura_CNYS_actual[5]);
        Serial.print(lectura_CNYS_actual[6]);
        Serial.print(lectura_CNYS_actual[7]);
        Serial.println();
        tiempo2 = millis();
      }*/

      marcaFrenada ();
      ////////////////
      /*tiempo_inicial = millis();
      if (tiempo_inicial - tiempo_final > tiempo_PD)
      {*/

      if (parar_rasterador_blanco > MARCA_PARADA_BLANCO || parar_rasterador_negro > MARCA_PARADA_NEGRO)
      {
        estado_finito = REPOSO;
        limpiezaVariables ();
      }

      numero_lineas = comprobarNumeroLineas (); // Comprobamos el número de líneas detectadas

      // Cambiamos de estado finito si terminamos de leer la marca
      if ((marca_real > MARCA_MINIMA) && (numero_lineas == 1))
      {
        estado_finito = ELECCION_CAMINO;
        //Serial.println(marca_real);
        //Serial.println(lado_marca);
        break;
      }

      // Se va comprobando si hay marcas y/o falsos positivos
      if (numero_lineas <= 1) // Parece que no hay marcas
      {
        marca_real = 0;
        digitalWrite (LED_1, LOW);
        digitalWrite (LED_3, LOW);

        calculoDireccionNormal (); // Cálculo de la dirección actual
        control_pwm = controlPD (); // Cálculo del PID
        actuacionMotores (control_pwm); // Actuación sobre los motores
      }

      if (numero_lineas > 1) // Parece que hay una marca
      {
        if (marca_real > 10 && marca_real < 15) // Se espera para asegurar una lectura correcta del lado de la marca
        {
          lado_marca = comprobarLadoMarca (); // Comprobación del lado de la marca

          // Se marca con leds el lado de la marca
          if (lado_marca == 1) digitalWrite(LED_1, HIGH);
          if (lado_marca == 2) digitalWrite(LED_3, HIGH);
          if (lado_marca == 3) {digitalWrite(LED_1, HIGH); digitalWrite(LED_3, HIGH);}
        }

        marca_real++; // Contador para asegurar que es una marca real

        VELOCIDAD_BASE = 70;

        calculoDireccionRecto ();
        control_pwm = controlPD (); // Cálculo del PID
        actuacionMotores (control_pwm); // Actuación sobre los motores
      }

      //tiempo_final = millis();}////

      // Elección de estado REPOSO
      if (digitalRead (BOTON_2) == HIGH)
      {
        delay (500);
        estado_finito = REPOSO;
        limpiezaVariables ();
      }

      break;

    /*
     * 4: Estado de elección de camino en bifurcación
     */
    case ELECCION_CAMINO:

      //VELOCIDAD_BASE = 50;

      //digitalWrite (LED_2, LOW);

      if (parar_rasterador_blanco > MARCA_PARADA_BLANCO || parar_rasterador_negro > MARCA_PARADA_NEGRO)
      {
        estado_finito = REPOSO;
        limpiezaVariables ();
      }

      lecturaCnys (); // Lectura de los sensores

      sensoresActivos ();

      anchoLectura(sensorIzquierda(lectura_CNYS_actual), sensorDerecha(lectura_CNYS_actual));

      bifurcacion = deteccionBifurcacion ();

      //Serial.print(diferencia); Serial.print(","); Serial.println(numero_lineas);

      if (bifurcacion == 0 && bifurcacion_pasada == 0 && diferencia <= 1)// sensores_activos_actual < 3)
      {
        //calculoDireccionRecto (); // Cálculo de la dirección actual
        calculoDireccionNormal (); // Cálculo de la dirección actual
        control_pwm = controlPD (); // Cálculo del PID
        actuacionMotores (control_pwm); // Actuación sobre los motores
      }

      if (bifurcacion == 1 || diferencia > 3)// sensores_activos_actual > 2)
      { digitalWrite (LED_2, LOW);
        if (lado_marca == 1) // Marca a la derecha
        {
          calculoDireccionDerecha (); // Sólo se lee por la derecha

          control_pwm = controlPD (); // Cálculo del PID

          actuacionMotores (control_pwm); // Actuación sobre los motores
        }

        if (lado_marca == 2) // Marca a la izquierda
        {
          calculoDireccionIzquierda (); // Sólo se lee por la izquierda

          control_pwm = controlPD (); // Cálculo del PID

          actuacionMotores (control_pwm); // Actuación sobre los motores
        }

        if (lado_marca == 3) // Marca a ambos lados
        {
          calculoDireccionRecto (); // Sólo se lee por el centro

          control_pwm = controlPD (); // Cálculo del PID

          actuacionMotores (control_pwm); // Actuación sobre los motores
        }

        bifurcacion_pasada = 1;
      }

      // Volver a SIGUELINEAS
      if (bifurcacion == 0 && bifurcacion_pasada == 1 && diferencia <= 1) //sensores_activos_actual < 3)
      {
        digitalWrite (LED_1, LOW);
        digitalWrite (LED_2, HIGH);
        digitalWrite (LED_3, LOW);
        estado_finito = SIGUELINEAS;
        limpiezaVariables ();
      }

      // Elección de estado REPOSO
      if (digitalRead (BOTON_2) == HIGH)
      {
        delay (500);
        estado_finito = REPOSO;
        limpiezaVariables ();
      }

      break;
  }

  //telemetria (); // Activamos la telemetría
}
