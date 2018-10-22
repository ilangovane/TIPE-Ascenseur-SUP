int demande1=0;
int demande2=0;
int demande3=0;
//broches des capteurs infrarouges de chaque étage
const int infra1=22;//étage 1
const int infra2=23;//étage 2
const int infra3=24;//étage 3

//broche des microrupteurs 
const int micro1=32;//étage 1 
const int micro3=34;//étage 3

//étage de destination (à intérieur de la cabine)
const int BP1=36;
const int BP2=38;
const int BP3=40;
//led dans la cabine
const int led1=43;
const int led2=45;
const int led3=47;

//étage de l'appel (à chaque étage)
const int E1=37;
const int E2=39;
const int E3=41;

//microrupteur->ici ouverture et fermeture de la cabine
const int micro_open=26;
const int micro_close=27;

//commande du moteur B: monter ou descendre ou frein ou/et impulsion PWM
const int DIR_B=13;
const int BRAKE_B=8;
const int pwm_b =11;
//commande du moteur A ouverture ou fermeture des portes ou/et impusion PWM
const int DIR_A=12;
const int BRAKE_A=9;
const int pwm_a =10 ;



//led d'appel,présence de l'ascenseur à l'étage ou erreur
const int ledrouge1=48;
const int ledrouge2=49;
const int ledrouge3=42;

const int ledverte1=52;
const int ledverte2=53;
const int ledverte3=46;

const int ledbleu1=50;
const int ledbleu2=51;
const int ledbleu3=44;

//led infraouge situé en  haut de l'ascenseur 
const int led_IR=25;


//niveau logique lu sur le bouton poussoir
const int APPUI=0;
const int PAS_APPUI=1;
//niveau logique lu sur le phototransitor
const int DETECTE=0;
const int PAS_DETECTE=1;
//niveau logique lu sur le microrupteur
const int ACTIONNE = 1;
const int PAS_ACTIONNE = 0;

int vitesseA=255;
int vitesseB=255;
//###concernat LCD Par Endrick############
#include <LiquidCrystal.h>
const int rs=2;
const int enable=3;
const int d4=4;
const int d5=5;
const int d6=6;
const int d7=7;
LiquidCrystal lcd(rs, enable, d4, d5, d6, d7);
//#################
void setup() {
pinMode(infra1,INPUT_PULLUP);
pinMode(infra2,INPUT_PULLUP);
pinMode(infra3,INPUT_PULLUP);

pinMode(micro1,INPUT_PULLUP);

pinMode(micro3,INPUT_PULLUP);

pinMode(BP1,INPUT_PULLUP);
pinMode(BP2,INPUT_PULLUP);
pinMode(BP3,INPUT_PULLUP);

pinMode(E1,INPUT_PULLUP);
pinMode(E2,INPUT_PULLUP);
pinMode(E3,INPUT_PULLUP);

pinMode(micro_close,INPUT_PULLUP);
pinMode(micro_open,INPUT_PULLUP);

pinMode(DIR_A,OUTPUT);
pinMode(DIR_B,OUTPUT);
pinMode(BRAKE_A,OUTPUT);
pinMode(BRAKE_B,OUTPUT);
pinMode(pwm_a,OUTPUT);
pinMode(pwm_b,OUTPUT);

pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);
pinMode(ledrouge1,OUTPUT);
pinMode(ledrouge2,OUTPUT);
pinMode(ledrouge3,OUTPUT);
pinMode(ledverte1,OUTPUT);
pinMode(ledverte2,OUTPUT);
pinMode(ledverte3,OUTPUT);
pinMode(ledbleu1,OUTPUT);
pinMode(ledbleu2,OUTPUT);
pinMode(ledbleu3,OUTPUT);
digitalWrite(BRAKE_B,HIGH);
digitalWrite(BRAKE_A,HIGH);
pinMode(led_IR,OUTPUT);
digitalWrite(led_IR,HIGH);//allimer la led infrarouge sinon on ne peut pas détecter l'ascenseur 
 Serial.begin(9600);
}

void loop() {//itinération sans fin

            appel_ascenseur();//allume les led en bleu   
            destination_ascenseur();//allume les led en bleu->cabine
            presence_ascenseur();//allume led en vert

            
            bool error=erreur();
            while(error){//tant qu'une erreur survient les leds rouge s'allumeront et l'ascenseur restera immobile
              frein_B();
              error=erreur();//nouveau test
            }
            
            /*#######################################CAS OU UN ETAGE EST PRIORITAIRE##############################################
            if(etat_led1()){
              table_de_verite1();//on privilegiera le trajet jusqu'à l'étage1
              }
             else if(etat_led2()){
              table_de_verite2();//on privilegira le trajet jusqu'à l'étage2
              }else if(etat_led3()){
                table_de_verite3();//on privilegiera le trajet jusqu'à l'étage3
                }else{
                  table_de_verite1();//par défaut ->pas inutile car permet de repositionner l'ascenseur au 1er étage si aucune demande
                  }
  ###############################################################################################################################*/
        //on insere la valeur de l'étage dans une des variable demande1 ,demande2 , demande3 pour effectuer dans l'ordre les appels
      if(digitalRead(BP1) == APPUI ||digitalRead(E1) == APPUI ){
              if(demande1 == 0){//si demande 1,2 ou 3 ne contient  rien on lui affecte l'étage 1
              demande1=1;
              }else if(demande2==0 && demande1 != 1){
              demande2=1;
              }else if(demande3==0 && demande2 !=1 && demande1!=1){
               demande3=1; 
              }  
            }
            
      if(digitalRead(BP2) == APPUI ||digitalRead(E2) == APPUI){
              if(demande1 == 0){//si demande 1,2 ou 3 ne contient  rien on lui affecte l'étage 2
              demande1=2;
              }else if(demande2==0 && demande1 != 2){
              demande2=2;
              }else if(demande3==0 && demande2 !=2 && demande1!=2){
               demande3=2; 
              }  
            }
            
              if(digitalRead(BP3) == APPUI ||digitalRead(E3) == APPUI){
                  if(demande1 == 0){//si demande 1,2 ou 3 ne contient  rien on lui affecte l'étage 3
                  demande1=3;
                  }else if(demande2==0 && demande1 != 3){
                  demande2=3;
                  }else if(demande3==0 && demande2 !=3 && demande1!=3){
                   demande3=3; 
                  }
              }
    //on va acquitter les demande lorsques l'asenseur est présent à un étage et est immobile   
      if(digitalRead(ledverte1) == HIGH ){//on supprime la requete où l'ascenseur  va à  l'étage 1 
        if(demande1 == 1){
          demande1 = 0;//on efface la valeur de l'étage car travail de l'ascenseur effectué
          }else if(demande2 == 1){
            demande2=0;
            }else if (demande3 == 1){
              demande3=0;
              }
        }
          if(digitalRead(ledverte2) == HIGH ){//on supprime la requete où l'ascenseur  va à  l'étage 2
        if(demande1 == 2){
          demande1 = 0;//on efface la valeur de l'étage car travail de l'ascenseur effectué
          }else if(demande2 == 2){
            demande2=0;
            }else if (demande3 == 2){
              demande3=0;
              }
        }
          if(digitalRead(ledverte3) == HIGH ){//on supprime la requete où l'ascenseur  va à  l'étage 3
        if(demande1 == 3){
          demande1 = 0;//on efface la valeur de l'étage car travail de l'ascenseur effectué
          }else if(demande2 == 3){
            demande2=0;
            }else if (demande3 == 3){
              demande3=0;
              }
        }
        if(demande1 == 0){
        demande1=demande2;
        demande2=demande3;  
        demande3=0;
        }
  if(demande1==1){
  table_de_verite1();//on privilegiera le trajet jusqu'à l'étage1
  }
 else if(demande1==2){
  table_de_verite2();//on privilegira le trajet jusqu'à l'étage2
  }else if(demande1==3){
    table_de_verite3();//on privilegiera le trajet jusqu'à l'étage3
    }else{
      table_de_verite1();//par défaut ->pas inutile car permet de repositionner l'ascenseur au 1er étage si aucune demande
      }
    
}



//Commande moteurB ->monter ou descendre l'ascenseur
void monter(){
   digitalWrite(BRAKE_B,LOW);//frein débloqué
   digitalWrite(DIR_B,HIGH);//moteur B=>tourne dans le sens B
   analogWrite(pwm_b,vitesseB);//on alimente le moteur B

  }
void descendre(){
   digitalWrite(BRAKE_B,LOW);//frein débloqué
   digitalWrite(DIR_B,LOW);//moteur B=>tourne dans le sens B
   analogWrite(pwm_b,vitesseB);
    }
void frein_B(){
digitalWrite(BRAKE_B,HIGH);
analogWrite(pwm_b,0); //vitesse du moteur B nulle
}
//commandemoteurA->ouvrir et fermer la porte
void ouvrir(){
   digitalWrite(BRAKE_A,LOW);//frein débloqué
   digitalWrite(DIR_A,HIGH);//moteur A=>tourne dans le sens A
   analogWrite(pwm_a,vitesseA);
}  
void fermer(){
   digitalWrite(BRAKE_A,LOW);//frein débloqué
   digitalWrite(DIR_A,LOW);//moteur A=>tourne dans le sens B
   analogWrite(pwm_a,vitesseA);
}  
void frein_A(){
digitalWrite(BRAKE_A,HIGH);//frein 
analogWrite(DIR_A,0); //vitesse du moteur A nulle
}

//
 //allume la led bleu de la led RGB->appel, le passager appel l'ascenseur en appuyant un boutton
  void appel_ascenseur(){
    //lit les états des entrées et sorties en premier temps
bool b1=digitalRead(ledbleu1);
bool b2=digitalRead(ledbleu2);
bool b3=digitalRead(ledbleu3);
bool C1=Capt1();
bool C2=Infra2();
bool C3=Capt3();
bool frein=digitalRead(BRAKE_B);
//état memoire des leds
      if(  (digitalRead(E1) == APPUI || b1 ) && !(C1&& frein) ){  
        
        digitalWrite(ledbleu1,HIGH);//led bleu allumée
        }else{
          digitalWrite(ledbleu1,LOW);//led bleu éteinte
          }
        if(  (digitalRead(E2) == APPUI || b2)&& !(C2&& frein)  )
        {
         digitalWrite(ledbleu2,HIGH); //led bleu allumée
        }else{
          digitalWrite(ledbleu2,LOW);//led bleu éteinte
          }
        if(  (digitalRead(E3) == APPUI || b3) && !(C3&& frein)  )
        {
          digitalWrite(ledbleu3,HIGH);//led bleu allumé
        }else{
        digitalWrite(ledbleu3,LOW);//éteinte
        }  
      
    }
// le passager est dans la cabine et choisit sa destination en appuyant un boutton   
    void destination_ascenseur(){
      //on lit les états des entrées et sorties
bool L1=digitalRead(led1);
bool L2=digitalRead(led2);
bool L3=digitalRead(led3);
bool C1=Capt1();
bool C2=Infra2();
bool C3=Capt3();
bool frein=digitalRead(BRAKE_B);
            if(  (digitalRead(BP1) == APPUI || L1 ) && !(C1&& frein) ){  
              
              digitalWrite(led1,HIGH);//led  allumée
            }else{
              digitalWrite(led1,LOW);//led éteint
              }
            if(  (digitalRead(BP2) == APPUI ||L2 ) && !(C2&& frein) )
            {
             
             digitalWrite(led2,HIGH); //led  allumée
            }else{
              digitalWrite(led2,LOW);//éteint
            }
            if(  (digitalRead(BP3) == APPUI || L3)&& !(C3&& frein) )
            {
              digitalWrite(led3,HIGH);//led  allumée
            }else{
            digitalWrite(led3,LOW);//éteint
            }  
     } 
void presence_ascenseur(){//allume une led verte si présence de l'ascenseur à un étage

  bool C1=Capt1();
  bool C2=Infra2();
  bool C3=Capt3();
  bool frein=digitalRead(BRAKE_B);

            if(C1 && frein ){
            digitalWrite(ledverte1,HIGH);//allume la led verte du 1ere étage

            }else if(C2 && frein){
            digitalWrite(ledverte2,HIGH);//allume la led verte du 2e étage 

             }else if(C3 && frein){
              digitalWrite(ledverte3,HIGH);//allume la led verte du 3e étage
          

              }else{//éteint toutes les led verte
               digitalWrite(ledverte1,LOW);
               digitalWrite(ledverte2,LOW);
               digitalWrite(ledverte3,LOW);
               }
       
} 

/*#############TABLE DE VERITEE#################
ETAPE 1:lire les entrées(leds,capteurs)
ETAPE 2:commander les sorties (moteurs)
#########################*/
bool etat_led1(){//on lit l'état des led dans la cabine et la led bleu de la led RGB à l'exterieur
            if(digitalRead(led1)==HIGH || digitalRead(ledbleu1)==HIGH){
              return true;//appel
              }else{
                return false;//pas d'appel
                }
            }
           
bool etat_led2(){//on lit l'état des led dans la cabine et la led bleu de la led RGB
            if(digitalRead(led2)==HIGH || digitalRead(ledbleu2)==HIGH){
              return true;// appel
              }else{
                return false;//pas d'appel
                }
            }  
bool etat_led3(){
            if(digitalRead(led3)==HIGH || digitalRead(ledbleu3)==HIGH){
              return true;//appel 
              }else{
                return false;//pas d'appel
                }
            }   
  
bool Capt1(){//on lit l'état des capteurs

                          if(digitalRead(infra1) == DETECTE ){
                            return true;//l'ascenseur est détecté à l'étage 1
                            }else{
                              return false;//absense de l'ascenseur à l'étage 1
                              }
                          }
bool Infra2(){//on lit l'état des capteurs

                          if(digitalRead(infra2) == DETECTE ){
                            return true;//l'ascenseur est détecté à l'étage2
                            }else{
                              return false;//absense de l'ascenseur à l'étage2
                              }
                          }  
bool Capt3(){
  

                          if((digitalRead(infra3) == DETECTE && digitalRead(micro3)!= ACTIONNE)||(digitalRead(infra3) != DETECTE && digitalRead(micro3)== ACTIONNE )){
                            return true;//l'ascenseur est détecté à un étage
                            }else{
                              return false;//absense de l'ascenseur à l'étage
                              }
                          }


                          
bool erreur(){//detecte une erreur 
    bool C1=Capt1();
    bool I2=Infra2();
    bool C3=Capt3();  
    if(
    
    (digitalRead(infra3)==DETECTE && digitalRead(micro3)==ACTIONNE)||
   
     (!C1 && I2 && C3)||(C1 && I2 )||(C1 && !I2 && C3)
    ){//allume toute les led rouge + valeur de erreur en booleen
      digitalWrite(ledrouge1,HIGH);
      digitalWrite(ledrouge2,HIGH);
      digitalWrite(ledrouge3,HIGH);
      
            digitalWrite(ledverte1,LOW);
      digitalWrite(ledverte2,LOW);
      digitalWrite(ledverte3,LOW);
      return true;
    }else{//éteint toute les leds rouge
      digitalWrite(ledrouge1,LOW);
      digitalWrite(ledrouge2,LOW);
      digitalWrite(ledrouge3,LOW);
    return false;
    }  
}  

void table_de_verite1(){//commande du moteur sachant que la priorité est : aller au 1er étage
        //on recupère l'état des entrées A->appel de l'ascenseur = led allumé et C->capteur
        bool A1=etat_led1();
        bool A2=etat_led2();
        bool A3=etat_led3();
        bool C1=Capt1();
        bool C2=Infra2();
        bool C3=Capt3();

        if(
        (C1&&!C2&&!C3&&A1)||
        (C1&&!C2&&!C3&&!A1&&!A2&&!A3)||
        (!C1&&!C2&&C3&&!A1&&!A2)||
        (!C1&&C2&&!C3&&!A1&&!A2&&!A3)||
        (!C1&&C2&&!C3&&!A1&&A2&&!A3)
        ){
        frein_B();  
        }else if(
        (A1&&!C1&&!C2) || 
        (A1&&!C1&&C2&&!C3) || 
        (!A1&&A2&&!A3&&!C1&&!C2&&C3) || 
        (!A1&&!A2&&!A3&&!C1&&!C2&&!C3)
        ){
          descendre();
          
          }else if(  (!A1&&!A2&&A3&&!C2&&!C3)  || 
        (!C1&&C2&&!C3&&!A1&&!A2&&A3) || 
        (C1&&!C2&&!C3&&!A1&&A2)  ){
        monter();
        }  
}


void table_de_verite2(){
          //on recupère l'état des entrées A->appel de l'ascenseur = led allumé et C->capteur
        bool A1=etat_led1();
        bool A2=etat_led2();
        bool A3=etat_led3();
        bool C1=Capt1();
        bool C2=Infra2();
        bool C3=Capt3();
        
      if(//condition pour actionner le frein du moteur
      (!A1&&!A2&&!C1&&!C2&&C3)||
      (A2&&!C1&&C2&&!C3)||
      (!C1&&C2&&!C3&&!A1&&!A2&&!A3)||
      (C1&&!C2&&!C3&&!A2&&!A3) 
      ){
        frein_B();
        }else if (
                  (C1&&!C2&&!C3&&A2)||
                 (!C2&&!C3&&!A1&&!A2&&A3) ||
                 (!C1&&C2&&!C3&&!A1&&!A2&&A3)
                 ){  //condition de la montée 
                     monter();
                  }else if (
                  (!A1&&!A2&&!A3&&!C1&&!C2&&!C3)||
                    (!C1&&!C2&&C3&&A2)||
                    (!C1&&!C2&&A1&&!A2&&!A3)||
                    (!C1&&C2&&!C3&&A1&&!A2&&!A3)
                  ){//condition pour descendre
                    descendre();
                    }
      
                  
  }

void table_de_verite3(){
 //on recupère l'état des entrées A->appel de l'ascenseur = led allumé et C->capteur
        bool A1=etat_led1();
        bool A2=etat_led2();
        bool A3=etat_led3();
        bool C1=Capt1();
        bool C2=Infra2();
        bool C3=Capt3();
        
      if//condition pour actionner le frein du moteur
      (
      (C1&&!C2&&!C3&&!A2&&!A3)||
      (!C1&&C2&&!C3&& ((!A1&&!A2&&!A3) ||  (!A1&&A2&&!A3)  )  )||
      (!C1&&!C2&&C3&&!A1&&!A2&&!A3)||
      (!C1&&!C2&&C3&&!A1&&A3)||
      (!C1&&!C2&&C3&&A1&&A3)
      )
      {
        frein_B();
        }else if (
                  (!C2&&!C3&&!A1&&!A2&&A3)||
                  ( !C1&&C2&&!C3&&( (!A1&&A3) || (A1&&A3) ) )
                  ||(!A1&&A2&&C1&&!C2&&!C3) 
                  || (A1&&A3&&C1&&!C2&&!C3)
                 ){  //condition de la montée 
                     monter();
                  }else if (
                  (A1&&!A2&&!A3&&!C1&&!C2) ||
                  (!A1&&!A2&&!A3&&!C1&&!C2&&!C3) || 
                  (!C1&&C2&&!C3&&A1&&!A2&&!A3) ||
                  (!C1&&!C2&&C3&&A2&&!A3)
                  ){//condition pour descendre
                    descendre();
                    }



}  


  

  void porte(){//cette fonction ouvre la porte automatiquement quand l'ascenseur arrive à un étage ou que quand une personne appui sur le bouton de l'ascenseur elle s'ouvre si c'est le bon étage
  bool frein=digitalRead(BRAKE_B);
  if((digitalRead(micro_close)==ACTIONNE && (digitalRead(ledverte1)||digitalRead(ledverte2) || digitalRead(ledverte3) ))
   ||( (digitalRead(E1)|| digitalRead(BP1))&&digitalRead(ledverte1) )
   ||( (digitalRead(E2)|| digitalRead(BP2))&&digitalRead(ledverte2) )
   ||( (digitalRead(E3)|| digitalRead(BP3))&&digitalRead(ledverte3) ) ){
        while(digitalRead(micro_open) != ACTIONNE )
        {
          ouvrir();
                   
            lcd_affichage();//affiche un message sur lcd
            appel_ascenseur();//allume les led en bleu   
            destination_ascenseur();//allume les led en bleu->cabine
            presence_ascenseur();//allume led en vert
        }
        frein_A();
        delay(10000);//pause de 10 s ->temps d'aller dans l'ascenseur et/ou en sortir
        while(digitalRead(micro_close)!=ACTIONNE)
        {
          fermer();
                   
            lcd_affichage();//affiche un message sur lcd
            appel_ascenseur();//allume les led en bleu   
            destination_ascenseur();//allume les led en bleu->cabine
            presence_ascenseur();//allume led en vert
        }
        frein_A();  
    }
  }

void lcd_affichage(){
          //###########"Mettre les variable ici###############
          const int FLECHE_HAUT = 0;// numero du caractere fleche haut
          const int FLECHE_BAS = 1;// numero du caractere fleche bas

        byte flecheHaut[] =
        {B00000000,
         B00000000,
         B00000100,
         B00001110,
         B00011111,
         B00000000,
         B00000000,
         B00000000 };
        lcd.createChar(FLECHE_HAUT, flecheHaut);// creation du caractere fleche haut
        
        byte flecheBas[] =
        {B00000000,
         B00000000,
         B00011111,
         B00001110,
         B00000100,
         B00000000,
         B00000000,
         B00000000 };
        
        lcd.createChar(FLECHE_BAS, flecheBas);// creation du caractere fleche bas


  if(Capt1()){//l'ascenseur est détecter au 1ere étage
     lcd.setCursor(1, 1);
     lcd.write('1');
     

    }else if(Infra2()){//l'ascenseur est détecter au 2e étage
      lcd.setCursor(1, 1);
      lcd.write('2');

      }else if(Capt3()){//l'ascenseur est détecter au 3e étage
     lcd.setCursor(1, 1);
     lcd.write('3');

      }else{//sinon on laisse un espace vide à la colone 1 et ligne 1 de LCD
            lcd.setCursor(1, 1);
     lcd.write(' ');
       } 
       
       if(digitalRead(BRAKE_B)==LOW && digitalRead(pwm_b)==HIGH && digitalRead(DIR_B) == LOW){//l'ascenseur descend
         lcd.setCursor(3, 1);
         lcd.write(byte(1));//affiche la fleche bas

        }else if(digitalRead(BRAKE_B)==LOW && digitalRead(pwm_b)==HIGH && digitalRead(DIR_B)==HIGH){//l'ascenseur monte
          lcd.setCursor(3, 1);
          lcd.write(byte(0));//affiche la fleche bas

          }else if(digitalRead(BRAKE_B)==HIGH){//le frein est enclenché l'ascenseur est immobile
                      lcd.setCursor(3, 1);
                      lcd.write(" ");//on laisse un espace vide à la colone 1 et ligne 1 de LCD
            }
      
  
  }
  


