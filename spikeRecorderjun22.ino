#define SINAL_PIN 34 // Seu pino de sinal do AD8232

void setup() {
  // O Spike Recorder se comunica exatamente nesta velocidade
  Serial.begin(222222); 
  pinMode(SINAL_PIN, INPUT);
}

void loop() {
  // Lê o sinal bruto do cérebro (0 a 4095)
  int sinalBruto = analogRead(SINAL_PIN);
  
  // O Spike Recorder precisa receber os dados divididos em 2 "pedaços" (bytes)
  byte primeiroByte = (sinalBruto >> 7) & 0x1F;
  byte segundoByte = sinalBruto & 0x7F;
  
  // Envia o formato exato que o aplicativo entende
  Serial.write(primeiroByte);
  Serial.write(segundoByte);
  
  // Mantém o ritmo de envio perfeito do aplicativo
  delayMicroseconds(100); 
}