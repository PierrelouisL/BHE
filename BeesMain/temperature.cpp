#include "bees.h"
#include <MaximWire.h>
#include <Arduino_HTS221.h>
#include <DHT.h>
#include <DHT_U.h>

#define MAXIMWIRE_EXTERNAL_PULLUP

// DHT Sensor
#define DHTPIN 8 // broche ou l'on a branche le capteur
#define DHTTYPE DHT22 // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);//déclaration du capteur

// DS18B20 Sensor
#define PIN_BUS 9
MaximWire::Bus bus(PIN_BUS);
MaximWire::DS18B20 device;

float h_dht22, t_dht22;
float temperature_carte, humidity_carte;

float echantillon[3];// echantillon des 3 capteurs float, apres get_DS18B20()

void get_DHT22(){ 
  // La lecture du capteur prend 250ms
  // Les valeurs lues peuvet etre vieilles de jusqu'a 2 secondes (le capteur est lent)
  h_dht22 = dht.readHumidity();//on lit l'hygrometrie
  t_dht22 = dht.readTemperature();//on lit la temperature en celsius (par defaut)
}

void get_DS18B20(){
  unsigned short nbr_sensors = 1;
  MaximWire::Discovery discovery = bus.Discover();
  do {
    if (nbr_sensors > 3){
      break;
    }
    MaximWire::Address address;
    if (discovery.FindNextDevice(address)){
        if (address.GetModelCode() == MaximWire::DS18B20::MODEL_CODE){
          MaximWire::DS18B20 device(address);
          float temp = device.GetTemperature<float>(bus);
          echantillon[nbr_sensors - 1] = temp;
          device.Update(bus);
        }
    nbr_sensors++;
    } 
  } while (discovery.HaveMore());
}

void get_tempboard(){
  temperature_carte = HTS.readTemperature();
  humidity_carte    = HTS.readHumidity();
}

void get_temp() {
  delay(3000);
  get_DS18B20();
  get_DHT22();
  get_tempboard();
}
