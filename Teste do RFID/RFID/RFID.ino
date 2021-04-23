// --- Bibliotecas Auxiliares ---
#include <SPI.h>
#include <MFRC522.h>


// --- Mapeamento de Hardware ---
#define   SS_PIN    53
#define   RST_PIN    9
#define   led_green  8
#define   led_red    7
#define   buzzer     6
#define   door_lock  5
#define   door_sens  4

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria instância com MFRC522


// --- Protótipo das Funções Auxiliares ---
void rfid_func();                            //Função para identificação das tags RFID
 

// --- Variáveis Globais --- 
char st[20];



// --- Configurações Iniciais ---
void setup() 
{

  pinMode(led_green, OUTPUT);
  pinMode(led_red,   OUTPUT);
  pinMode(buzzer,    OUTPUT);
  pinMode(door_lock, OUTPUT);
  pinMode(door_sens, INPUT_PULLUP); //sensor de porta
  
  Serial.begin(9600);   // Inicia comunicação Serial em 9600 baud rate
  SPI.begin();          // Inicia comunicação SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();

  //saídas iniciam desligadas
  digitalWrite(door_lock, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_red,   LOW);
  digitalWrite(buzzer,    LOW);
   
  
} //end setup


// --- Loop Infinito ---
void loop() 
{
    rfid_func();   //chama função para identificação de tags RFID
 
} //end loop


// === Funções Auxiliares ===
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
      String conteudo= "";
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



      if (conteudo.substring(1) == "D6 96 BE F7") //tag Chaveiro para liberar acesso
      {
        Serial.println("Acesso liberado!");
        Serial.println();
        
        // Aciona LED verde e buzzer
        digitalWrite(led_green, HIGH);
        digitalWrite(buzzer,    HIGH);
        delay(800);
        digitalWrite(led_green,  LOW);
        digitalWrite(buzzer,     LOW);
        delay(1200);

        //Mantém acesso liberado até acionar o sensor de porta
        while(digitalRead(door_sens)) digitalWrite(door_lock, HIGH);
        digitalWrite(door_lock, LOW);
         
      }
     
      if (conteudo.substring(1) != "86 D0 17 7E") //outras tags
      {
        Serial.println("Acesso Negado");
        Serial.println();
        
        // Aciona LED vermelho e buzzer
        digitalWrite(led_red,   HIGH);
        digitalWrite(buzzer,    HIGH);
        delay(800);
        digitalWrite(led_red,    LOW);
        digitalWrite(buzzer,     LOW);
        delay(1200);
        
      }
  
  
  
} //end rfid_func















 
 
