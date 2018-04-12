/*When a person sees an object, its image remains in the retina of the eye for a time interval of 1/16th of a second.

  This phenomenon is known as persistence of vision.
  This phenomenon is used in the POV Display to form images.
  We turn the LEDs on and off in such a way that the different images overlap each other forming letters.*/

#define D1S0 0
#define D1S1 1
#define D1S2 4
#define D2S0 15
#define D2S1 14
#define D2S2 13
#define D2S3 12
#define left A0
#define right A1
#define clockwi A4
#define anticlock A3
#define pausebutton A5
#define startbutton A6
#define fastforward A2
#define pulse_score 23
#define pulse_reset 22

short int gametime=500;// this variable used for fast forward button .... to store the original falling time of tetrimino
short int score;
short int currentblock, stacksize,TIME=500;// current block used as a variable to give the type of block which is currently falling
short int led[4];// array of tetrimino
short int tempLed[4];// temporary array used as a check condition when some button performs an invalid move
short int stack[128];// entire stack 
short int boundary[8];// boundary is those elements which are just above the stack
short int cycle=0;// cycles of periodicity 4 of I block

short int randomize() {
  short int k = (short int)random(1, 8);
  short int l= (short int)random(0,3);
  //randomSeed(analogRead(2));
  short int a;
  // blocks J,L,S,T,Z will have 0th index as their COM.
  switch (k) {
    case 1: a = (short int)random(-3, 3); //J-BLOCK com 12
      led[0] = 12 + a;
      led[1] = 11 + a;
      led[2] = 3 + a;
      led[3] = 13 + a;
//      {short int count=0;
//      while(count<=l){
//        rotatecw();
//        count++;
//      }
//      }
      break;
    case 2: a = (short int)random(-3, 4); //O-BLOCK
      led[0] = 3 + a;
      led[1] = 11 + a;
      led[2] = 12 + a;
      led[3] = 4 + a;
      break;
    case 3: a = (short int)random(-2, 3); //I-BLOCK
      led[0] = 2 + a; // head
      led[1] = 3 + a;
      led[2] = 4 + a;
      led[3] = 5 + a; //tail
      break;
    case 4: a = (short int)random(-2, 4); //S-BLOCK com 11
      led[0] = 11 + a;
      led[1] = 3 + a;
      led[2] = 10 + a;
      led[3] = 4 + a;
//       {short int count=0;
//      while(count<=l){
//        rotatecw();
//        count++;
//      }
//      }
      break;
    case 5: a = (short int)random(-3, 3); //Z-BLOCK com 12
      led[0] = 12 + a;
      led[1] = 3 + a;
      led[2] = 4 + a;
      led[3] = 13 + a;
//       {short int count=0;
//      while(count<=l){
//        rotatecw();
//        count++;
//      }
//      }
      break;
    case 6: a = (short int)random(-2, 4); //L-BLOCK com 11
      led[0] = 11 + a;
      led[1] = 10 + a;
      led[2] = 12 + a;
      led[3] = 4 + a;
//       {short int count=0;
//      while(count<=l){
//      rotatecw();
//        count++;
//      }
//      }
      break;
    case 7: a = (short int)random(-2, 4); //T-BLOCK com 11
      led[0] = 11 + a;
      led[1] = 3 + a;
      led[2] = 12 + a;
      led[3] = 10 + a;
//       {short int count=0;
//      while(count<=l){
//       rotatecw();
//        count++;
//      }
//      }
      break;
  }
   cycle=0;
  return k;
}
short int rowCompletion(){
  short int row[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (short int i = 0; i < stacksize ; i++){
    row[stack[i] / 8]++;
  }
  for (short int i = 15; i >=0 ; i--){
       if (row[i] == 8)return i;
  }
  return -1;
}
void pause(){
 while(!digitalRead(pausebutton)&&digitalRead(startbutton)){
  for (long int z = millis(); millis() - z <= TIME; ) {
    short int display_led_matrix[128];
    short int count;
    for (count = 0; count <= 3; count++) {
      display_led_matrix[count] = led[count];
    }
    for (count = 4; count <= stacksize + 3; count++) {
      display_led_matrix[count] = stack[count - 4];
    }
    light(count , display_led_matrix);
  }
 } 
}
void light(short int totalcount, short int ledarr[]) {
  short int a[7], count1, count;
  for (count1 = 0; count1 < totalcount; count1++) {
    for (count = 0; count < 7; count++) {
      a[count] = bitRead(ledarr[count1], count);
    }
    digitalWrite(D1S0, a[0]);
    digitalWrite(D1S1, a[1]);
    digitalWrite(D1S2, a[2]);
    digitalWrite(D2S0, a[3]);
    digitalWrite(D2S1, a[4]);
    digitalWrite(D2S2, a[5]);
    digitalWrite(D2S3, a[6]);
   delayMicroseconds(1000000/(totalcount*65));
   }
}
void updateboundary(short int limit) { 
 for(short int i=0; i<8; i++){
        boundary[i]=120+i;
      }
   for (short int col = 0; col <= 7; col++) {
    for (short int row = limit; row <= 15; row++) {
      for (short int i = 0; i < stacksize; i++) {
        if (stack[i] == row * 8 + col){
          boundary[col] = stack[i] - 8;
          row = 16; //used to break from both for loops at once because row =16 is invalid
          break;
        }
      }
    }
  }
}
void meta() {
  short int r = rowCompletion();
  if (r != -1){ 
    eliminateRow(r);
    fall(r);
    meta();
  }
  else{
    updateboundary(0);
  }
}
void startGame(){
  currentblock=randomize();
}
void rotatecwc(short int block[]) {
  short int i;
  short int relative[3];
  for (i = 0; i <= 2; i++) {
    relative[i] = block[i + 1] - block[0];
    switch (relative[i]) {
      case -9: block[i + 1] += 2;
        break;
      case -8: block[i + 1] += 9;
        break;
      case -7: block[i + 1] += 16;
        break;
      case -1: block[i + 1] += -7;
        break;
      case 1: block[i + 1] += 7;
        break;
      case 7: block[i + 1] += -16;
        break;
      case 8: block[i + 1] += -9;
        break;
      case 9: block[i + 1] += -2;
        break;
    }
  }
}
void icwc(short int block[]){
  short int i;
  short int origin;
  switch((cycle)%4){
   case 0: origin=block[0]-8;
           break;
   case 1:origin=block[0]-2;
           break;
   case 2:origin=block[0]-19;
           break;
   case 3:origin=block[0]-25;
           break;
  }
  if(origin<0) return;
  short int relative[4];
  for(i=0;i<=3;i++){
    relative[i]=block[i]-origin;
  }
for(i=0;i<=3;i++){
  switch(relative[i]){
    case 8:block[i]-=6;
          break;
    case 1:block[i]+=10;
          break;
    case 2:block[i]+=17;
          break;
    case 11:block[i]+=15;
          break;
    case 19:block[i]+=6;
          break;
    case 26:block[i]-=10;
          break;
    case 25:block[i]-=17;
          break;
    case 16:block[i]-=15;
          break; 
    case 9:block[i]+=1;
          break;
    case 10:block[i]+=8;
          break;
    case 18:block[i]-=1;
          break;
    case 17:block[i]-=8;
          break;                                                                 
  }
  }
  cycle++;
  if(cycle>3)cycle=0;
}
void ianticwc(short int block[]){
  short int i;
   short int origin;
  switch((cycle)%4){
   case 0: origin=block[0]-8;
           break;
   case 1:origin=block[0]-2;
           break;
   case 2:origin=block[0]-19;
           break;
   case 3:origin=block[0]-25;
           break;
  }
  if(origin<0) return;
  short int relative[4];
  for(i=0;i<=3;i++){
    relative[i]=block[i]-origin;
  }
for(i=0;i<=3;i++){
  switch(relative[i]){
    case 8:block[i]+=17;
          break;
    case 1:block[i]+=15;
          break;
    case 2:block[i]+=6;
          break;
    case 11:block[i]-=10;
          break;
    case 19:block[i]-=17;
          break;
    case 26:block[i]-=15;
          break;
    case 25:block[i]-=6;
          break;
    case 16:block[i]+=10;
          break; 
    case 9:block[i]+=8;
          break;
    case 10:block[i]-=1;
          break;
    case 18:block[i]-=8;
          break;
    case 17:block[i]+=1;
          break;                                                                 
  }
   }
  cycle--;
  if(cycle<0) cycle=3;
}
void rotateanticwc(short int block[]) {
  short int i;
  short int relative[3];
  for (i = 0; i <= 2; i++) {
    relative[i] = block[i + 1] - block[0];
    switch (relative[i]) {
      case -9: block[i + 1] += 16;
        break;
      case -8: block[i + 1] += 7;
        break;
      case -7: block[i + 1] += -2;
        break;
      case -1: block[i + 1] += 9;
        break;
      case 1: block[i + 1] += -9;
        break;
      case 7: block[i + 1] += 2;
        break;
      case 8: block[i + 1] += -7;
        break;
      case 9: block[i + 1] += -16;
        break;
    }
  }
}
void tetris(){
  stacksize=0;
  short int t[7]={0,1,2,9,17,25,33};
  short int e[11]={4,5,6,12,20,21,22,28,36,37,38};
  short int r[12]={4,5,6,14,22,21,20,12,28,36,29,38};
  short int i[9]={0,1,2,9,17,25,33,32,34};
  short int s[11]={4,5,6,12,20,21,22,30,38,37,36};
  short int count=0;
  for(count=0;count<20&&!digitalRead(startbutton);count++){
    if(count<9){stack[count]=i[count];}
    else {stack[count]=s[count-9];}
  }
  stacksize=20;
  for(short int x=0;x<11&&!digitalRead(startbutton);x++){
    disPlay(100);  
  for(count=0;count<20&&!digitalRead(startbutton);count++){
    stack[count]+=8;
  }
  }
  for(count=0;count<19&&!digitalRead(startbutton);count++){
    if(count<7){stack[count+20]=t[count];}
    else {stack[count+20]=r[count-7];}
  }
  stacksize=39;
  for(short int x=0;x<6&&!digitalRead(startbutton);x++){
    disPlay(100);
  for(count=20;count<39&&!digitalRead(startbutton);count++){
    stack[count]+=8;
  }
  }
  for(count=0;count<18&&!digitalRead(startbutton);count++){
    if(count<7){stack[count+39]=t[count];}
    else {stack[count+39]=e[count-7];}
  }
  stacksize=57;
  for(short int x=0;x<1&&!digitalRead(startbutton);x++){
    disPlay(100);
  for(count=39;count<57&&!digitalRead(startbutton);count++){
    stack[count]+=8;
  }
  }
  disPlay(500);  
}
void loop() {
  if(digitalRead(startbutton)){
  for (long int z = millis(); millis() - z <= TIME; ) {
    short int display_led_matrix[128];
    short int count;
    for (count = 0; count <= 3; count++) {
      display_led_matrix[count] = led[count];
    }
    for (count = 4; count <= stacksize + 3; count++) {
      display_led_matrix[count] = stack[count - 4];
    }
    light(count , display_led_matrix);
  }
 if (digitalRead(left) == HIGH) moveLeft();
 if (digitalRead(right) == HIGH) moveRight();
  // turn clockwise and anti clockwise.
 if (digitalRead(clockwi) == HIGH) {rotatecw();}
 if (digitalRead(anticlock) == HIGH)  rotateacw();
 if (digitalRead(fastforward) == HIGH){   
    TIME/=2;                   
    }
 else TIME=gametime;
 if(!digitalRead(pausebutton)&&digitalRead(startbutton)){pause();}
  moveDown();
  addTetrisToStack();
  if(checkGameOver()){
      resetGame();
      gameOverAnimation();
      startGame();
    }
}
else
{
  tetris();
  resetGame();
  if(digitalRead(startbutton))
  {gameOverAnimation();
  resetGame();
  startGame();
  }
}
}
void moveLeft(){
  assignTempLed(led, tempLed);
 for (short int i = 0 ; i < 4 ; i++){
      //move the tetrimino left
      tempLed[i] -= 1;
    }
  if(isInvalid())
    return;
  else{
    assignTempLed(tempLed, led);}
}
void moveRight(){
  assignTempLed(led, tempLed);
 for (short int i = 0 ; i < 4 ; i++){
      //move the tetrimino left
      tempLed[i] += 1;
    }
  if(isInvalid())
    return;
  else{
    assignTempLed(tempLed, led);}
}
void moveDown(){
  assignTempLed(led, tempLed);
  for (short int i = 0 ; i < 4 ; i++){
    //move the tetrimino downwards
    tempLed[i] += 8;
  }
  int flag=0;
  for( short int i=0;i<stacksize; i++){
    for(short int j=0; j<4; j++){
      if(tempLed[j]==stack[i])
        {flag=1;i=stacksize;break;}
    }
  }
  if(flag==1){
    for (short int z = 0; z < 4; z++){
          stack[stacksize] = led[z];
          stacksize++;
        }
        meta();
        currentblock = randomize();
        TIME=gametime;
        }
  else{ 
    assignTempLed(tempLed, led);}
}
void rotatecw(){
  assignTempLed(led, tempLed);
    switch (currentblock) {
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
        rotatecwc(tempLed);
        break;
      case 3:icwc(tempLed);
            //Serial.println("icwc");
            break;
    }
   if(isInvalid())
    return;
  else{ 
    assignTempLed(tempLed, led);}
}
void rotateacw(){
  assignTempLed(led, tempLed);
    switch (currentblock) {
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
        rotateanticwc(tempLed);
        break;
     case 3:ianticwc(tempLed);
             break;
    }
    if(isInvalid())
      return;
    else{ 
    assignTempLed(tempLed, led);}
}
short int addTetrisToStack(){
  short int flag = 0;
   for (short int i = 0 ; i < 4 && flag == 0 ; i++){
    for (short int x = 0; x < 8 && flag == 0; x++) {
      if (led[i] == boundary[x]) {
        for (short int z = 0; z < 4; z++){
          stack[stacksize] = led[z];
          stacksize++;
        }
        meta();
        currentblock = randomize();
        flag = 1;
      }
    }
  }
  if(flag==1){TIME=gametime;}// reset TIME after the block adds to the stack.
  return flag;
}
short int isInvalid(){
  for( short int i=0;i<stacksize; i++){
    for(short int j=0; j<4; j++){
      if(tempLed[j]==stack[i])
        return 1;
    }
  }
  // to check that when any button is pressed the block doesnt enter the existing stack
  for( short int i=0;i<8; i++){
    for(short int j=0; j<4; j++){
      if(tempLed[j]==boundary[i])
        return 1;
    }
  }
  // to check if the tetrimino doesnt enter into wall and reappear from others side
  for(short int j=0; j<4; j++){
     if(abs(tempLed[j]%8-led[j]%8)>=5)
       return 1;
   }
  return 0;
}
void assignTempLed(short int a[], short int b[]){
  for(short int i=0; i<4; i++){
    b[i]=a[i];
  }
  return;
}
void eliminateRow(short int r){ 
  for (short int i = 0; i < stacksize; i++){
    if ((stack[i] / 8) == r)stack[i] = -1;
  }
  pushZerosToEnd(stack, stacksize);
  stacksize -= 8;
  if(score%3==0)
  {
    gametime*=0.75;
   }
  TIME=gametime;
  score++;
  digitalWrite(pulse_score,HIGH);
  delay(100);
  digitalWrite(pulse_score,LOW);
}
void fall(short int row){
  for (short int i = 0; i < stacksize; i++){
    if (stack[i] < (8 * row))stack[i] += 8;
  }
}
void pushZerosToEnd(short int arr[], short int n){
  short int count = 0;  // Count of non-zero elements
  for (short int i = 0; i < n; i++)
    if (arr[i] != -1)
      arr[count++] = arr[i];
  while (count < n)
    arr[count++] = -1;
}
void setup() {
  pinMode(D1S0, OUTPUT);
  pinMode(D1S1, OUTPUT);
  pinMode(D1S2, OUTPUT);
  pinMode(D2S0, OUTPUT);
  pinMode(D2S1, OUTPUT);
  pinMode(D2S2, OUTPUT);
  pinMode(D2S3, OUTPUT);
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  randomSeed(analogRead(1));
  pinMode(clockwi, INPUT);
  pinMode(anticlock, INPUT);
  pinMode(pausebutton ,INPUT);
  pinMode(fastforward,INPUT);
  pinMode(pulse_score,OUTPUT);
  pinMode(pulse_reset,OUTPUT);
  digitalWrite(pulse_score,LOW);
  digitalWrite(pulse_reset,LOW);
  startGame();
  score=0;
  stacksize=0;
  while(!digitalRead(startbutton)){
    tetris();
    if(digitalRead(startbutton))gameOverAnimation();
  }
  resetGame();
  stacksize=0;
}
void disPlay(int t){
   for (long int z = millis(); millis() - z <= t; ) {
    short int display_led_matrix[128];
    short int count;
    for (count = 0; count < stacksize; count++) {
      display_led_matrix[count] = stack[count];
    }
    light(count, display_led_matrix);
  }
}
void resetGame(){
     score=0;
  digitalWrite(pulse_reset,HIGH);
  delay(100);
  digitalWrite(pulse_reset,LOW);
     stacksize=0;
     for (short int i = 0; i < 128; i++)
       stack[i] = -1;
     for(short int i=0; i<8; i++){
        boundary[i]=120+i;
      }
      cycle=0;//cycles for I block
  }
short int checkGameOver(){
  for(short int i=0; i<8; i++){
    if(boundary[i]/8<=0)
      return 1;
  }
  return 0;
}
void gameOverAnimation(){
  for(stacksize=0; stacksize<128; stacksize++){
    stack[stacksize]=(stacksize%2)*7+stacksize*8+(stacksize/16)*pow(-1,stacksize%2);
    if(stacksize%16==0)disPlay(100);
  }
  resetGame();
}
