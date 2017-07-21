/*
 * Función para configurar el módulo Bluethoot HC-05 en chipset
 * Necesario hacer un puente entre el pin 34 y VCC antes
 * de conectar el módulo para entrar en modo AT y mantenerlo así
 * hasta el aviso del LED_3
 */
void configurarBluethoot ()
{
  Serial.begin(38400); // Velocidad de puerto serie para modo AT

  // Tiempo para entrar en modo AT con el puente
  digitalWrite(LED_1, HIGH);
  delay(20000);
  digitalWrite(LED_1, LOW);


  Serial.println("AT+NAME=FENRIR"); delay(1000); // Nombre de la señal
  Serial.println("AT+PSWD=9351"); delay(1000); // Contraseña de la senal

  // Aviso de configuración terminada
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LED_3, HIGH);
    delay(100);
    digitalWrite(LED_3, HIGH);
    delay(100);
  }
}

/*
 * Función para telemetría
 */
void telemetria ()
{
  //Serial.print ("ESTADO: "); Serial.print(estado_finito); Serial.println ();

  /*Serial.print("\tControl: "); Serial.println(control_pwm);
  Serial.print("Motor derecho: "); Serial.print(md_pwm);
  Serial.print("\tMotor izquierdo: "); Serial.println(mi_pwm);
  Serial.println();
  delay(1200);*/

  /*Serial.print("CNY1:");Serial.print(analogRead(CNY_1));
  Serial.print("\tCNY2:");Serial.print(analogRead(CNY_2));
  Serial.print("\tCNY3:");Serial.print(analogRead(CNY_3));
  Serial.print("\tCNY4:");Serial.print(analogRead(CNY_4));
  Serial.print("\tCNY5:");Serial.print(analogRead(CNY_5));
  Serial.print("\tCNY6:");Serial.print(analogRead(CNY_6));
  Serial.print("\tCNY7:");Serial.print(analogRead(CNY_7));
  Serial.print("\tCNY8:");Serial.println(analogRead(CNY_8));
  delay(500);*/

  Serial.print("lectura_CNYS_actual: ");
  Serial.print(lectura_CNYS_actual[0]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[1]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[2]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[3]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[4]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[5]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[6]); Serial.print(",");
  Serial.print(lectura_CNYS_actual[7]); Serial.println(",");

  /*Serial.print("lectura_CNYS_anterior: ");
  Serial.print(lectura_CNYS_anterior[0]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[1]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[2]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[3]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[4]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[5]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[6]); Serial.print(",");
  Serial.print(lectura_CNYS_anterior[7]); Serial.println(",");

  Serial.print("lectura_CNYS: ");
  Serial.print(lectura_CNYS[0]); Serial.print(",");
  Serial.print(lectura_CNYS[1]); Serial.print(",");
  Serial.print(lectura_CNYS[2]); Serial.print(",");
  Serial.print(lectura_CNYS[3]); Serial.print(",");
  Serial.print(lectura_CNYS[4]); Serial.print(",");
  Serial.print(lectura_CNYS[5]); Serial.print(",");
  Serial.print(lectura_CNYS[6]); Serial.print(",");
  Serial.print(lectura_CNYS[7]); Serial.println(",");*/

  Serial.print("numero_lineas: "); Serial.println(numero_lineas);
  Serial.print("marca_real: "); Serial.println(marca_real);
  Serial.print("direccion: "); Serial.println(direccion);
  Serial.print("lado_marca: "); Serial.println(lado_marca);
  /*Serial.print("bifurcacion: "); Serial.println(bifurcacion);
  Serial.print("bifurcacion_pasada: "); Serial.println(bifurcacion_pasada);
  Serial.println();*/

  delay(250);

///////////////////////////////////
  /*while(Serial.available() > 1) // Para leer al menos dos bytes
  {
    byte entrada = Serial.read();

    switch (entrada)
    {
      case 'V': // Velocidad base
      {
        int velocidad = Serial.parseInt();

        if(velocidad == 0)
        {
          Serial.print("La velocidad es: "); Serial.println(VELOCIDAD_BASE);
        }
        else
        {
          VELOCIDAD_BASE = velocidad;
          Serial.print("La nueva velocidad es: "); Serial.println(VELOCIDAD_BASE);
        }
      }
        break;

      case 'P': // Controlador proporcional
      {
        int proporcional = Serial.parseInt();

        if(proporcional == 0)
        {
          Serial.print("Kp es: "); Serial.println(KP);
        }
        else
        {
          KP = proporcional;
          Serial.print("La nueva Kp es: "); Serial.println(KP);
        }
      }
        break;

      case 'D': // Controlador derivativo
      {
        int derivativo = Serial.parseInt();

        if(derivativo == 0)
        {
          Serial.print("Kd es: "); Serial.println(KD);
        }
        else
        {
          KD = derivativo;
          Serial.print("La nueva Kd es: "); Serial.println(KD);
        }
      }
        break;

      case 'E': // Para cambiar de estado finitos
      {
        int estado = Serial.parseInt();

        if(estado == 1)
        {
          estado_finito = REPOSO;
          Serial.println("El estado ahora es REPOSO");
        }
        else if (estado == 2)
        {
          estado_finito = SIGUELINEAS;
          Serial.println("El estado ahora es SIGUELINEAS");
        }
        else
        {
          Serial.println("Se mantiene el estado actual");
        }
      }
        break;
    }
  }*/

}
