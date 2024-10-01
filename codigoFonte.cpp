#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

const char* ssid = "Wokwi-GUEST";
const char* senha = "";
const char* servidor_mqtt = "test.mosquitto.org";

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinoVRx = 34;
const int pinoVRy = 35;
const int pinoSW = 32;

const int pinoTrig = 5;
const int pinoEcho = 18;

Ultrasonic ultrassonico(pinoTrig, pinoEcho);

WiFiClient clienteWiFi;
PubSubClient clienteMQTT(clienteWiFi);

const int zonaMortaMin = 1800;
const int zonaMortaMax = 2300;
const int limiteMovimentoBaixo = 1000;
const int limiteMovimentoAlto = 3000;

int ultimoValorX = 2048;
int ultimoValorY = 2048;
int ultimoEstadoBotao = HIGH;

void callback(char* topico, byte* mensagem, unsigned int comprimento) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.print(topico);
  Serial.print(". Mensagem: ");
  for (int i = 0; i < comprimento; i++) {
    Serial.print((char)mensagem[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");

  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("Conectando WiFi");
  }
  Serial.println("Conectado ao Wi-Fi");
  lcd.setCursor(0, 1);
  lcd.print("WiFi Conectado");

  clienteMQTT.setServer(servidor_mqtt, 1883);
  clienteMQTT.setCallback(callback);

  while (!clienteMQTT.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (clienteMQTT.connect("ESP32ClienteTest")) {
      Serial.println("Conectado ao MQTT");
      clienteMQTT.subscribe("dash/game/control");
      lcd.setCursor(0, 1);
      lcd.print("MQTT Conectado  ");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" Tentando novamente...");
      delay(5000);
    }
  }
  
  pinMode(pinoVRx, INPUT);
  pinMode(pinoVRy, INPUT);
  pinMode(pinoSW, INPUT_PULLUP);
}

void verificaConexaoMQTT() {
  if (!clienteMQTT.connected()) {
    while (!clienteMQTT.connected()) {
      Serial.print("Reconectando ao MQTT...");
      if (clienteMQTT.connect("ESP32ClienteTest")) {
        Serial.println("Conectado ao MQTT");
        clienteMQTT.subscribe("dash/game/control");
        lcd.setCursor(0, 1);
        lcd.print("MQTT Reconectado");
      } else {
        Serial.print("Falha, rc=");
        Serial.print(clienteMQTT.state());
        Serial.println(" Tentando novamente...");
        delay(5000);
      }
    }
  }
}

void loop() {
  verificaConexaoMQTT();
  clienteMQTT.loop();

  int valorX = analogRead(pinoVRx);
  int valorY = analogRead(pinoVRy);
  int estadoBotao = digitalRead(pinoSW);

  lcd.setCursor(0, 0);
  lcd.print("Movimento: ");

  String movimento = "";
  if (valorX < limiteMovimentoBaixo && valorX != ultimoValorX) {
    movimento = "ESQUERDA";
    lcd.setCursor(0, 1);
    lcd.print("ESQUERDA         ");
  } else if (valorX > limiteMovimentoAlto && valorX != ultimoValorX) {
    movimento = "DIREITA";
    lcd.setCursor(0, 1);
    lcd.print("DIREITA          ");
  }

  if (valorX < zonaMortaMin || valorX > zonaMortaMax) {
    ultimoValorX = valorX;
  }

  if (valorY < limiteMovimentoBaixo && valorY != ultimoValorY) {
    movimento = "FREAR";
    lcd.setCursor(0, 1);
    lcd.print("FREAR            ");
  } else if (valorY > limiteMovimentoAlto && valorY != ultimoValorY) {
    movimento = "ACELERAR";
    lcd.setCursor(0, 1);
    lcd.print("ACELERAR         ");
  }

  if (valorY < zonaMortaMin || valorY > zonaMortaMax) {
    ultimoValorY = valorY;
  }

  if (estadoBotao != ultimoEstadoBotao) {
    if (estadoBotao == LOW) {
      movimento = "ACAO";
      lcd.setCursor(0, 1);
      lcd.print("ACAO             ");
    }
    ultimoEstadoBotao = estadoBotao;
  }

  float distancia = ultrassonico.read();
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  int forcaSinal = map(distancia, 0, 400, 100, 0);
  forcaSinal = constrain(forcaSinal, 0, 100);

  if (movimento != "") {
    clienteMQTT.publish("dash/game/movimento", movimento.c_str());
  }

  char sinalStr[8];
  sprintf(sinalStr, "%d", forcaSinal);
  clienteMQTT.publish("dash/game/sinal", sinalStr);

  lcd.setCursor(0, 1);
  lcd.print("Sinal BT: ");
  lcd.print(forcaSinal);
  lcd.print("%     ");

  delay(50);  
}
