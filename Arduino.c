#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// DEFINIÇÃO DE VARIÁVEIS SIMPLES
char ordemInserida[] = {'0', '0', '0', '0'};
int contador = 0;
char ordemCorreta[] = {'2', '1', '4', '3'};
int contTentativas = 0;
int valor = 0;

// DEFINIÇÃO DO LCD
LiquidCrystal_I2C lcd(39,16,2);

// DEFINIÇÃO BOTÕES
int b1 = 0;
int b2 = 0;
int b3 = 0;
int b4 = 0;
int b5_fechar = 0;

//DEFINIÇÃO DO BUZZER
int pinoBuzzer = A3;

// DEFININÇÃO SERVO MOTOR
int pos = 0;
Servo servo_3;

// LEDS
int vermelho = 5;
int verde = 4;

// lcd.print("                ");

void setup()
{
  Serial.begin(9600);

  // BOTÕES
  pinMode (11, INPUT);
  pinMode (10, INPUT);
  pinMode (9, INPUT);
  pinMode (8, INPUT);
  pinMode (2, INPUT);

  // BUZZER
  pinMode(pinoBuzzer, OUTPUT);

  // SERVO
  servo_3.write(0);
  servo_3.attach(3, 500, 2500);

  // LEDs
  pinMode(vermelho, OUTPUT);
  pinMode(verde, OUTPUT);

  lcd.init();
  lcd.backlight(); 

  lcd.print("Acione os botoes");
  lcd.setCursor(0,1);
  lcd.print("na ordem correta");
  delay(1000);

  lcd.clear();

  lcd.print("Sequencia feita");
  lcd.setCursor(0,1);
  lcd.print("de 4 botoes");
  delay(1000);

  lcd.clear();
  
  lcd.print("Voce tem 3");
  lcd.setCursor(0,1);
  lcd.print("tentativas");
  delay(1000);
  
  lcd.clear();

  atualizarLEDs(false);
}

void loop(){
  b1 = digitalRead(11);
  b2 = digitalRead(10);
  b3 = digitalRead(9);
  b4 = digitalRead(8);
  b5_fechar = digitalRead(2);
  
  valor=analogRead(A2);
  float c = (valor-102)/2;
  alarmeTemp(c);

  if(b1 == HIGH) {
	botaoPressionado(1, "Botao 1");
  }

  if(b2 == HIGH) {
    botaoPressionado(2, "Botao 2");
  }

  if(b3 == HIGH) {
    botaoPressionado(3, "Botao 3");
  }

 if(b4 == HIGH) {
    botaoPressionado(4, "Botao 4");
  }

  if(b5_fechar == HIGH) {
    abrirFechar(false);
  }

}

// SALVAR SEQUÊNCIA

void botaoPressionado(int b, String msg) {
  Serial.print("\n\n\n\n\n\n\n\n"); // Limpar Terminal
  
  ordemInserida[contador] = b + '0';

  lcd.print(msg);
  delay(1000);
  lcd.clear();

  Serial.println("==================");
  Serial.println(msg + " pressionado");
  for(int i = 0;i < (sizeof(ordemInserida) 
         / sizeof(ordemInserida[0])); i++) {
    Serial.print(ordemInserida[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.println("==================");

  delay(500);
  contador++;

  verificar(contador);

}

// ATUALIZAR OS LEDs

void atualizarLEDs(bool caso) {

  if (caso) {

    Serial.println("Ligando LED verde");
    digitalWrite(vermelho, LOW);
    digitalWrite(verde, HIGH);

  } else {

    Serial.println("Ligando LED vermelho");
    digitalWrite(vermelho, HIGH);
    digitalWrite(verde, LOW);

  }
}

// ABRIR E FECHAR O COFRE
void abrirFechar(bool acesso) {

  if(acesso) {
    
    atualizarLEDs(true);

    lcd.clear();
    lcd.print("Abrindo o cofre");
    Serial.println("Abrindo o Cofre");
    
    for(pos = 0; pos <= 90; pos++) {

      servo_3.write(pos);
      delay(15);

    }

    delay(500);
    lcd.clear();

  } else {

    atualizarLEDs(false);

    lcd.clear();
    lcd.print("Fechando o cofre");
    Serial.println("Fechando o Cofre");
    for(pos = 90; pos >= 0; pos--) {

      servo_3.write(pos);
      delay(15);   

    }

    delay(500);

    lcd.clear();

  }

}

// BUZZER
void alarme(bool bloqueio) {
  if (bloqueio) {
  	tone(pinoBuzzer, 259);
  } else {
    noTone(pinoBuzzer);
  }
}

void alarmeTemp(float tempAtual) {
  if (tempAtual >= 50) {
    alarme(true); // Liga o som
  
    while(tempAtual >= 50) {
      lcd.clear();
      lcd.print("TEMPERATURA: ");
      lcd.print(tempAtual);
      lcd.setCursor(0,1);
      lcd.print("AGUARDE ESFRIAR");

      delay(1000);

      valor = analogRead(A2);
      tempAtual = (valor - 102) / 2.0;

    }
    
    alarme(false);
    lcd.clear();
    lcd.print("Temperatura OK");
    delay(1000);
    lcd.clear();
  }
}

// CASO SENHA CORRETA
void senhaCorreta() {
  Serial.println("Ordem Correta!");

  contador = 0;
  contTentativas = 0;

  lcd.clear();

  digitalWrite(vermelho, LOW);
  digitalWrite(verde, HIGH);

  lcd.setCursor(0,0);
  lcd.print("Ordem");
  lcd.setCursor(0,1);
  lcd.print("Correta");
  delay(1000);

  digitalWrite(verde, LOW);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Bem Vindo!");

  delay(1000);

  digitalWrite(verde, HIGH);

  abrirFechar(true);

  delay(1000);

}

// BLOQUEAR COFRE
void bloquear(int tentativas) {
  
  if (tentativas == 1) {
    lcd.print("2 tentativas");
    lcd.setCursor(0,1);
    lcd.print("Restantes");
    delay(1000);
    lcd.clear();
  } else if (tentativas == 2) {
    lcd.print("1 tentativa");
    lcd.setCursor(0,1);
    lcd.print("Restante");
    delay(1000);
    lcd.clear();
  } else {
    lcd.print("0 tentativas");
    lcd.setCursor(0,1);
    lcd.print("Restantes");
    delay(1000);
    lcd.clear();
  }
  
  if(tentativas == 3) {
    alarme(true);
  	lcd.setCursor(0,0);
    lcd.print("Cofre");
    lcd.setCursor(0,1);
    lcd.print("Bloqueado");
    delay(1000);
	
    lcd.clear();
    
    lcd.print("Voce pode tentar");
    lcd.setCursor(0,1);
    lcd.print("Novamente em");
    delay(1000);
    
    lcd.clear();
    
    do {
      
      lcd.print(tentativas);
      delay(1000);
      tentativas--;
      lcd.clear();
      
      if(tentativas == 0) {
      	lcd.print("Tente Novamente");
        delay(1000);
        lcd.clear();
      }
      
    } while (tentativas > 0);
    
   	alarme(false);
    
  }
}

// CASO SENHA ERRADA
void senhaErrada() {
  contTentativas++; 
 
  Serial.println("Ordem Incorreta!");
  contador = 0;
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Ordem");
  lcd.setCursor(0,1);
  lcd.print("Incorreta");
  delay(1000);

  if (contTentativas != 3) {
    lcd.setCursor(0,0);
    lcd.print("Tente");
    lcd.setCursor(0,1);
    lcd.print("Novamente");
  }

  delay(1000);
  
  lcd.clear();
 
  bloquear(contTentativas);

}

// VERIFICAR SENHA
void verificar(int qtde) {

  if(qtde == 4) {
  	int acertos = 0;
    Serial.println("Verificando...");
    for(int i = 0; i < 4; i++) {

      if (ordemInserida[i] == ordemCorreta[i]){
        
        acertos++;
        
      }
    }

    if(acertos == 4) {
      
      senhaCorreta();
      
    } else {
      
      senhaErrada();
      
    }
  }
}
