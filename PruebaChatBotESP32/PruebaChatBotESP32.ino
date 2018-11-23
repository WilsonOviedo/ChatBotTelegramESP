#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Wifi connection to the router
char ssid[] = "xxxxxxxxxxxxx";     // your network SSID (name)
char password[] = "xxxxxxxxxxx"; // your network key

// Initialize Telegram BOT
#define BOTtoken "xxxxxxxxx:xxxx-xxxxxxxxxxxxxxxxxx" // your Bot Token (Get from Botfather)

String texto;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/test") {
        bot.sendChatAction(chat_id, "typing");
        delay(4000);
        bot.sendMessage(chat_id, "¿Viste el mensaje de acción?");

        // You can't use own message, just choose from one of bellow

        //typing for text messages
        //upload_photo for photos
        //record_video or upload_video for videos
        //record_audio or upload_audio for audio files
        //upload_document for general files
        //find_location for location data

        //more info here - https://core.telegram.org/bots/api#sendchataction
    }

    if (text == "/start") {
      texto = "Bienvenido a Universal Arduino Telegram Bot library, " + from_name + ".\n";
      texto += "Ejemplo de chatBot.\n\n";
      texto += "/test : para realizar pruebas\n";
      bot.sendMessage(chat_id, texto);
    }else
        if (text == "/lucesOn") {
      texto = "Luces encendidas";
      bot.sendMessage(chat_id, texto);
    }else
        if (text == "/lucesOff") {
      texto = "Luces apagadas";
      bot.sendMessage(chat_id, texto);
    }
    else
        if (text == "/sensor") {
      texto = "Valor de sensor:"+ String(digitalRead(34));
      bot.sendMessage(chat_id, texto);
    }    
  }
}


void setup() {
  Serial.begin(115200);
pinMode(34,INPUT);
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
