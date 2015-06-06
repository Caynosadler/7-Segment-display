/* --> Código para desarmar bomba.
 --> Está faltando implementar algumas funções básicas, estão marcadas como //IMPLEMENTAR
 --> link dos estados e da imagem:
-> Estados: http://pastebin.com/idFtvMDz
-> Imagem: http://pixahc.com.br/blog/wp-content/uploads/2015/04/Mapa.png

Coded by: João Carlos Pandolfi Santana (06/06/2015)
*/

#define trig_Pin 13 //Pino 13 envia o pulso para gerar o echo
//Sensores Ultrassom
#define echo_Pin_L 12
#define echo_Pin_M 11
#define echo_Pin_R 10

//Distancias
#define WALL_DISTANCE 10    // 10 centimetros
#define OVER_DISTANCE 1000  // 1 Metro (passou disso, ignora)
#define DOOR_DISTANCE 20    // 20 centimetros (distancia ao lado da porta fechada)
#define COLIDER_DISTANCE 3  // 3 centimetros p evitar colisão

//Estado atual
int actual_state = 0;

//ultimo estado
int last_state = 0;

// tarefa [0 - Desarmar, 1 - Resgatar]
int task = 0;

//matriz de estados - Desarma bomba e preencho os estados (-1, sem estado)
int states_bomb [11][3] = {
  {0, 1, -1},
  {1, 2, -1},
  {2, 3, 4},
  {3, 5, -1},
  {4, 6, -1},
  {5, 7, -1},
  {6, 8, -1},
  {7, 9, -1},
  {8, 10, -1},
  {9, 10, -1},
  {10, -1, -1}
};

//condicoes dos estados
char condition_states_bomb[11] = {'A', 'B', 'C', 'D', 'D', 'G', 'G', 'E', 'F', 'E', 'M'};

//Inicialização do robô
void setup() {
  Serial.begin(9600);
  //Sensores de ultrassom
  pinMode(trig_Pin, OUTPUT);
  pinMode(echo_Pin_L, INPUT);
  pinMode(echo_Pin_M, INPUT);
  pinMode(echo_Pin_R, INPUT);

  //funcoes
  enable_barcode_scan();
}


void loop() {
  if (task == 0) {
    check_change_state();
  }
}

//ativa leitura do cod de barras
void enable_barcode_scan() {
  //IMPLEMENTAR
}

//desativa leitura do cod de barras
void disable_barcode_scan() {
  //IMPLEMENTAR
}

// ====================== MAQUINA DE ESTADOS =======================

//verifica se mudou o estado e toma decisao [UTILIZÁVEL PELO USUARIO]
int check_change_state() {
  int newState = calculate_state_bomb();
  if (newState != actual_state) {
    last_state = actual_state;
    actual_state = newState;
    take_decision_state();
    return 1;
  }
  return 0;
}

//Calculate State
int calculate_state_bomb() {
  //calcula o estado atual
  int newState = actual_state;

  //percorro os possiveis estados
  for (int i = 0; i < 3; i++) {
    //se nao tiver um proximo estado, para loop
    if (states_bomb[actual_state][i] == -1)
      break;

    switch (condition_states_bomb[states_bomb[actual_state][i]]) {
      case 'A':
        if (check_state_A())
          newState = states_bomb[actual_state][i];
        break;

      case 'B':
        if (check_state_B())
          newState = states_bomb[actual_state][i];
        break;

      case 'C':
        if (check_state_C())
          newState = states_bomb[actual_state][i];
        break;

      case 'D':
        if (check_state_D())
          newState = states_bomb[actual_state][i];
        break;

      case 'E':
        if (check_state_E())
          newState = states_bomb[actual_state][i];
        break;

      case 'F':
        if (check_state_F())
          newState = states_bomb[actual_state][i];
        break;

      case 'G':
        if (check_state_G())
          newState = states_bomb[actual_state][i];
        break;
    }
  }

  return newState;
}

//Toma decisão após a troca de estado
void take_decision_state() {
  //para robo para tomada de decisao
  move_robot_back();

  //toma decisao
  if (actual_state == 0) {
    enable_barcode_scan();
    move_robot_front();
  }
  else if (actual_state == 1) {
    disable_barcode_scan();
    move_robot_front();
  }
  else if (actual_state == 2) { //escolhe entre direita e esquerda
    gira(-90); //escolhi girar para Esquerda [ver se coloca random]
    move_robot_front();
  }
  else if (actual_state == 3) {
    move_robot_front();
  }
  else if (actual_state == 4 || actual_state == 6) {
    gira(-90); //viro para esquerda
    move_robot_front();
  }
  else if (actual_state == 5 || actual_state == 7) {
    gira(90); //viro para direita
    move_robot_front();
  }
  else if (actual_state == 8) {
    gira(-90); //gira para esquerda
    //teoricamente neste ponto, está de cara com a bomba
  }
  else if (actual_state == 9) {
    gira(90); //gira para direita
    //teoricamente neste ponto, está de cara com a bomba
  }
  else if (actual_state == 10) {
    //### ESTA DE CARA COM A BOMBA ###
    //ALGORITMO PARA APERTAR O BOTAO
  }
}

// ----------------------- ESTADOS ------------------------

int check_state_A() {
  if (get_distance('M') > DOOR_DISTANCE
      && get_distance('L') <= WALL_DISTANCE
      && get_distance('R') <= WALL_DISTANCE)
    return 1;

  return 0;
}

int check_state_B() {
  if (get_distance('M') >= COLIDER_DISTANCE)
    return 1;

  return 0;
}

int check_state_C() {
  if (get_distance('M') <= COLIDER_DISTANCE
      && get_distance('L') > DOOR_DISTANCE
      && get_distance('R') > DOOR_DISTANCE)
    return 1;

  return 0;
}

int check_state_D() {
  if (get_distance('M') >= COLIDER_DISTANCE
      && get_distance('L') <= WALL_DISTANCE
      && get_distance('R') <= WALL_DISTANCE)
    return 1;

  return 0;
}

int check_state_E() {
  if (get_distance('M') >= COLIDER_DISTANCE
      && get_distance('L') <= WALL_DISTANCE
      && get_distance('R') > DOOR_DISTANCE)
    return 1;

  return 0;
}

int check_state_F() {
  if (get_distance('M') >= COLIDER_DISTANCE
      && get_distance('L') > WALL_DISTANCE
      && get_distance('R') <= WALL_DISTANCE)
    return 1;

  return 0;
}

int check_state_G() {
  if (get_distance('M') <= COLIDER_DISTANCE
      || (get_distance('L') <= WALL_DISTANCE
          && get_distance('R') <= WALL_DISTANCE))
    return 1;

  return 0;
}

// ============== CONTROLE DE MOVIMENTACAO ================

// ------- ROBO ---------

//gira 90 ou -90 graus
void gira(int angle) {
  //IMPLEMENTAR
}

//para robo
void move_robot_stop() {
  stop_motor('L');
  stop_motor('R');
}

//move o robo para frente
void move_robot_front() {
  move_Front('L');
  move_Front('R');
}

//move o robo para tras
void move_robot_back() {
  move_Back('L');
  move_Back('R');
}

// -------- MOTOR ---------

//para o motor
void stop_motor(char direction) {
  //IMPLEMENTAR
}

//move motor para traz
void move_Back(char direction) {
  //IMPLEMENTAR
}

//move motor para frente
void move_Front(char direction) {
  //IMPLEMENTAR
}

// ============== CONTROLE DE SENSORES ======================

int read_barCode() {
  //IMPLEMENTAR
  return 0;
}

//recupera a distancia de acordo com o pino pedido (L,M,R) [Em Cm]
long get_distance(char direction) {
  long duration = 0;
  digitalWrite(trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_Pin, LOW);

  if (direction == 'L')
    duration = pulseIn(echo_Pin_L, HIGH);
  else if (direction == 'R')
    duration = pulseIn(echo_Pin_R, HIGH);
  else
    duration = pulseIn(echo_Pin_M, HIGH);

  return duration / 29 / 2 ;
}
