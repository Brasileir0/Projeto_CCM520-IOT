# Locker Inteligente com Feedback Multissensorial

## 📋 Descrição do Projeto
Este projeto consiste em um sistema de controle de acesso seguro (cofre inteligente) desenvolvido para a disciplina **CCM520 - Internet das Coisas** no Centro Universitário FEI. O sistema utiliza uma Máquina de Estados Finita (FSM) para gerenciar a segurança, autenticação e alertas ambientais.

## 🛠️ Hardware e Pinagem
O projeto foi montado utilizando a plataforma Arduino Uno. Abaixo está o mapeamento dos pinos conforme a implementação física:

| Componente | Pino Arduino | Tipo de Sinal | Função |
| :--- | :---: | :---: | :--- |
| **LCD 16x2 (I2C)** | SDA (A4) / SCL (A5) | I2C | Interface visual com o usuário |
| **Servo Motor** | D9 | Saída (PWM) | Mecanismo de trava (0° Trancado / 90° Aberto) |
| **Sensor TMP36** | A0 | Entrada (Analógica) | Monitoramento de temperatura (Segurança) |
| **Buzzer Ativo** | D8 | Saída (Digital) | Alerta sonoro de erro ou perigo |
| **LED Vermelho** | D12 | Saída (Digital) | Indicador de estado: Trancado |
| **LED Verde** | D13 | Saída (Digital) | Indicador de estado: Aberto |
| **Botão 1** | D7 | Entrada (Digital) | Dígito 1 da senha |
| **Botão 2** | D6 | Entrada (Digital) | Dígito 2 da senha |
| **Botão 3** | D5 | Entrada (Digital) | Dígito 3 da senha |
| **Botão de Reset** | D2 | Interrupção (INT0) | Reset de emergência do sistema |

## 🧠 Lógica de Operação (FSM)
O firmware gerencia o fluxo do sistema através dos seguintes estados:

1.  **FECHADO:** Estado inicial. LED Vermelho ligado e servo em 0°. O LCD solicita a senha.
2.  **SENHA:** Captura as entradas dos botões D5, D6 e D7. Exibe `*` no LCD para cada dígito.
3.  **VALIDANDO:** Verifica se a sequência está correta.
    * **Sucesso:** Transita para o estado ABERTO.
    * **Erro:** Incrementa contador. Após 3 erros, ativa o estado ALERTA.
4.  **ABERTO:** LED Verde ligado, servo gira para 90°. O cofre libera o acesso por um tempo determinado.
5.  **ALERTA:** Ativado por erro de senha ou se o sensor TMP36 detectar temperatura crítica (simulação de arrombamento térmico). O buzzer soa continuamente.

## 🚀 Funcionalidades de Destaque
* **Interrupção de Hardware:** Uso do pino D2 (`attachInterrupt`) para garantir que o reset do sistema seja processado instantaneamente, priorizando a segurança.
* **Segurança Multissensorial:** Feedback visual (LCD/LEDs), sonoro (Buzzer) e físico (Servo).
* **Economia de Pinos:** Uso do protocolo I2C para o display, mantendo pinos livres para os sensores e atuadores.

---
**Desenvolvido por:** \
Vinicius Brasileiro Nobre (22.124.013-8) \
Gabriel de Figueiredo Pereira Engle (22.225.014-4)

**Curso:** Ciência da Computação - Centro Universitário FEI (2026)
