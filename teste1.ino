int pino = 34;
int valorFiltrado = 0;

void setup() {
  Serial.begin(230400);
}

void loop() {
  int leitura = analogRead(pino);

  // filtro simples (média)
  valorFiltrado = (valorFiltrado * 0.9) + (leitura * 0.1);

  Serial.println(valorFiltrado);

  // DETECÇÃO
  if (valorFiltrado > 2200) {
    Serial.println("PISCADA DETECTADA");
  }


  delay(2);
}