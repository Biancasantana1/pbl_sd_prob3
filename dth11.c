#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
//#include <MQTTClient.h>

//USE WIRINGPI PIN NUMBERS
#define LCD_RS  6               //Register select pin
#define LCD_E   31               //Enable Pin
#define LCD_D4  26               //Data pin 4
#define LCD_D5  27               //Data pin 5
#define LCD_D6  28               //Data pin 6
#define LCD_D7  29

#define BUTTON_0 21
#define BUTTON_1 24
#define BUTTON_2 25

//Data pin 7
#define MAXTIMINGS 85
#define DHTPIN 7
/* Caso desejar utilizar outro broker MQTT, substitua o endereco abaixo */
#define MQTT_ADDRESS   "tcp://iot.eclipse.org"
/* Substitua este por um ID unico em sua aplicacao */
#define CLIENTID       "MQTTCClientID"
//MQTTClient client;

// void publish(MQTTClient client, char* topic, char* payload);
// int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);

int lcd;
int dht11_dat[5] = {0, 0, 0, 0, 0};
int intervalo_medicao = 2000; //em ms
int temperatura = 0;
int umidade = 0;
int umidadeDecimal = 0;


// void publish(MQTTClient client, char* topic, char* payload) {
//     MQTTClient_message pubmsg = MQTTClient_message_initializer;

//     pubmsg.payload = payload;
//     pubmsg.payloadlen = strlen(pubmsg.payload);
//     pubmsg.qos = 2;
//     pubmsg.retained = 0;
//     MQTTClient_deliveryToken token;
//     MQTTClient_publishMessage(client, topic, &pubmsg, &token);
//     MQTTClient_waitForCompletion(client, token, 1000L);
// }

// int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
//     char* payload = message->payload;

//     /* Mostra a mensagem recebida */
//     printf("Mensagem recebida! \n\rTopico: %s Mensagem: %s\n", topicName, payload);

//     /* Faz echo da mensagem recebida */
//     publish(client, MQTT_PUBLISH_TOPIC, payload);

//     MQTTClient_freeMessage(&message);
//     MQTTClient_free(topicName);
//     return 1;
// }

void read_dht11_dat()
{
        // uint8_t laststate = HIGH;
        // uint8_t counter = 0;
        // uint8_t j = 0, i;
        // float f; 

        // dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

        // pinMode(DHTPIN, OUTPUT);
        // digitalWrite(DHTPIN, LOW);
        // delay(18);
        
        // digitalWrite(DHTPIN, HIGH);
        // delayMicroseconds(40);
        
        // pinMode(DHTPIN, INPUT);

        // for (i = 0; i < MAXTIMINGS; i++)
        // {
        //         counter = 0;
        //         while (digitalRead(DHTPIN) == laststate)
        //         {
        //                 counter++;
        //                 delayMicroseconds(1);
        //                 if (counter == 255)
        //                 {
        //                         break;
        //                 }
        //         }
        //         laststate = digitalRead(DHTPIN);

        //         if (counter == 255)
        //                 break;

        //         if ((i >= 4) && (i % 2 == 0))
        //         {
        //                 dht11_dat[j / 8] <<= 1;
        //                 if (counter > 16)
        //                         dht11_dat[j / 8] |= 1;
        //                 j++;
        //         }
        //  }

        // if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)))
        // {
        //         f = dht11_dat[2] * 9. / 5. + 32;

        //         dht11_dat[2] = rand() % 100// Celsius
        //         dht11_dat[0] = rand() % 100// Umidade
        //         dht11_dat[1] = rand() % 100// Umidade decicmal

        //         lcdPosition(lcd, 0, 0);
        //         lcdPrintf(lcd, "Humidity: %d.%d %%\n", dht11_dat[0], dht11_dat[1]);

        //         lcdPosition(lcd, 0, 1);
        //         lcdPrintf(lcd, "Temp: %d.0 C", dht11_dat[2]); //Uncomment for Celsius
        //         //lcdPrintf(lcd, "Temp: %.1f F", f); //Comment out for Celsius
        // }

        temperatura = dht11_dat[2] = rand() % 100;// Celsius
        umidade = dht11_dat[0] = rand() % 100;// Umidade
        umidadeDecimal = dht11_dat[1] = rand() % 100;// Umidade decicmal
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "H: %d.%d %%\n", umidade, umidadeDecimal);
        //printf("H: %d.%d %%\n", umidade, umidadeDecimal);
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "Temp: %d.0 C", temperatura);
        //printf("Temp: %d.0 C", temperatura);       
        
        //lcdPrintf(lcd, "Temp: %.1f F", f); //Comment out for Celsius
}

void *getMeasurement(){
        while (1)
        {
                read_dht11_dat();
                delay(intervalo_medicao); 
        }
}

void mostrarMedidas(){
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "H: %d.%d %%\n", umidade, umidadeDecimal);
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "Temp: %d.0 C", temperatura);
}

void mostrarSelecaoDeIntervalo(){
        lcdPosition(lcd, 0, 0);
        lcdPrintf(lcd, "Intervalo de Medicao (em ms)");
        lcdPosition(lcd, 0, 1);
        lcdPrintf(lcd, "%d ms", intervalo_medicao);
}

int main(void)
{		
	// int rc;
	// MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	// /* Inicializacao do MQTT (conexao & subscribe) */
	// MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	// MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

	// rc = MQTTClient_connect(client, &conn_opts);

	// if (rc != MQTTCLIENT_SUCCESS)
	// {
	// 	printf("\n\rFalha na conexao ao broker MQTT. Erro: %d\n", rc);
	// }

	// MQTTClient_subscribe(client, MQTT_SUBSCRIBE_TOPIC, 0);        
        wiringPiSetup();
        lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

        pthread_t tid;
        pthread_create(&tid, NULL, getMeasurement, NULL);

        //Logica dos botoes
        // DOIS MODOS NO DISPLAY //
                // MODO MEDIDAS
                // MODO INTERVALO
        int modo = 1;
        pinMode(BUTTON_0, INPUT);           


        while(1){
                if(digitalRead(BUTTON_0) == LOW){
                        printf("%d", modo); 
                        modo = !modo;                                               
                        delay(20);
                        while(digitalRead(7) == LOW); // aguarda enquato chave ainda esta pressionada           
                        delay(20); 
                }
                if(modo == 1) mostrarMedidas();
                if (modo == 0) mostrarSelecaoDeIntervalo();
                delay(1000);            
        }

        
        
        return(0);      
}
