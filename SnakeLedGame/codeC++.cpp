#include <LedControl.h>
#include <stdlib.h>
const int DIN = 11 ;
const int CLK = 13 ;
const int CS = 10 ;

const int nb_matrix = 4 ; 

LedControl matrix = LedControl(DIN , CLK , CS , nb_matrix) ;

// gestion du joystick 
#define vertical A0
#define horizon A1
const int button = 6 ;

const int parametre = 10 ;  
int Speed = 500 ;

const int high_lim = 800 ;
const int min_lim = 200 ;



// Structure pour la gestion du fruit 
typedef struct {
  int num_matrice ;
  int row ;
  int column ;
}fruitElement , *Fruit ;

// Structure du snake 
typedef struct Snakepoint {
  int num_matrice ;
  int row ;
  int column ;
  struct Snakepoint *next ;
  struct Snakepoint *previous ;
}Snakepoint , *point ;

typedef struct Snake{
  int taille ;
  point tete ;
  point queue ;
} snakeelement , *snake ;

typedef enum { 
  vers_gauche , vers_droite , vers_haut , vers_bas
}Direction ;


// prepoccession des fonctions 
void eteindre_tout(LedControl matrixLed , const int nb_matrice = nb_matrix);
void lettre_A(LedControl matrixLed , int matrice);
void lettre_E(LedControl matrixLed , int matrice);
void lettre_N(LedControl matrixLed , int matrice);
void lettre_S(LedControl matrixLed , int matrice);
void lettre_K(LedControl matrixLed , int matrice);
void lettre_M(LedControl matrixLed , int matrice);
void lettre_G(LedControl matrixLed , int matrice);
void lettre_C(LedControl matrixLed , int matrice);
void lettre_L(LedControl matrixLed , int matrice);
void lettre_U(LedControl matrixLed , int matrice);
void lettre_O(LedControl matrixLed , int matrice);
void lettre_Y(LedControl matrixLed , int matrice);
void space(LedControl matrixLed , int matrice);
void interro(LedControl matrixLed , int matrice);



void print_snake(snake mysnake, LedControl matrixLed , Fruit fruit);
void add_point_queue(snake mysnake , int matrice = 0 , int row = 5 , int column = 3);
void add_point_tete(snake mysnake, int matrice= 0 , int row = 5 ,int column = 3);
void rearrange(snake mysnake);
void move_snake(snake mysnake , Direction snakeDirection , Fruit fruit);
void delete_point(snake mysnake);


// Fruits Manager
Fruit createFruit();
bool aMangerLeFruit(Fruit fruit , snake mysnake);


snakeelement *create_snake(Fruit fruit);
point create_point(int matrice, int row , int column);

void beginAnimation(LedControl matrixLed);
void endAnimation(LedControl matrixLed , Fruit fruit);

Direction snakeDirection =  vers_bas ;



Fruit myfruit = createFruit() ;
snake mysnake = create_snake(myfruit);
boolean partieEnCour = false ;

void setup() {
  Serial.begin(9600);
  Serial.println("Demarrage...");

  for(int i = 0 ; i <nb_matrix ; i++){
    matrix.shutdown(i , false);
    matrix.setIntensity(i , 14);
  }
  pinMode(vertical , INPUT);
  pinMode(horizon , INPUT);
  pinMode(button , INPUT_PULLUP);

  beginAnimation(matrix);
  partieEnCour = true ;

  delay(1000);

}

void loop() {
  
  if(partieEnCour){ 
  int ver = analogRead(vertical);
  int hor = analogRead(horizon);

  if(high_lim < ver && snakeDirection != vers_haut){
    snakeDirection = vers_bas ;
  }
  else if(ver < min_lim && snakeDirection != vers_bas){
    snakeDirection = vers_haut ;
  }
  else if(high_lim < hor && snakeDirection != vers_droite){
    snakeDirection = vers_gauche ;
  }
  else if(hor < min_lim && snakeDirection != vers_gauche){
    snakeDirection = vers_droite ;
  }

  print_snake(mysnake, matrix, myfruit);
  move_snake(mysnake , snakeDirection , myfruit);


  delay(Speed -mysnake->taille * parametre);
  eteindre_tout(matrix);

  if((mysnake->tete->num_matrice == nb_matrix ) ||(mysnake->tete->num_matrice == -1 ) || 0 > mysnake->tete->row || mysnake->tete->row > 7){
    endAnimation(matrix , mysnake ,myfruit);
    delay(500);
  }  
  }
  else{
    Serial.println("Fin de la partie");
    delay(5000);
  }
}

void beginAnimation(LedControl matrixLed){
  // int s = 0 , n = 1 , a1= 2 , k =3 , e1 = 4 , spa = 5, g = 6 ,a2 = 7, m=8 , e2 = 9 ; 
  const int nb_mots = 13 ;
  Serial.println("Animation en cours");
  for(int i = 0; i < nb_mots ; i++ ){
    space(matrixLed , 0 - i);
    lettre_S(matrixLed , 1 - i);
    lettre_N(matrixLed , 2 - i);
    lettre_A(matrixLed , 3 - i);
    lettre_K(matrixLed , 4 - i);
    lettre_E(matrixLed , 5 - i);
    space(matrixLed , 6 - i);
    lettre_G(matrixLed , 7 - i);
    lettre_A(matrixLed , 8 - i);
    lettre_M(matrixLed , 9 - i);
    lettre_E(matrixLed , 10 - i);
    space(matrixLed , 11 - i);
    space(matrixLed , 12 - i);

    delay(200);

    eteindre_tout(matrixLed);
  }
}

void endAnimation(LedControl matrixLed, snake mysnake , Fruit fruit ){
  // int s = 0 , n = 1 , a1= 2 , k =3 , e1 = 4 , spa = 5, g = 6 ,a2 = 7, m=8 , e2 = 9 ; 
  const int nb_mots = 12 ;
  Serial.println("Animation de fin");
  for(int i = 0; i < nb_mots ; i++ ){
    space(matrixLed , 0 - i);
    lettre_Y(matrixLed , 1 - i);
    lettre_O(matrixLed , 2 - i);
    lettre_U(matrixLed , 3 - i);
    space(matrixLed , 4 - i);
    lettre_L(matrixLed , 5 - i);
    lettre_O(matrixLed , 6 - i);
    lettre_S(matrixLed , 7 - i);
    lettre_E(matrixLed , 8 - i);
    space(matrixLed , 9 - i);
    interro(matrixLed , 10 - i);
    interro(matrixLed , 11 - i);

    delay(200);

    eteindre_tout(matrixLed);
  }

  partieEnCour = false ;

  while(mysnake->tete != nullptr){
    point element = mysnake->tete ;
    mysnake->tete = mysnake->tete->next ;
    free(element);
  }
  free(mysnake);
  free(fruit);
  fruit = nullptr ;
  mysnake = nullptr ;
}




// Lettre de l'alphabet
void lettre_A(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;

  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B00011000);
  matrixLed.setRow(matrice , 2 , B00100100);
  matrixLed.setRow(matrice , 3 , B01111110);
  matrixLed.setRow(matrice , 4 , B11000011);
  matrixLed.setRow(matrice , 5 , B10000001);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_N(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;

  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11100011);
  matrixLed.setRow(matrice , 2 , B11110011);
  matrixLed.setRow(matrice , 3 , B11011011);
  matrixLed.setRow(matrice , 4 , B11001111);
  matrixLed.setRow(matrice , 5 , B11000011);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_C(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;

  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B01111110);
  matrixLed.setRow(matrice , 2 , B11000011);
  matrixLed.setRow(matrice , 3 , B11000000);
  matrixLed.setRow(matrice , 4 , B11000011);
  matrixLed.setRow(matrice , 5 , B01111110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_K(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11000110);
  matrixLed.setRow(matrice , 2 , B11001100);
  matrixLed.setRow(matrice , 3 , B11111000);
  matrixLed.setRow(matrice , 4 , B11001100);
  matrixLed.setRow(matrice , 5 , B11000110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_E(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11111110);
  matrixLed.setRow(matrice , 2 , B11000000);
  matrixLed.setRow(matrice , 3 , B11111110);
  matrixLed.setRow(matrice , 4 , B11000000);
  matrixLed.setRow(matrice , 5 , B11111110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}


void lettre_G(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B01111110);
  matrixLed.setRow(matrice , 2 , B11000000);
  matrixLed.setRow(matrice , 3 , B11001110);
  matrixLed.setRow(matrice , 4 , B11000011);
  matrixLed.setRow(matrice , 5 , B01111110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}


void lettre_S(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B01111110);
  matrixLed.setRow(matrice , 2 , B11000000);
  matrixLed.setRow(matrice , 3 , B01111110);
  matrixLed.setRow(matrice , 4 , B00000011);
  matrixLed.setRow(matrice , 5 , B11111110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}


void lettre_M(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11000011);
  matrixLed.setRow(matrice , 2 , B11111111);
  matrixLed.setRow(matrice , 3 , B11011011);
  matrixLed.setRow(matrice , 4 , B11000011);
  matrixLed.setRow(matrice , 5 , B11000011);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_Y(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11000011);
  matrixLed.setRow(matrice , 2 , B01100110);
  matrixLed.setRow(matrice , 3 , B00111100);
  matrixLed.setRow(matrice , 4 , B00011000);
  matrixLed.setRow(matrice , 5 , B00011000);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_O(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B01111110);
  matrixLed.setRow(matrice , 2 , B10000001);
  matrixLed.setRow(matrice , 3 , B10000001);
  matrixLed.setRow(matrice , 4 , B10000001);
  matrixLed.setRow(matrice , 5 , B01111110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void lettre_U(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11000011);
  matrixLed.setRow(matrice , 2 , B11000011);
  matrixLed.setRow(matrice , 3 , B11000011);
  matrixLed.setRow(matrice , 4 , B11000011);
  matrixLed.setRow(matrice , 5 , B01111110);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}
void lettre_L(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B11000000);
  matrixLed.setRow(matrice , 2 , B11000000);
  matrixLed.setRow(matrice , 3 , B11000000);
  matrixLed.setRow(matrice , 4 , B11000000);
  matrixLed.setRow(matrice , 5 , B11111111);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void interro(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B00011000);
  matrixLed.setRow(matrice , 2 , B00011000);
  matrixLed.setRow(matrice , 3 , B00011000);
  matrixLed.setRow(matrice , 4 , B00011000);
  matrixLed.setRow(matrice , 5 , B00000000);
  matrixLed.setRow(matrice , 6 , B00011000);
  matrixLed.setRow(matrice , 7 , B00000000);
}


void space(LedControl matrixLed , int matrice){
  if(nb_matrix <= matrice || matrice < 0)
      return ;
  matrixLed.setRow(matrice , 0 , B00000000);
  matrixLed.setRow(matrice , 1 , B00000000);
  matrixLed.setRow(matrice , 2 , B00000000);
  matrixLed.setRow(matrice , 3 , B00000000);
  matrixLed.setRow(matrice , 4 , B00000000);
  matrixLed.setRow(matrice , 5 , B00000000);
  matrixLed.setRow(matrice , 6 , B00000000);
  matrixLed.setRow(matrice , 7 , B00000000);
}

void eteindre_tout(LedControl matrixLed , const int nb_matrice){
  for(int i= 0; i< nb_matrice ; i++){
    matrixLed.clearDisplay(i);
    // matrixLed.setRow(i , 0 , B00000000);
    // matrixLed.setRow(i , 1 , B00000000);
    // matrixLed.setRow(i , 2 , B00000000);
    // matrixLed.setRow(i , 3 , B00000000);
    // matrixLed.setRow(i , 4 , B00000000);
    // matrixLed.setRow(i , 5 , B00000000);
    // matrixLed.setRow(i , 6 , B00000000);
    // matrixLed.setRow(i , 7 , B00000000);
  }

}


// ============================================================


point create_point(int matrices, int row , int column){
  point element = malloc(sizeof(Snakepoint));
  (*element).num_matrice = matrices ;
  (*element).row = row ;
  (*element).column = column ;
  (*element).next = nullptr ;
  (*element).previous = nullptr ;
  
  return element ;
} 

void add_point_queue(snake mysnake, int matrice , int row , int column){
  if((*mysnake).taille == 0){
    point element = create_point(matrice , row , column);
    Serial.println(element->num_matrice);
    mysnake->tete = element ;
    mysnake->queue = element ;
  }
  else {
    point element = create_point(matrice , row , column);
    mysnake->queue->next = element ;
    element->previous = mysnake->queue ;
    mysnake->queue = element ;
  }
  Serial.println("point ajouter avec succes");
  mysnake->taille++ ;

}

void add_point_tete(snake mysnake, int matrice , int row , int column){
  if((*mysnake).taille == 0){
    point element = create_point(matrice , row , column);
    Serial.println((*element).num_matrice);
    mysnake->tete = element ;
    mysnake->queue = element ;
  }
  else {
    point element = create_point(matrice , row , column );
    element->next = mysnake->tete ;
    mysnake->tete->previous = element ;
    mysnake->tete = element ;

  }
  // Serial.println("point ajouter avec succes");
  mysnake->taille++ ;

}


void delete_point(snake mysnake){
  if(mysnake->taille == 0){
    Serial.println("rien  retirer car le serpent est vide ou n'existe pas ");
    return ;
  }

  point element = mysnake->queue ;
  mysnake->queue = mysnake->queue->previous ;
  free(element);
  element = nullptr ;
  mysnake->queue->next = nullptr ;
  mysnake->taille--;
}


snakeelement *create_snake(Fruit fruit){

  snakeelement* temporysnake = malloc(sizeof(snakeelement));
  (*temporysnake).taille = 0 ;
  add_point_queue(temporysnake);
  add_point_queue(temporysnake , 0 , 5 , 2);
  add_point_queue(temporysnake , 0 , 5 , 1);
  
  print_snake(temporysnake , matrix , fruit);

  return temporysnake ;
}

void print_snake(snake mysnake, LedControl matrixLed , Fruit fruit){
  if(mysnake == nullptr){
    Serial.println("Pas de snake");
    return ;
  }
  point element = mysnake->tete ;
  Serial.println("Afficher");

  for(int i = 0 ; i < mysnake->taille; i++) {
      // Serial.print(element->num_matrice);
      // Serial.print(element->row);
      // Serial.println(element->column);
      matrixLed.setLed(element->num_matrice , element->row , element->column , true);
      element = element->next ;

  }

  // afficher directement le fruit 
  matrixLed.setLed(fruit->num_matrice , fruit->row , fruit->column , true);

}

//Creation et gestion du fruit 
Fruit createFruit(){
  randomSeed(analogRead(A4));
  Fruit element = malloc(sizeof(fruitElement));

  element->num_matrice = random(nb_matrix);
  element->row = random(8);
  element->column = random(8);

  return element ;
}

bool aMangerLeFruit(Fruit fruit , snake mysnake){
  if(fruit->column == mysnake->tete->column && fruit->row == mysnake->tete->row && fruit->num_matrice == mysnake->tete->num_matrice){
    Fruit tempory = createFruit() ;
    fruit->column = tempory->column ;
    fruit->num_matrice = tempory->num_matrice ;
    fruit->row = tempory->row ;
    
    return true ;
  }
    return false ;
}



void move_snake(snake mysnake , Direction snakeDirection, Fruit fruit){

  point current = mysnake->queue ;
  switch(snakeDirection){
    case vers_gauche :
      if(!aMangerLeFruit(fruit, mysnake)){
        delete_point(mysnake); 
      }
      
      add_point_tete(mysnake , mysnake->tete->num_matrice ,mysnake->tete->row , mysnake->tete->column + 1);
      rearrange(mysnake);
      
      break ;

    case vers_droite :
      if(!aMangerLeFruit(fruit, mysnake)){
        delete_point(mysnake);
        
      }
      add_point_tete(mysnake , mysnake->tete->num_matrice ,mysnake->tete->row , mysnake->tete->column - 1);
      rearrange(mysnake);

      break ;    

    case vers_haut :
      if(!aMangerLeFruit(fruit, mysnake)){
        delete_point(mysnake);
        
    }
      add_point_tete(mysnake , mysnake->tete->num_matrice ,mysnake->tete->row + 1 , mysnake->tete->column);
      rearrange(mysnake);
      
      break ;

    case vers_bas :
      
      add_point_tete(mysnake , mysnake->tete->num_matrice ,mysnake->tete->row - 1 , mysnake->tete->column);
      rearrange(mysnake);
      if(!aMangerLeFruit(fruit, mysnake)){
        delete_point(mysnake);     
      }
      break ;
    
    default :
      break ;
  }

}

void rearrange(snake mysnake){
  if(mysnake->tete->column >= 8 ){
    mysnake->tete->num_matrice = mysnake->tete->num_matrice + 1 ;
    mysnake->tete->column = 0 ;
  }  
  else if(0 > mysnake->tete->column){
    mysnake->tete->column = 7 ;
    mysnake->tete->num_matrice = mysnake->tete->num_matrice - 1 ;
  }else{}
  Serial.println(mysnake->tete->num_matrice);
  
    
  
}
