// BẬC 1: LOW: NGƯỢC          HIGH: THUẬN CHIỀU KIM
// BẬC 2: LOW: NGƯỢC          HIGH: THUẬN CHIỀU KIM
// BẬC 3: LOW: NGƯỢC          HIGH: THUẬN CHIỀU KIM
//Khai bao chan dieu khien cho 3 step driver, shield V3
int en = 8;
int dirPin1 = 5; // dirX
int stepPin1 = 2; // stepX
int dirPin2 = 6; // dirY
int stepPin2 = 3; // stepY
int dirPin3 = 7; // dirZ
int stepPin3 = 4; // stepZ
#define limitX 9 //limitX
#define limitY 10 //limitY
#define limitZ 11 //limitZ
#define hut 13  //nam cham hut

int docX;
int docY;
int docZ;

//Khai bao cac bien hien thi trong qua trinh truyen nhan
int pulseMax = 0;
int pulseMin = 0;

int DemXungMax;
int DemXungMedium;
int DemXungMin;

//So xung
float nStepMax;
float nStepMedium;
float nStepMin;
//Chan co so xung lon nhat, tb, nho nhat
int stepPinMax;
int stepPinMedium;
int stepPinMin;
//Bien hien thi de kiem tra
long nStep1;
long nStep2;
long nStep3;

//Cac bien dl de tinh ra duoc khoang cach, vi du 30 muon xoay qua 45 thi quay them 15 do
float DL_DC1 = 0;
float DL_DC2 = 0;
float DL_DC3 = 0;

float DL_QuaKhu_DC1 = 0;
float DL_QuaKhu_DC2 = 0;
float DL_QuaKhu_DC3 = 0;

float DL_HienTai_DC1 = 0;
float DL_HienTai_DC2 = 0;
float DL_HienTai_DC3 = 0;

String inString = ""; //luu du lieu ben may tinh truyen qua de no nhan lai

int delay1 = 700; //delay so xung cua stepmotor
int delay2 = 700;
int delay3 = 700;

float delay_time = 1;

int DHT = 0;
int DHN = 0;
int Start_mode = 0; //nhan start 1 lan, ko duoc nhan tiep
int Stop_mode = 0;
int Dir3_TN = 0; //bien dieu khien huong dong co 3
int Dir3 = 1;

int Xung_DC1 = 0; //hien thi kiem tra
int Xung_DC2 = 0;
int Xung_DC3 = 0;
//----------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(en, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(limitX, INPUT_PULLUP);
  pinMode(limitY, INPUT_PULLUP);
  pinMode(limitZ, INPUT_PULLUP);
  pinMode(hut, OUTPUT);

  digitalWrite(dirPin1, 0);
  digitalWrite(dirPin2, 0);
  digitalWrite(dirPin3, 0);
  digitalWrite(en, 1); //chua chay thi cho enable=1
  digitalWrite(hut, LOW);
}

void loop()
{
  //docY=digitalRead(limitY);
  //Serial.println(docY);
  //Serial.print(docY);
  //Serial.print(docX);
  while (Serial.available())
  {
    DemXungMax = 0;
    DemXungMedium = 0;
    DemXungMin = 0;
    Tach_chuyen_So(60);
    if (DHN == 1)
    {
      MoveStep123_IK(nStep1, stepPin1, nStep2, stepPin2, nStep3, stepPin3);
    }
    if (DHT == 1)
    {
      MoveStep123_FK(nStep1, stepPin1, nStep2, stepPin2, nStep3, stepPin3);
    }
    //Serial.write("OK");
    
      Serial.print(Xung_DC1);
      Serial.print(" ");
      Serial.print(Xung_DC2);
      Serial.print(" ");
      Serial.println(Xung_DC3);
      Serial.print(stepPinMax);
      Serial.print(" ");
      Serial.print(stepPinMedium);
      Serial.print(" ");
      Serial.println(stepPinMin);

  }
}

//********************************************
//Hàm động học thuận là hàm tạo 1 xung để robot lên xuống
void MoveStep123_FK(long nStep1, int stepPin1, long nStep2, int stepPin2, long nStep3, int stepPin3)
{
  for (int i = 1; i <= nStep1; i = i + 1)
  {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(delay1);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(delay1);
  }
  //////////////////////////////////////////////
  if (Dir3_TN == 0)
  {
    digitalWrite(dirPin3, LOW);
  }
  else
  {
    digitalWrite(dirPin3, HIGH);
  }
  for (int i = 1; i <= nStep2; i = i + 1)
  {
    digitalWrite(stepPin2, HIGH);
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(delay2);
    digitalWrite(stepPin2, LOW);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(delay2);

  }
  /////////////////////////////////////////////////////
  if (Dir3 == 1)
  {
    digitalWrite(dirPin3, LOW); // cua vu la LOW
  }
  else
  {
    digitalWrite(dirPin3, HIGH); // cua vu la HIGH
  }
  for (int i = 1; i <= nStep3; i = i + 1)
  {
    digitalWrite(stepPin3, HIGH);
    delayMicroseconds(delay3);
    digitalWrite(stepPin3, LOW);
    delayMicroseconds(delay3);
  }
}

//*************************************
//Hàm DHN dựa trên sủ dụng điều khiển đồng bộ bởi 1 Arduino với thuật toán như sau:
/* Giả sử truyền vào 3 số xung tương ứng
 *  600 400 200  //Khi vào động học nghịch sẽ sắp xếp lại từ max tới min sau đó chia lấy tỉ lệ
   3  1.5  1      
   1.5 max/medium: Khi max đi được 3 xung thì medium đi 2 xung min đi 1 xung
   => đảm bảo điều kiện đồng bộ
   3   max/min
   //Điều kiện tiên quyết: Khi max xuống mức thấp thì medi, min pải xuống mức thấp 
   để bảo đảm điều kiện đồng bộ
*/
void MoveStep123_IK(long nStep1, int stepPin1, long nStep2, int stepPin2, long nStep3, int stepPin3)
{
  float current_axis_MaxMedium = 0;
  float current_axis_MaxMin = 0;
  long step_axis_MaxMedium = 0;
  long step_axis_MaxMin = 0;
  float Saturation_Max_Medium = 0;
  float Saturation_Max_Min = 0;

  SapXep(nStep1, stepPin1, nStep2, stepPin2, nStep3, stepPin3);

  Saturation_Max_Medium = nStepMax / nStepMedium;
  Saturation_Max_Min = nStepMax / nStepMin;

  // TH1: PinMax = Pin1 ; PinMedium = Pin2 ; PinMin = Pin3   // 120   90   -90
  if ((stepPinMax == stepPin1) && (stepPinMedium == stepPin2) && (stepPinMin == stepPin3))
  {
    Serial.println("TH1");
    for (int i = 1; i <= nStepMax; i = i + 1)
    {
      current_axis_MaxMedium = i / Saturation_Max_Medium;
      current_axis_MaxMin = i / Saturation_Max_Min;

      if (current_axis_MaxMedium - step_axis_MaxMedium >= 1)
      {
        if (Dir3_TN == 0)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        // xung chạy medium
        digitalWrite(stepPinMedium, HIGH);
        // xung bù
        digitalWrite(stepPinMin, HIGH);
        delayMicroseconds(100);
        digitalWrite(stepPinMin, LOW);
        //  tăng xung hiện tại đã chạy của medium
        step_axis_MaxMedium++;
        // đếm xung min và medium đã chạy
        DemXungMedium++;
        DemXungMin++;
      }
      if (current_axis_MaxMin - step_axis_MaxMin >= 1)
      {
        if (Dir3 == 1)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        // xung chạy min
        digitalWrite(stepPinMin, HIGH);
        // tăng xung hiện tại đã chạy của min
        step_axis_MaxMin++;
        DemXungMin++;
      }
      // max luôn có xung
      digitalWrite(stepPinMax, HIGH);
      delay(delay_time);
      digitalWrite(stepPinMax, LOW);

      // xong 1 vòng quét thì low tất cả
      digitalWrite(stepPinMedium, LOW);
      digitalWrite(stepPinMin, LOW);
      DemXungMax++;
    }
    // hiển thị xung để quản lí
    Xung_DC1 = DemXungMax;
    Xung_DC2 = DemXungMedium;
    Xung_DC3 = DemXungMin;
  }

  // TH2: PinMax = Pin3 ; PinMedium = Pin2 ; PinMin = Pin1     //       135 45 30
  else if ((stepPinMax == stepPin3) && (stepPinMedium == stepPin2) && (stepPinMin == stepPin1))
  {
    Serial.println("TH2");
    for (int i = 1; i <= nStepMax; i = i + 1)
    {
      current_axis_MaxMedium = i / Saturation_Max_Medium;
      current_axis_MaxMin = i / Saturation_Max_Min;

      if (current_axis_MaxMedium - step_axis_MaxMedium >= 1)
      {
        if (Dir3_TN == 0)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        digitalWrite(stepPinMedium, HIGH);
        digitalWrite(stepPinMax, HIGH);
        delayMicroseconds(100);
        digitalWrite(stepPinMax, LOW);
        step_axis_MaxMedium++;
        DemXungMedium++;
        DemXungMax++;
      }

      if (current_axis_MaxMin - step_axis_MaxMin >= 1)
      {
        digitalWrite(stepPinMin, HIGH);
        step_axis_MaxMin++;
        DemXungMin++;
      }
      if (Dir3 == 1)
      {
        digitalWrite(dirPin3, LOW);
      }
      else
      {
        digitalWrite(dirPin3, HIGH);
      }
      digitalWrite(stepPinMax, HIGH);
      delay(delay_time);
      digitalWrite(stepPinMax, LOW);
      digitalWrite(stepPinMedium, LOW);
      digitalWrite(stepPinMin, LOW);
      DemXungMax++;
    }
    Xung_DC3 = DemXungMax;
    Xung_DC2 = DemXungMedium;
    Xung_DC1 = DemXungMin;
  }
  // TH3: PinMax = Pin1 ; PinMedium = Pin3 ; PinMin = Pin2    //    120 90  45
  else if ((stepPinMax == stepPin1) && (stepPinMedium == stepPin3) && (stepPinMin == stepPin2))
  {
    Serial.println("TH3");
    for (int i = 1; i <= nStepMax; i = i + 1)
    {
      current_axis_MaxMedium = i / Saturation_Max_Medium;
      current_axis_MaxMin = i / Saturation_Max_Min;

      if (current_axis_MaxMedium - step_axis_MaxMedium >= 1)
      {
        if (Dir3 == 1)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        digitalWrite(stepPinMedium, HIGH);
        delay(delay_time);
        digitalWrite(stepPinMedium, LOW);
        step_axis_MaxMedium++;
        DemXungMedium++;
      }

      if (current_axis_MaxMin - step_axis_MaxMin >= 1)
      {
        if (Dir3_TN == 0)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        digitalWrite(stepPinMin, HIGH);
        digitalWrite(stepPinMedium, HIGH);
        delayMicroseconds(100);
        digitalWrite(stepPinMedium, LOW);
        step_axis_MaxMin++;
        DemXungMin++;
        DemXungMedium++;
      }

      digitalWrite(stepPinMax, HIGH);
      delay(delay_time);
      digitalWrite(stepPinMax, LOW);
      digitalWrite(stepPinMin, LOW);
      DemXungMax++;
    }
    Xung_DC1 = DemXungMax;
    Xung_DC3 = DemXungMedium;
    Xung_DC2 = DemXungMin;
  }
  // TH4: PinMax = Pin3 ; PinMedium = Pin1 ; PinMin = Pin2   //    135  90   45
  else if ((stepPinMax == stepPin3) && (stepPinMedium == stepPin1) && (stepPinMin == stepPin2))
  {
    Serial.println("TH4");
    for (int i = 1; i <= nStepMax; i = i + 1)
    {
      current_axis_MaxMedium = i / Saturation_Max_Medium;
      current_axis_MaxMin = i / Saturation_Max_Min;

      if (current_axis_MaxMedium - step_axis_MaxMedium >= 1)
      {
        digitalWrite(stepPinMedium, HIGH);
        step_axis_MaxMedium++;
        DemXungMedium++;
      }

      if (current_axis_MaxMin - step_axis_MaxMin >= 1)
      {
        if (Dir3_TN == 0)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        digitalWrite(stepPinMin, HIGH);
        digitalWrite(stepPinMax, HIGH);
        delayMicroseconds(100);
        digitalWrite(stepPinMax, LOW);
        step_axis_MaxMin++;
        DemXungMin++;
        DemXungMax++;
      }
      if (Dir3 == 1)
      {
        digitalWrite(dirPin3, LOW);
      }
      else
      {
        digitalWrite(dirPin3, HIGH);
      }
      digitalWrite(stepPinMax, HIGH);
      delay(delay_time);
      digitalWrite(stepPinMax, LOW);
      digitalWrite(stepPinMedium, LOW);
      digitalWrite(stepPinMin, LOW);
      DemXungMax++;
    }
    Xung_DC3 = DemXungMax;
    Xung_DC1 = DemXungMedium;
    Xung_DC2 = DemXungMin;
  }
  // TH5: PinMax = Pin2 ; PinMedium = Pin3 ; PinMin = Pin1   //    45   -45   -90
  else if ((stepPinMax == stepPin2) && (stepPinMedium == stepPin3) && (stepPinMin == stepPin1))
  {
    Serial.println("TH5");
    for (int i = 1; i <= nStepMax; i = i + 1)
    {
      current_axis_MaxMedium = i / Saturation_Max_Medium;
      current_axis_MaxMin = i / Saturation_Max_Min;

      if (current_axis_MaxMedium - step_axis_MaxMedium >= 1)
      {
        if (Dir3 == 1)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        digitalWrite(stepPinMedium, HIGH);
        delayMicroseconds(100);
        digitalWrite(stepPinMedium, LOW);
        step_axis_MaxMedium++;
        DemXungMedium++;
      }

      if (current_axis_MaxMin - step_axis_MaxMin >= 1)
      {
        digitalWrite(stepPinMin, HIGH);
        step_axis_MaxMin++;
        DemXungMin++;
      }
      if (Dir3_TN == 0)
      {
        digitalWrite(dirPin3, LOW);
      }
      else
      {
        digitalWrite(dirPin3, HIGH);
      }
      digitalWrite(stepPinMax, HIGH);
      digitalWrite(stepPinMedium, HIGH);
      delayMicroseconds(100);
      digitalWrite(stepPinMedium, LOW);
      delay(delay_time);
      digitalWrite(stepPinMax, LOW);
      digitalWrite(stepPinMin, LOW);
      DemXungMax++;
      DemXungMedium++;
    }
    Xung_DC2 = DemXungMax;
    Xung_DC3 = DemXungMedium;
    Xung_DC1 = DemXungMin;
  }
  // TH6: PinMax = Pin2 ; PinMedium = Pin1 ; PinMin = Pin3   //     45  -30   -45
  else if ((stepPinMax == stepPin2) && (stepPinMedium == stepPin1) && (stepPinMin == stepPin3))
  {
    Serial.println("TH6");
    for (int i = 1; i <= nStepMax; i = i + 1)
    {
      current_axis_MaxMedium = i / Saturation_Max_Medium;
      current_axis_MaxMin = i / Saturation_Max_Min;

      if (current_axis_MaxMedium - step_axis_MaxMedium >= 1)
      {
        digitalWrite(stepPinMedium, HIGH);
        step_axis_MaxMedium++;
        DemXungMedium++;
      }

      if (current_axis_MaxMin - step_axis_MaxMin >= 1)
      {
        if (Dir3 == 1)
        {
          digitalWrite(dirPin3, LOW);
        }
        else
        {
          digitalWrite(dirPin3, HIGH);
        }
        digitalWrite(stepPinMin, HIGH);
        delay(delay_time);
        digitalWrite(stepPinMin, LOW);
        step_axis_MaxMin++;
        DemXungMin++;
      }
      if (Dir3_TN == 0)
      {
        digitalWrite(dirPin3, LOW);
      }
      else
      {
        digitalWrite(dirPin3, HIGH);
      }
      digitalWrite(stepPinMax, HIGH);
      digitalWrite(stepPinMin, HIGH);
      delayMicroseconds(100);
      digitalWrite(stepPinMin, LOW);
      delay(delay_time);
      digitalWrite(stepPinMax, LOW);
      digitalWrite(stepPinMedium, LOW);
      DemXungMax++;
      DemXungMin++;
    }
    Xung_DC2 = DemXungMax;
    Xung_DC1 = DemXungMedium;
    Xung_DC3 = DemXungMin;
  }
}
//*****************************************************
void SapXep(long nStep1, int stepPin1, long nStep2, int stepPin2, long nStep3, int stepPin3)
{
  nStepMax = nStep1;
  stepPinMax = stepPin1;
  if (nStep2 >= nStep3)
  {
    nStepMedium = nStep2;
    stepPinMedium = stepPin2;
    nStepMin = nStep3;
    stepPinMin = stepPin3;
  }
  else
  {
    nStepMedium = nStep3;
    stepPinMedium = stepPin3;
    nStepMin = nStep2;
    stepPinMin = stepPin2;
  }
  /////////////////////////////////////////////
  if (nStep2 > nStepMax)
  {
    nStepMax = nStep2;
    stepPinMax = stepPin2;
    if (nStep1 > nStep3)
    {
      nStepMedium = nStep1;
      stepPinMedium = stepPin1;
      nStepMin = nStep3;
      stepPinMin = stepPin3;
    }
    else
    {
      nStepMedium = nStep3;
      stepPinMedium = stepPin3;
      nStepMin = nStep1;
      stepPinMin = stepPin1;
    }
  }
  ////////////////////////////////////////////////
  if (nStep3 > nStepMax)
  {
    nStepMax = nStep3;
    stepPinMax = stepPin3;
    if (nStep1 > nStep2)
    {
      nStepMedium = nStep1;
      stepPinMedium = stepPin1;
      nStepMin = nStep2;
      stepPinMin = stepPin2;
    }
    else
    {
      nStepMedium = nStep2;
      stepPinMedium = stepPin2;
      nStepMin = nStep1;
      stepPinMin = stepPin1;
    }
  }

}
//***********************************************
/*Ham tach chuyen so co chuc nang khi du lieu truyen ve co dang chữ cái tiên quyết
dulieu goc 1 + chu A, du lieu goc thu 2 + chu B, du lieu goc thu 3 + chu C
thi khi qua ham tach chuyen so se tach chu so tien quyet ra 1 group, tach mang
32A32B32C thi 32 cho step1, 32 cho step2, 32 cho step3*/

void Tach_chuyen_So(int SoBuoc)
{
  //luu du lieu doc ve tu bien inchar va delay 1 khoang thoi gian
  String inChar = Serial.readString(); //Xem thêm về Serial.read() tại http://arduino.vn/reference/library/serial/1/huong-dan-ham/read
  delayMicroseconds(1);
  //******************** trang thai Start
  if ((inChar[0] == 'S') && (Start_mode == 0))
  {
    digitalWrite(en, 0);
    digitalWrite(dirPin1, 0);
    digitalWrite(dirPin2, 0);
    digitalWrite(dirPin3, 0);
    DHT = 0;
    DHN = 0;
    DL_DC1 = 0;
    DL_DC2 = 0;
    DL_DC3 = 0;
    DL_QuaKhu_DC1 = 0;
    DL_QuaKhu_DC2 = 0;
    DL_QuaKhu_DC3 = 0;
    DL_HienTai_DC1 = 0;
    DL_HienTai_DC2 = 0;
    DL_HienTai_DC3 = 0;
//    for (int x = 10; x < 2600; x++)
//    {
//      digitalWrite(stepPin1, HIGH);
//      delayMicroseconds(delay3);
//      digitalWrite(stepPin1, LOW);
//      delayMicroseconds(delay3);
//    }
//    for (int x = 10; x < 4500; x++)
//    {
//      digitalWrite(stepPin2, HIGH);
//      delayMicroseconds(delay2);
//      digitalWrite(stepPin2, LOW);
//      delayMicroseconds(delay2);
//    }
//    for (int x = 10; x < 2000; x++)
//    {
//      digitalWrite(stepPin3, HIGH);
//      delayMicroseconds(delay3);
//      digitalWrite(stepPin3, LOW);
//      delayMicroseconds(delay3);
//    }
    for(int x = 10; x < 10000; x++) 
    {
      docZ=digitalRead(limitZ);
      if(docZ==0)
      x=10000;
      digitalWrite(stepPin3,HIGH); 
      delayMicroseconds(delay3);
      digitalWrite(stepPin3,LOW); 
      delayMicroseconds(delay3);  
    }   
    for(int x = 10; x < 10000; x++) 
    {
      docY=digitalRead(limitY);
      if(docY==0)
      x=10000;
      digitalWrite(stepPin2,HIGH); 
      delayMicroseconds(delay1); 
      digitalWrite(stepPin2,LOW); 
      delayMicroseconds(delay1); 
    }
    //STEP 1
    for(int x = 10; x < 10000; x++) 
    {
      docX=digitalRead(limitX);
      if(docX==0)
      x=10000;
      digitalWrite(stepPin1,HIGH); 
      delayMicroseconds(delay1);  
      digitalWrite(stepPin1,LOW); 
      delayMicroseconds(delay1);  
    }
    digitalWrite(dirPin1,1);
    for(int x = 0; x < 2900; x++) 
    {
     
      digitalWrite(stepPin1,HIGH); 
      delayMicroseconds(delay1);  
      digitalWrite(stepPin1,LOW); 
      delayMicroseconds(delay1);  
    }
    digitalWrite(dirPin1,0);
    //STEP 2
    digitalWrite(dirPin2,1);
    for(int x = 0; x < 5200; x++) 
    {
     
      digitalWrite(stepPin2,HIGH); 
      delayMicroseconds(delay2);  
      digitalWrite(stepPin2,LOW); 
      delayMicroseconds(delay2);  
    }
    digitalWrite(dirPin2,0);
    // STEP 3
    digitalWrite(dirPin3,1);
    for(int x = 0; x < 3000; x++) 
    {
     
      digitalWrite(stepPin3,HIGH); 
      delayMicroseconds(delay3);  
      digitalWrite(stepPin3,LOW); 
      delayMicroseconds(delay3);  
    }
    digitalWrite(dirPin3,0);

    Serial.println("Robot mode: ON");
    Start_mode++;
    Stop_mode = 0;
  }
  //******************** Trang thai STOP
  else if ((inChar[0] == 'T') && (Stop_mode == 0))
  {
    digitalWrite(en, 1);
    Serial.println("Robot mode: OFF");
    Start_mode = 0;
    Stop_mode++;
    nStep1 = 0;
    nStep2 = 0;
    nStep3 = 0;
    DHT = 0;
    DHN = 0;
    digitalWrite(dirPin1, LOW);
    digitalWrite(dirPin2, LOW);
    digitalWrite(dirPin3, 1);

  }


  // KÍ TỰ FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
  //********************dong hoc thuan
  else if (inChar[0] == 'F')
  {
    DHT = 1;
    DHN = 0;
    inString = ""; //clear chuoi nay di
  }


  // KÍ TỰ IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
  //******************** dong hoc ngich
  else if (inChar[0] == 'I')
  {
    DHT = 0;
    DHN = 1;
    inString = "";
  }

    else if (inChar[0] == 'H')
    {
      DHT = 0;
      DHN = 0;
      digitalWrite(hut, HIGH);
      inString = "";
    }
    else if (inChar[0] == 'L')
    {
      DHT = 0;
      DHN = 0;
      digitalWrite(hut, LOW);
      inString = "";
    }

  //****************** Tach chuyen ki tu thanh so
  for (int x = 1; x < inChar.length(); x++) //phần tử thứ 0 là chữ cái tiên quyết r nên ko xét nữa
  {

    if ((inChar[x] == '-') || (inChar[x] == '.'))
    {
      inString += (char)inChar[x];
    }
    if (isDigit(inChar[x])) { // Hàm kiểm tra số đó có là một ký tự số hay không (xem bảng ACSII http://arduino.vn/reference/bang-ma-ascii )
      // Chuyển đổi số đó thành ký tự và thêm vào chuỗi.
      inString += (char)inChar[x];
    }

    // KÍ TỰ AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    if (inChar[x] == 'A')
    {
      if (DL_QuaKhu_DC1 >= inString.toFloat()) // ÂM        // NGHỊCH
      {
        DL_HienTai_DC1 = inString.toFloat(); //ép về dang số
        DL_DC1 = abs(DL_HienTai_DC1 - DL_QuaKhu_DC1); //tinh toan them truyền bao nhiêu xung nữa
        nStep1 = DL_DC1 / (360.0 / (200.0 * SoBuoc)); //chuyển dữ liệu thành số xung và điều khiển hướng
        digitalWrite(dirPin1, HIGH);
        DL_QuaKhu_DC1 = DL_HienTai_DC1 ;
      }
      else                       // DƯƠNG           // THUẬN
      {
        DL_HienTai_DC1 = inString.toFloat();
        DL_DC1 = abs(DL_HienTai_DC1 - DL_QuaKhu_DC1);
        nStep1 = DL_DC1 / (360.0 / (200.0 * SoBuoc));
        digitalWrite(dirPin1, LOW);
        DL_QuaKhu_DC1 = DL_HienTai_DC1 ;
      }
      inString = ""; //clear dữ liệu thứ nhất
    }

    // KÍ TỰ BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
    else if (inChar[x] == 'B')
    {
      //DL_HienTai_DC2 = inString.toFloat();
      if (DL_QuaKhu_DC2 >= inString.toFloat())   // ÂM      // THUẬN
      {
        DL_HienTai_DC2 = inString.toFloat();
        DL_DC2 = abs(DL_HienTai_DC2 - DL_QuaKhu_DC2);
        nStep2 = DL_DC2 / (360.0 / (200.0 * SoBuoc));
        digitalWrite(dirPin2, HIGH);
        Dir3_TN = 0;
        DL_QuaKhu_DC2 = DL_HienTai_DC2 ;
      }
      else          // DƯƠNG          // NGHỊCH
      {
        DL_HienTai_DC2 = inString.toFloat();
        DL_DC2 = abs(DL_HienTai_DC2 - DL_QuaKhu_DC2);
        nStep2 = DL_DC2 / (360.0 / (200.0 * SoBuoc));
        digitalWrite(dirPin2, LOW);
        Dir3_TN = 1;
        DL_QuaKhu_DC2 = DL_HienTai_DC2 ;
      }
      inString = "";
    }

    // KÍ TỰ CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
    else if (inChar[x] == 'C' ) //30 là số thì lấy, C là ký tự thì ép vô step thứ 3
    {
      if (DL_QuaKhu_DC3 >= inString.toFloat())         // ÂM      //  THUẬN
      {
        DL_HienTai_DC3 = inString.toFloat();
        DL_DC3 = abs(DL_HienTai_DC3 - DL_QuaKhu_DC3);
        nStep3 = DL_DC3 / (360.0 / (200.0 * SoBuoc));
        Dir3 = 0; // cua dien la 1
        digitalWrite(dirPin3, LOW);
        DL_QuaKhu_DC3 = DL_HienTai_DC3 ;
      }
      else            // DƯƠNG        // NGHỊCH
      {
        DL_HienTai_DC3 = inString.toFloat();
        DL_DC3 = abs(DL_HienTai_DC3 - DL_QuaKhu_DC3);
        nStep3 = DL_DC3 / (360.0 / (200.0 * SoBuoc));
        digitalWrite(dirPin3, HIGH);
        Dir3 = 1; // cua dien la 0
        DL_QuaKhu_DC3 = DL_HienTai_DC3 ;
      }
      inString = "";
    }
  }
}
