/*
  ==============================================================
  Semáforo

  IDE Arduino: 1.8.13

  Autora: Téc. em Mecatrônica - Ivoneide Duarte
  Data: Janeiro de 2021
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

#define   SS_PIN      53
#define   RST_PIN     9
#define   ledVerde    2 //Cartão rfid
#define   ledVermelho 3 //Cartão rfid

bool estadoBotao;
int flag = 0;
int tempo_verm = 10000;
unsigned long tempo, tempoAtual;
unsigned long cor;

Adafruit_NeoPixel matrizLEDVerde(numLEDVerde, pinMatrizLED_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrizLEDVermelho(numLEDVermelho, pinMatrizLED_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrizLEDVerdePed(numLEDVerdePed, pinMatrizLED_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel matrizLEDVermelhoPed(numLEDVermelhoPed, pinMatrizLED_4, NEO_GRB + NEO_KHZ800);

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância com MFRC522

char st[20];

// ==============================================================//
// --- Protótipos das Funções ---
void semaforoVerde();
void semaforoAmarelo();
void semaforoVermelho();
void desliga();
void rfid_func();

// ==============================================================//
// --- Configurações Iniciais ---
void setup()
{
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

  pinMode(ledVerde, OUTPUT);      //Cartão rfid
  pinMode(ledVermelho,   OUTPUT); //Cartão rfid

  Serial.begin(9600);   // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522

  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  //saídas iniciam desligadas
  digitalWrite(ledVerde, LOW);     //Cartão rfid
  digitalWrite(ledVermelho, LOW);  //Cartão rfid

  pinMode(sensorChuva, INPUT);
  pinMode(botaoPedestre, INPUT_PULLUP);

  tempo = millis();
} //end setup

// ==============================================================//
// --- Loop Infinito ---
void loop()
{
  tempoAtual = millis() - tempo;

  rfid_func();   //chama função para identificação de tags RFID

  estadoBotao = digitalRead(botaoPedestre);
  if (!estadoBotao && !digitalRead(sensorChuva) && (flag == 0))
  {
    tempo_verm = tempoAtual + 1000;
  }
  else
  {
    if ((tempoAtual >= tempo_verm) && (flag == 0))
    {
      //      digitalWrite(ledVermelho, LOW);
      //      digitalWrite(ledAmarelo,  LOW);
      //      digitalWrite(ledVerde,    HIGH);
      //
      //      digitalWrite(ledVermelhoPedestre, HIGH);
      //      digitalWrite(ledVerdePedestre, LOW);

      semaforoVerde();
      desliga();

      flag = 1;
      tempo = millis();
      tempo_verm = 10000;
    }
    else if ((tempoAtual >= 7000) && (flag == 1))
    {
      //      digitalWrite(ledVermelho, LOW);
      //      digitalWrite(ledAmarelo,  HIGH);
      //      digitalWrite(ledVerde,    LOW);
      //
      //      digitalWrite(ledVermelhoPedestre, HIGH);
      //      digitalWrite(ledVerdePedestre, LOW);

      semaforoAmarelo();
      desliga();

      flag = 2;
      tempo = millis();
    }
    else if ((tempoAtual >= 3000) && (flag == 2))
    {
      //      digitalWrite(ledVermelho, HIGH);
      //      digitalWrite(ledAmarelo,  LOW);
      //      digitalWrite(ledVerde,    LOW);
      //
      //      digitalWrite(ledVermelhoPedestre, LOW);
      //      digitalWrite(ledVerdePedestre, HIGH);

      semaforoVermelho();
      desliga();

      flag = 0;
      tempo = millis();
    }
  }
} //end loop


// ==============================================================//
// --- Desenvolvimento das Funções ---

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
  delay(5000);
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Apaga os LEDs
    matrizLEDVerde.setPixelColor(nL, LOW);
    matrizLEDVerde.show();
    delay(50);
  }
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
  delay(8000);
  for (int nL = 0; nL < numLEDVermelho; nL++ )
  { //Apaga os LEDs
    matrizLEDVermelho.setPixelColor(nL, LOW);
    matrizLEDVermelho.show();
    delay(50);
  }
  for (int nL = 0; nL < numLEDVerdePed; nL++ )
  { //Apaga os LEDs
    matrizLEDVerdePed.setPixelColor(nL, LOW);
    matrizLEDVerdePed.show();
    delay(50);
  }
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
  delay(2000);
  for (byte x = 70; x > 0; x = x - 10) {
    for (int nL = 0; nL < 24; nL++ )
    { //Acende os LEDs
      matrizLEDVermelho.setPixelColor(nL, LOW);
      matrizLEDVermelho.show();
      matrizLEDVerde.setPixelColor(nL, LOW);
      matrizLEDVerde.show();
      delay(x);
    }
    matrizLEDVerde.clear();
    matrizLEDVermelho.clear();
    cor = matrizLEDVerde.Color(100, 100, 0);
    for (int nL = 0; nL < 24; nL++ )
    { //Acende os LEDs
      matrizLEDVermelho.setPixelColor(nL, cor);
      matrizLEDVermelho.show();
      matrizLEDVerde.setPixelColor(nL, cor);
      matrizLEDVerde.show();
    }
  }
  //Apaga o semáforo do pedestre vermelho
  for (int nL = 0; nL < numLEDVermelhoPed; nL++ )
  { //Apaga os LEDs
    matrizLEDVermelhoPed.setPixelColor(nL, LOW);
    matrizLEDVermelhoPed.show();
    delay(50);
  }
}

void desliga()
{
  matrizLEDVerde.clear();
  for (int nL = 0; nL < numLEDVerde; nL++ )
  { //Acende os LEDs
    matrizLEDVerde.setPixelColor(nL, LOW);
    matrizLEDVerde.show();
  }

  matrizLEDVermelho.clear();
  for (int nL = 0; nL < numLEDVermelho; nL++ )
  { //Acende os LEDs
    matrizLEDVermelho.setPixelColor(nL, LOW);
    matrizLEDVermelho.show();
  }
  delay(100);
}

void rfid_func()                            //Função para identificação das tags RFID
{
  // -- Verifica novas tags --
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleciona um dos cartões
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  // Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();


  if (conteudo.substring(1) == "50 0A CE A5") //tag Chaveiro para liberar acesso
  {
    Serial.println("Acesso liberado!");
    Serial.println();
    
    // Aciona LED verde e buzzer
    digitalWrite(ledVerde, HIGH);
    delay(800);
    digitalWrite(ledVerde,  LOW);
    delay(1200);
  }

  if (conteudo.substring(1) != "50 0A CE A5") //outras tags
  {
    Serial.println("Acesso Negado");
    Serial.println();

    // Aciona LED vermelho e buzzer
    digitalWrite(ledVermelho,   HIGH);
    delay(800);
    digitalWrite(ledVermelho,    LOW);
    delay(1200);
  }
} //end rfid_func
