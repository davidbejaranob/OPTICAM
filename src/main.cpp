#include <Arduino.h>

const int LED = 15;
const int push_button = 23;

String brazo_1, brazo_2, brazo_3, flag;

///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////
#define encoder_a 13 // Green - pin 2 - Digital
#define encoder_b 12 // White - pin 3 - Digital
long encoder = 0;    // Contador de pasos del encoder

#define in2 25       // Pin que controla el sentido de giro motor
#define in1 26       // Pin que controla el sentido de giro motor
#define velocidad 33 // Pin que controla la velocidad de giro motor
int M1 = LOW;        // Estado de la terminal 1 del motor
int M2 = LOW;        // Estado de la terminal 2 del motor

// Filtros brazo 1
const int filtro_u = 360, filtro_g = 720;
float error_brazo1 = 0; // no se usa
int brazo1_goal;
// Filtros brazo 2
const int filtro_r = 360;
float error_brazo2 = 0; // no se usa
int brazo2_goal;
// Filtros Brazo 3
const int filtro_i = 360, filtro_z = 720;
float error_brazo3 = 0;
int brazo3_goal;
// Sin filtro
int vacio = 1080;

///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////

void blink_led()
{
  digitalWrite(LED, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
  digitalWrite(LED, LOW);  // turn the LED off by making the voltage LOW
  delay(500);
}

/*
String set_to_position() {
  for(int i; i<=99; i++) {

  }
  return 'algoooooooooooooo';
}
*/

///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////

void encoderPinChangeA()
{
  encoder += digitalRead(encoder_a) == digitalRead(encoder_b) ? -1 : 1;
}

void encoderPinChangeB()
{
  encoder += digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
}
///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(.1);

  pinMode(LED, OUTPUT);
  pinMode(push_button, INPUT);

  ///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////
  pinMode(encoder_a, INPUT_PULLUP);
  pinMode(encoder_b, INPUT_PULLUP);

  attachInterrupt(encoder_a, encoderPinChangeA, CHANGE);
  attachInterrupt(encoder_b, encoderPinChangeB, CHANGE);

  pinMode(in1, OUTPUT); // Configura los pines como salida
  pinMode(in2, OUTPUT);
  pinMode(velocidad, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(velocidad, 255); // Velocidad del motor
  ///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////
}

void loop()
{
  while (!Serial.available())
    ;
  brazo_1 = Serial.readString();
  if (brazo_1.equals("u") || brazo_1.equals("g") || brazo_1.equals("vacio"))
  {
    Serial.print(brazo_1);
    blink_led();
    if (brazo_1.equals("u"))
    {
      brazo1_goal = filtro_u;
    }
    if (brazo_1.equals("g"))
    {
      brazo1_goal = filtro_g;
    }
    if (brazo_1.equals("vacio"))
    {
      brazo1_goal = vacio;
    }

    while (!Serial.available())
      ;
    brazo_2 = Serial.readString();
    if (brazo_2.equals("r") || brazo_2.equals("vacio"))
    {
      Serial.print(brazo_2);
      blink_led();
      if (brazo_2.equals("r"))
      {
        brazo2_goal = filtro_r;
      }
      if (brazo_2.equals("vacio"))
      {
        brazo2_goal = vacio;
      }

      while (!Serial.available())
        ;
      brazo_3 = Serial.readString();
      if (brazo_3.equals("i") || brazo_3.equals("z") || brazo_3.equals("vacio"))
      {
        Serial.print(brazo_3);
        blink_led();
        if (brazo_3.equals("i"))
        {
          brazo3_goal = filtro_i;
        }
        if (brazo_3.equals("z"))
        {
          brazo3_goal = filtro_z;
        }
        if (brazo_3.equals("vacio"))
        {
          brazo3_goal = vacio;
        }

        while (!Serial.available())
          ;
        flag = Serial.readString();
        ////////////////////////////aqui adentro va el proceso de alineacion//////////////////////
        while (flag == "begin")
        {
          blink_led();
          ///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////
          while ((360 * encoder) / 2400 <= brazo1_goal)
          {
            // set_to_position();
            // Serial.println("Posicion: " + String((360*encoder)/2400) + " grados");
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
          }
          digitalWrite(in1, LOW);
          digitalWrite(in2, LOW);
          // Serial.println("Posicion: " + String((360*encoder)/2400) + " grados");
          flag = "done";
          encoder = 0;
          Serial.print(flag);
          ///////////////////////////////////////NUEVA PARTE////////////////////////////////////////////////
        }
      }
    }
  }
}