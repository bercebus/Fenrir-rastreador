/*
 * FUNCIONES QUE CONTROLAN LOS MOTORES
 */
# define ADELANTE 0
# define ATRAS 1
# define STOP 2

/*
 * Función de control del motor izquierdo
 */
void motorIzquierdo (byte sentido)
{
  switch (sentido)
  {
    case ADELANTE:
      digitalWrite (ENTRADA_A_MOTOR_IZQUIERDO, HIGH);
      digitalWrite (ENTRADA_B_MOTOR_IZQUIERDO, LOW);
      break;
    case ATRAS:
      digitalWrite (ENTRADA_A_MOTOR_IZQUIERDO, LOW);
      digitalWrite (ENTRADA_B_MOTOR_IZQUIERDO, HIGH);
      break;
    case STOP:
      digitalWrite (ENTRADA_A_MOTOR_IZQUIERDO, HIGH);
      digitalWrite (ENTRADA_B_MOTOR_IZQUIERDO, HIGH);
      break;
  }
}

/*
 * Función de control del motor derecho
 */
void motorDerecho (byte sentido)
{
  switch (sentido)
  {
    case ADELANTE:
      digitalWrite (ENTRADA_A_MOTOR_DERECHO, HIGH);
      digitalWrite (ENTRADA_B_MOTOR_DERECHO, LOW);
      break;
    case ATRAS:
      digitalWrite (ENTRADA_A_MOTOR_DERECHO, LOW);
      digitalWrite (ENTRADA_B_MOTOR_DERECHO, HIGH);
      break;
    case STOP:
      digitalWrite (ENTRADA_A_MOTOR_DERECHO, HIGH);
      digitalWrite (ENTRADA_B_MOTOR_DERECHO, HIGH);
      break;
   }
}

/*
 * Función de control de velocidad de los motores
 */
void controlMotores (int motor_izquierdo, int motor_derecho)
{
  // Para motor izquierdo
  if(motor_izquierdo > 0)
  {
    if(motor_izquierdo > 255)
    {
      motor_izquierdo = 255;
    }
    motorIzquierdo (ADELANTE);
    analogWrite (PWM_MOTOR_IZQUIERDO, motor_izquierdo);
  }
  else if(motor_izquierdo < 0)
  {
    if(motor_izquierdo < -255)
    {
      motor_izquierdo = -255;
    }
    motorIzquierdo (ATRAS);
    analogWrite (PWM_MOTOR_IZQUIERDO, -motor_izquierdo);
  }
  else
  {
    motorIzquierdo (STOP);
    analogWrite (PWM_MOTOR_IZQUIERDO, 255);
  }

  // Para motor derecho
  if(motor_derecho > 0)
  {
    if(motor_derecho > 255)
    {
      motor_derecho = 255;
    }
    motorDerecho (ADELANTE);
    analogWrite (PWM_MOTOR_DERECHO, motor_derecho);
  }
  else if(motor_derecho < 0)
  {
    if(motor_derecho < -255)
    {
      motor_derecho = -255;
    }
    motorDerecho (ATRAS);
    analogWrite (PWM_MOTOR_DERECHO,-motor_derecho);
  }
  else
  {
    motorDerecho (STOP);
    analogWrite (PWM_MOTOR_DERECHO, 255);
  }
}
