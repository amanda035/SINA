#include "arduinoFFT.h"

#define SINAL_PIN 34          

// Dobramos as amostras e a frequência para fugir do ruído de 60Hz
const uint16_t samples = 256; 
const double samplingFrequency = 200; 

ArduinoFFT<double> FFT = ArduinoFFT<double>();

double vReal[samples];
double vImag[samples];

unsigned long sampling_period_us;
unsigned long microseconds;

void setup() {
  Serial.begin(115200);
  pinMode(SINAL_PIN, INPUT);
  sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
}

void loop() {
  // 1. CAPTURA DOS DADOS + BALANÇO DE ZERO AUTOMÁTICO
  // Em vez de usar um número fixo (como 2048), o código agora calcula a média real 
  // do seu sinal e subtrai ela. Isso zera o ruído elétrico de fundo sozinho.
  
  double somaSinal = 0;
  for (int i = 0; i < samples; i++) {
    microseconds = micros();
    vReal[i] = analogRead(SINAL_PIN);
    somaSinal += vReal[i];
    while (micros() < (microseconds + sampling_period_us)) {}
  }
  
  double mediaSinal = somaSinal / samples;
  for (int i = 0; i < samples; i++) {
    vReal[i] -= mediaSinal; // Aplica o balanço de zero dinâmico
    vImag[i] = 0; 
  }

  // 2. PROCESSAMENTO DA FFT
  FFT.windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(vReal, vImag, samples, FFT_FORWARD);
  FFT.complexToMagnitude(vReal, vImag, samples);

  // 3. NOVA CALIBRAÇÃO DA ONDA BETA (12Hz a 30Hz)
  // Com 200Hz de amostragem, cada caixinha da FFT agora vale 0.78125 Hz.
  // Caixinha Inicial (12 Hz) = 12 / 0.78125 = 15
  // Caixinha Final (30 Hz) = 30 / 0.78125 = 38
  
  double potenciaBeta = 0;
  for (int i = 15; i <= 38; i++) {
    potenciaBeta += vReal[i];
  }

// 4. ENVIO PARA O SERIAL PLOTTER COM ESCALA TRAVADA
  Serial.print("Onda_Beta_Filtrada:");
  Serial.print(potenciaBeta);
  Serial.print(","); // Adiciona uma vírgula para enviar mais dados
  
  Serial.print("Limite_Superior:");
  Serial.print(20000); // Trava o topo do gráfico em 20000
  Serial.print(",");
  
  Serial.print("Limite_Inferior:");
  Serial.println(0); // Trava o fundo do gráfico em 0

  delay(50);
}