void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int intervalo_tarefa1 = 1000;
int intervalo_tarefa2 = 2000;
int intervalo_tarefa3 = 6400;
int tempo_agora_t1 = 0;
int tempo_agora_t2 = 0;
int tempo_agora_t3 = 0;
int estado_led = 1;

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - tempo_agora_t1 < intervalo_tarefa1){
    tempo_agora_t1 = millis();
    Serial.print("Entrei t1: ");
    Serial.println(tempo_agora_t1);

    if(millis() - tempo_agora_t1 > intervalo_tarefa1 & millis() - tempo_agora < intervalo_tarefa2){
      tempo_agora_t1 = millis();
      Serial.print("Entrei t2: ");
      Serial.println(tempo_agora_t1);
      if(millis() - tempo_agora_t1 > intervalo_tarefa1 & millis() - tempo_agora < intervalo_tarefa3){
        tempo_agora_t1 = millis();
        Serial.print("Entrei t3: ");
        Serial.println(tempo_agora_t1);
      }
    }
  }

  // // tarefa 2
  // if(millis() - tempo_agora_t2 > intervalo_tarefa2){
  //   tempo_agora_t2 = millis();
  //   Serial.print("Entrei t2: ");
  //   Serial.println(tempo_agora_t2);
  // }

  // // tarefa 3
  // if(millis() - tempo_agora_t3 > intervalo_tarefa3){
  //   tempo_agora_t3 = millis();
  //   Serial.print("Entrei t3: ");
  //   Serial.println(tempo_agora_t3);
  //   estado_led != estado_led;
  //   digitalWrite(LED_BUILTIN,estado_led);
  // }

}
