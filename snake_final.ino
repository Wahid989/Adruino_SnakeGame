#include <TFT_eSPI.h>
#define Joystick_X A7 
#define Joystick_Y A6 
#define Joystick_Button 34 
#define maximum_snake_lenght 300 
#define field_width 91 
#define field_height 70 

#define p2s_x(p_x) (10*p_x+5) 
#define p2s_y(p_y) (10*p_y+35)  

TFT_eSPI tft = TFT_eSPI();

int JoystickX;
int JoystickY;

int score = 0; 
int game_status = 0; 

int CursorX;
int CursorY;  

int dx=1,dy=0; 
int schlangen_laenge; 
byte blocked[field_width][field_height]; 
byte position_x[maximum_snake_lenght]; 
byte position_y[maximum_snake_lenght]; 
int head = 0; 
int tail = 0; 
int counter = 5;

int random_food_x; 
int random_food_y; 

int random_generator_for_food_x; 
int random_generator_for_food_y; 

void wellcome_screen()
{
  tft.fillScreen(TFT_WHITE); 
  tft.setTextColor(TFT_BLACK); 
  tft.setTextSize(5); 
  tft.setCursor(135, 30); 
  tft.print("WELCOME"); 
  //tft.drawBitmap(110, 75, wellcome_snake, 92, 56, TFT_GREEN);
  tft.setCursor(0, 150); 
  tft.setTextSize(2);
  tft.print("Get hooked on the classic of Snake!!!"); 
  tft.setCursor(0, 260); 
  tft.setTextSize(3);
  tft.print("Push Joystick to START!");
}

void game_screen()
{
  tft.fillScreen(TFT_BLACK); 
  tft.drawRect(0, 30, 480, 260, TFT_PURPLE); 
  tft.drawRect(1, 31, 478, 258, TFT_PURPLE);
  tft.drawRect(2, 32, 476, 256, TFT_PURPLE);
  tft.drawRect(3, 33, 474, 254, TFT_PURPLE);
  tft.drawRect(4, 34, 472, 252, TFT_PURPLE);
  
  tft.setTextColor(TFT_RED); 
  tft.setTextSize(3); 
  tft.setCursor(65, 4); 
  tft.print("Score:"); 
  tft.print(score);
}

void loser_screen()
{
  tft.fillScreen(TFT_BLACK); 
  tft.setTextColor(TFT_WHITE); 
  tft.setTextSize(3); 
  tft.setCursor(85, 10);
  tft.print("YOU LOST"); 
  //tft.drawBitmap(100, 55, dead_snake, 124, 38, TFT_RED);
  tft.setCursor(45, 110);
  tft.setTextSize(3);
  tft.print("Learn to play"); 
  tft.setCursor(65, 160);
  tft.setTextColor(TFT_RED); 
  tft.print("Score:"); 
  tft.print(score);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE); 
  tft.setCursor(12, 210); 
  tft.print("Play again? Push Joystick"); 
}

void food_generator()
{
    random_generator_for_food_x = random(0,31); 
    random_generator_for_food_y = random(0,20); 
    random_food_x = p2s_x(random_generator_for_food_x); 
    random_food_y = p2s_y(random_generator_for_food_y); 
    tft.drawRect(random_food_x,random_food_y, 10, 10, TFT_RED); 
}

void move_tail()
{    
    if(counter > 0)
    {
      counter-=1;
    }
    else
    {
      tft.drawRect(p2s_x(position_x[tail]), p2s_y(position_y[tail]), 10, 10, TFT_BLACK);
      blocked[position_x[tail]][position_y[tail]] = 0; 
                                                       
      if(tail >= maximum_snake_lenght)
      {
       tail = 0;
      }
    }
}

void print_score()
{
    tft.fillRect(173, 4, 250, 24, TFT_BLACK); 
    tft.setTextColor(TFT_RED); 
    tft.setTextSize(3); 
    tft.setCursor(65, 4); 
    tft.print("Score:"); 
    tft.print(score);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(Joystick_Button, INPUT_PULLUP);
  tft.begin(); 
  tft.setRotation(3); 
}

void loop(void) 
{
      

  int JoystickX = analogRead(Joystick_X);
  int JoystickY = analogRead(Joystick_Y);

  switch(game_status) 
  {
    case 0: // The game has not been started yet.
          wellcome_screen();
          game_status=1;
          break;

    case 1:
      if(digitalRead(Joystick_Button)==LOW) 
      {
        game_screen(); // Game Screen starten
        game_status = 2;
        CursorX = 0; dx=1;
        CursorY = 0; dy=0;
        head=0;
        tail=0;
        counter=4;
        score=0;
        print_score();

        
        for (int i=0;i<31;i++) {
          for (int j=0;j<20;j++) {
            blocked[i][j]=0;
          }
        }
       
        food_generator();
      }
      break;
      
    case 2: // The game has been started!
 
      if ( blocked[CursorX][CursorY]==1)
      { 
        game_status=3;  
      }
      
  
      tft.drawRect(p2s_x(CursorX), p2s_y(CursorY), 10, 10, TFT_GREEN); // Draw Snake 
      blocked[CursorX][CursorY]=1; 
        
      delay(125);

      position_x[head] = CursorX; 
      position_y[head] = CursorY; 
      head++; 
          
      if(head >= maximum_snake_lenght) 
      {
        head=0;
      }

      move_tail();

      CursorX+=dx;
      CursorY+=dy;
      
      if(CursorX == 31) 
      {
        game_status = 3;
      }
      if(CursorX < 0) 
      {
        game_status = 3;
      }
      if(CursorY == -1) 
      {
        game_status = 3;
      }
      if(CursorY == 20) 
      {
        game_status = 3;
      }

      if(random_generator_for_food_x == CursorX && random_generator_for_food_y == CursorY) 
      {
        food_generator(); 
        counter=1; 
        score+=10; 
        print_score(); 
      }
      
      if(JoystickX>900)
      {
        dx=1;dy=0;     
       }   
      if(JoystickX<300)
      {
        dx=-1;dy=0;
      }
      if(JoystickY>900)
      {        
        dx=0;dy=1;
      }
      if(JoystickY<300)
      {
        dx=0;dy=-1;
      } 
      
      break;   

    case 3:
    {
      loser_screen(); 
      game_status=1; 
      break;
    }
    
  }

}
 