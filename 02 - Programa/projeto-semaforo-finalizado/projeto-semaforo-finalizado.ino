/*
  ==============================================================
  Semáforo com LEDs endereçaveis

  IDE Arduino: 1.8.13

  Autora: Téc. em Mecatrônica - Ivoneide Duarte
  Autor: Profº de Robótica - Sandro Mesquita
  Data: Janeiro de 2021 - Atualizado: Abril de 2021
  ==============================================================
*/

// ==============================================================//
// --- Declaração de bibliotecas ---
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <MFRC522.h>

// ==============================================================//
// --- Mapeamento de Hardware ---
#define pinMatrizLED_1  6
#define pinMatrizLED_2  7

#define pinMatrizLED_3  5
#define pinMatrizLED_4  4

#define numLEDVerde    24
#define numLEDVermelho 24

#define numLEDVerdePed    12
#define numLEDVermelhoPed 12

#define sensorChuva 10
#define botaoPedestre 8

#define SS_PIN 53
#define RST_PIN 9
#define ledVerde 2
#define ledVermelho 3

bool estadoBotao;
int flag = 0;
int tempVerde = 7000, tempVermelho = 10000;
unsigned long tempo, tempoAtual;
unsigned long cor;

Adafruit_NeoPixel matrizLEDVerde(numLEDVerde, pinMatrizLED_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrizLEDVermelho(numLEDVermelho, pinMatrizLED_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrizLEDVerdePed(numLEDVerdePed, pinMatrizLED_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrizLEDVermelhoPed(numLEDVermelhoPed, pinMatrizLED_4, NEO_GRB + NEO_KHZ800);

MFRC522 mfrc522(SS_PIN, RST_PIN);

char st[20];
// ==============================================================//
// --- Protótipos das Funções ---
void semaforoVerde();
void semaforoVermelho();
void semaforoAmarelo();
void desliga();
void rfid_func();
void efeitoSemaforoVerde();
void efeitoSemaforoAmarelo();

// ==============================================================//
// --- Configurações Iniciais ---
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  //Inicializa o semáforo dos veículos
  matrizLEDVerde.begin();
  matrizLEDVerde.show();
  matrizLEDVermelho.begin();
  matrizLEDVermelho.show();
  //Inicializa o semáforo dos pedestres
  matrizLEDVerdePed.begin();
  matrizLEDVerdePed.show();
  matrizLEDVermelhoPed.begin();
  matrizLEDVermelhoPed.show();

  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(sensorChuva, INPUT);
  pinMode(botaoPedestre, INPUT_PULLUP);

  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  tempo = millis();
}

// ==============================================================//
// --- Loop Infinito ---
void loop()
{
  tempoAtual = millis() - tempo;

  rfid_func();

  estadoBotao = digitalRead(botaoPedestre);

  if (!estadoBotao && !digitalRead(sensorChuva))
  {
    tempVerde = tempoAtual + 500;
  }
  else
  {
    if ((tempoAtual >= tempVermelho) && (flag == 0))
    {
      desliga();
      semaforoVerde();
      tempo = millis();
      tempVermelho = 10000;
    }
    else if ((tempoAtual >= tempVerde) && (flag == 1))
    {
      efeitoSemaforoVerde();
      desliga();
      semaforoAmarelo();
      tempVerde = 7000;
      tempo = millis();
    }
    else if ((tempoAtual >= 3000) && (flag == 2))
    {
      efeitoSemaforoAmarelo();
      desliga();
      semaforoVermelho();
      tempo = millis();
    }
  }
}

// --- Desenvolvimento das Funções
void semaforoVerde()
{
  //Veículos
  matrizLEDVerde.clear();
  cor = matrizLEDVerde.Color(0, 255, 8); //Cor verde
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Acende os LEDs
    matrizLEDVerde.setPixelColor(nL, cor);
    matrizLEDVerde.show();
  }
  //Pedestres
  cor = matrizLEDVermelhoPed.Color(255, 3, 3); //Cor vermelha
  for (int nL = 0; nL < numLEDVermelhoPed; nL++)
  { //Acende os LEDs
    matrizLEDVermelhoPed.setPixelColor(nL, cor);
    matrizLEDVermelhoPed.show();
  }
  flag = 1;
}

void semaforoVermelho()
{
  //Veículos
  matrizLEDVermelho.clear();
  cor = matrizLEDVermelho.Color(255, 3, 3); //Cor vermelha
  for (int nL = 0; nL < numLEDVermelho; nL++ )
  { //Acende os LEDs
    matrizLEDVermelho.setPixelColor(nL, cor);
    matrizLEDVermelho.show();
  }
  //Pedestres
  cor = matrizLEDVerdePed.Color(0, 255, 8); //Cor verde
  for (int nL = 0; nL < numLEDVerdePed; nL++)
  { //Acende os LEDs
    matrizLEDVerdePed.setPixelColor(nL, cor);
    matrizLEDVerdePed.show();
  }
  flag = 0;
}

void semaforoAmarelo()
{
  matrizLEDVerde.clear();
  cor = matrizLEDVerde.Color(100, 100, 0);
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Acende os LEDs
    matrizLEDVerde.setPixelColor(nL, cor);
    matrizLEDVerde.show();
  }

  matrizLEDVermelho.clear();
  cor = matrizLEDVermelho.Color(100, 100, 0);
  for (int nL = 0; nL < numLEDVermelho; nL++ )
  { //Acende os LEDs
    matrizLEDVermelho.setPixelColor(nL, cor);
    matrizLEDVermelho.show();
  }

  cor = matrizLEDVermelhoPed.Color(255, 3, 3);
  for (int nL = 0; nL < numLEDVermelho; nL++ )
  { //Acende os LEDs
    matrizLEDVermelhoPed.setPixelColor(nL, cor);
    matrizLEDVermelhoPed.show();
  }
  flag = 2;
}

void desliga()
{
  matrizLEDVerde.clear();
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Apaga os LEDs
    matrizLEDVerde.setPixelColor(nL, LOW);
    matrizLEDVerde.show();
  }

  matrizLEDVerde.clear();
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Apaga os LEDs
    matrizLEDVermelho.setPixelColor(nL, LOW);
    matrizLEDVermelho.show();
  }

  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Apaga os LEDs
    matrizLEDVerdePed.setPixelColor(nL, LOW);
    matrizLEDVerdePed.show();
  }
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Apaga os LEDs
    matrizLEDVermelhoPed.setPixelColor(nL, LOW);
    matrizLEDVermelhoPed.show();
  }
  delay(100);
}

void efeitoSemaforoVerde()
{
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Apaga os LEDs
    matrizLEDVerde.setPixelColor(nL, LOW);
    matrizLEDVerde.show();
    delay(50);
  }
}

void efeitoSemaforoAmarelo()
{
  for (byte x = 70; x > 0; x -= 10)
  {
    for (int nL = 0; nL < 24; nL++)
    {
      matrizLEDVermelho.setPixelColor(nL, LOW);
      matrizLEDVermelho.show();
      matrizLEDVerde.setPixelColor(nL, LOW);
      matrizLEDVerde.show();
      delay(x);
    }
    matrizLEDVerde.clear();
    matrizLEDVermelho.clear();
    cor = matrizLEDVerde.Color(100, 100, 0);
    for (int nL = 0; nL < 24; nL++)
    {
      matrizLEDVermelho.setPixelColor(nL, cor);
      matrizLEDVermelho.show();
      matrizLEDVerde.setPixelColor(nL, cor);
      matrizLEDVerde.show();
    }
  }
}

void rfid_func()
{
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  Serial.print("UID da tag: ");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "30 17 D4 5C")
  {
    Serial.println();
    Serial.println("Acesso liberado!");
    Serial.println();

    // Aciona LED verde
    //    digitalWrite(ledVerde, HIGH);
    //    delay(800);
    //    digitalWrite(ledVerde, LOW);
    //    delay(1200);

    tempVermelho *= 2; //Dobra o tempo do semáforo vermelho do carro
  }
  else
  {
    Serial.println();
    Serial.println("Acesso Negado");
    Serial.println();

    // Aciona LED vermelho
    //    digitalWrite(ledVermelho, HIGH);
    //    delay(800);
    //    digitalWrite(ledVermelho, LOW);
    //    delay(1200);
  }
}
