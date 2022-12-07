//IES LAS FUENTEZUELAS
//ALEJANDRO PÉREZ ARANDA

#include <dht_nonblocking.h>                                                                                //LIBRERIA DEL SENSOR DHT.
#define DHT_SENSOR_TYPE DHT_TYPE_11                                                                         //DEFINIMOS MODELO DEL SENSOR.

static const int DHT_SENSOR_PIN = 7;                                                                        //PIN DE DATOS DEL SENSOR.
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

static const int DHT1_SENSOR_PIN = 6;                                                                        //PIN DE DATOS DEL SENSOR 2.
DHT_nonblocking dht_sensor1( DHT1_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup( ){
  Serial.begin( 9600);                                                                                       //COMUNICACIÓN CON EL PUERTO SERIE.
  pinMode(13, OUTPUT);                                                                                       //LED AZUL (FUNCIONA EL SISTEMA).                                                    
  pinMode(12, OUTPUT);                                                                                       //LED VERDE.
  pinMode(11, OUTPUT);                                                                                       //LED ROJO.
}

                                                                                              
static bool measure_environment1( float *temperature, float *humidity ){                                     //RECOGEMOS DATOS DE TEMPERATURA DEL SENSOR 2.
  static unsigned long measurement_timestamp1 = millis( );

  if( millis( ) - measurement_timestamp1 > 3000ul )
  {
    if( dht_sensor1.measure( temperature, humidity ) == true )
    {
      measurement_timestamp1 = millis( );
      return( true );
    }
  }

  return( false );
}

static bool measure_environment( float *temperature, float *humidity ){                                      //RECOGEMOS DATOS DE TEMPERATURA DEL SENSOR 1.
  static unsigned long measurement_timestamp = millis( );

  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop( ){
  digitalWrite(13, HIGH);                                                                                     //INICIAMOS LED AZUL, PARA SABER QUE EL CIRCUITO FUNCIONA.
  float temperature;                                                                                          //VARIABLE TEMPERATURA.
  float humidity;                                                                                             //VARIABLE HUMEDAD.

  if( measure_environment( &temperature, &humidity ) < measure_environment1(&temperature, &humidity)){        //DHT1 < DHT2 = CIERRE DE PERSIANAS.
    digitalWrite(12, LOW);                                                                                    //LED VERDE APAGADO.
    digitalWrite(11, HIGH);                                                                                   //LED ROJO ENCENDIDO.
    Serial.println("Recomendacion: Cierre de persianas.");
  }

  if( measure_environment( &temperature, &humidity ) > measure_environment1(&temperature, &humidity)){        //DHT1 > DHT2 = APERTURA DE PERSIANAS.
    digitalWrite(11, LOW);                                                                                    //LED ROJO APAGADO.
    digitalWrite(12, HIGH);                                                                                   //LED VERDE ENCENDIDO.
    Serial.println("Recomendacion: Apertura de persianas.");
  }
  
  if( measure_environment( &temperature, &humidity ) > measure_environment1(&temperature, &humidity)){        //DHT1 == DHT2 = MISMA TEMPERATURA
    digitalWrite(11, HIGH);                                                                                   //LED ROJO ENCENDIDO.
    digitalWrite(12, HIGH);                                                                                   //LED VERDE ENCENDIDO.
    Serial.println("Misma temperatura.");
   }
  
}
