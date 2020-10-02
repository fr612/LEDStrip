#include <Adafruit_NeoPixel.h>

#define PIN 8
#define STRIPLENGTH 99.0f// Length of strip less 1
#define STRIPLENGTHINT 99// Length of strip less 1
int mode=8;
float brightness=0.6f;
unsigned long time=0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, PIN, NEO_GRB + NEO_KHZ800);
int brightnessCounter=0;

namespace whatever
{
  typedef struct blob_type
  {
    float x;
    float v;
    int color[3];
    unsigned long timeColl;
  }
  blob_type;

  blob_type update(blob_type blob)
  {
    blob.x=blob.x+blob.v/50.0f;

    return blob;
  }

  int findSign(int inputNum)
  {
    int sign;
    if (inputNum>0)
    {
      sign=1;
    }
    else
    {
      sign=-1;
    }
    return(sign);
  }

  blob_type wallColl(blob_type blob)
  {
    if ((blob.x>=STRIPLENGTH)&&(blob.v>0.0f))
    {
      blob.v=-abs(blob.v)*1.3f;
    }
    else if ((blob.x<=1.0f)&&(blob.v<0.0f))
    {
      blob.v=abs(blob.v)*1.3f;
    }

    if (blob.v>6.0f)
    {
      blob.v=6.0f;
    }
    else if (blob.v<-6.0f)
    {
      blob.v=-6.0f;
    }

    return blob;
  }

  // depart or arrive?
  int depOrArrv(blob_type &blob1, blob_type &blob2)
  {
    int dep;
    if(blob1.x>blob2.x)
    {
      if(blob1.v>blob2.v)
      {
        dep=1;
      }
      else
      {
        dep=0;
      }
    }
    else
    {
      if(blob1.v>blob2.v)
      {
        dep=0;
      }
      else
      {
        dep=1;
      }
    }

    return dep;
  }

  // blob colour?
  void blobColl(blob_type &blob1, blob_type &blob2)
  {
    int v1;
    float e;
    float v1Dash;
    float v2Dash;
    if ((abs(blob1.x-blob2.x)<5)&&abs(blob1.x-blob2.x)>0.02)
    {
      if(((millis()-blob1.timeColl)>30)&&((millis()-blob2.timeColl)>30))
      {
        if (depOrArrv(blob1,blob2)==0)
        {
          if (abs(blob1.x-blob2.x)<=3)
          {
            v1=blob1.v;
            e=random(110,125)/100.0f;

            v1Dash=(e*(blob1.v+blob2.v)+sqrt(abs(pow(e,2)*pow((blob1.v+blob2.v),2)-2*e*(e*pow(blob1.v+blob2.v,2)-(pow(blob1.v,2)+pow(blob2.v,2))))))/(2*e);
            v2Dash=blob1.v+blob2.v-v1Dash;

            if ((blob1.x>blob2.x)&&(v1Dash<v2Dash))
            {
              v1Dash=(e*(blob1.v+blob2.v)-sqrt(abs(pow(e,2)*pow((blob1.v+blob2.v),2)-2*e*(e*pow(blob1.v+blob2.v,2)-(pow(blob1.v,2)+pow(blob2.v,2))))))/(2*e);
              v2Dash=blob1.v+blob2.v-v1Dash;
            }
            else if ((blob1.x<blob2.x)&&(v1Dash>v2Dash))
            {
              v1Dash=(e*(blob1.v+blob2.v)-sqrt(abs(pow(e,2)*pow((blob1.v+blob2.v),2)-2*e*(e*pow(blob1.v+blob2.v,2)-(pow(blob1.v,2)+pow(blob2.v,2))))))/(2*e);
              v2Dash=blob1.v+blob2.v-v1Dash;
            }

            blob1.timeColl=millis();
            blob2.timeColl=millis();
            blob1.v=v1Dash;
            blob2.v=v2Dash;

            if (blob1.v>6.0f)
            {
              blob1.v=6.0f;
            }
            else if (blob1.v<-6.0f)
            {
              blob1.v=-6.0f;
            }

            if (blob2.v>6.0f)
            {
              blob2.v=6.0f;
            }
            else if (blob2.v<-6.0f)
            {
              blob2.v=-6.0f;
            }
          }
        }
      }
    }
  }
}


void changeMode()
{
  if ((millis()-time)>200)
  {
    if (mode<8)
    {
      mode++;
    }
    else
    {
      mode=1;
    }

    time=millis();
  }
}

void changeBright()
{
  if ((millis()-time)>200)
  {
    if (brightness<1)
    {
      brightness=brightness+0.2;
    }
    else
    {
      brightness=0.2;
    }
    time=millis();
  }
}

void setup()
{
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(A0,INPUT);
  pinMode(A3,INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5,HIGH);
  //attachInterrupt(0, changeMode, FALLING);
  //attachInterrupt(1, changeBright, FALLING);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(9600);
}

int findSign(int inputNum)
{
  int sign;
  if (inputNum>0)
  {
    sign=1;
  }
  else
  {
    sign=-1;
  }
  return(sign);
}

void loop()
{
  float k=0.0f;
  float delta1;
  float delta2;
  float delta3;
  int j=0;
  float smooth[4];
  whatever::blob_type blob[]={
    {
      3.0f, 1.0f, {
        0, 255, 147}
      , millis()
      }
      ,
    {
      36.0f, -1.3f, {
        255, 155, 47}
      ,millis()
      }
      ,
    {
      25.0f, 3.0f, {
        41, 105, 150}
      ,millis()
      }
      ,
    {
      12.0f, -1.1f, {
        100, 255, 47}
      ,millis()
      }
    };

  float remx;
  int intx;
  float hue;

  // constant colour mode
  if(digitalRead(3)==HIGH)
  {
    Serial.print(analogRead(3));
    brightness=1-analogRead(3)/1023.0f;
    hue=analogRead(0)/1024.0f;

    for (int i=0; i <= STRIPLENGTHINT; i++)
    {
      strip.setPixelColor(i,brightness*(127*cos(hue*6.3)+127), brightness*(127*cos(hue*6.3+4.2)+127), brightness*(127*cos(hue*6.3+8.4)+127));
    }

    strip.show();
  }


  // fun mode
  if(digitalRead(2)==HIGH)
  {
    // rainbow mode
    while (analogRead(0)<90)
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i,brightness*(127*cos(i*6.3/STRIPLENGTHINT+k)+127), brightness*(127*cos(i*6.3/STRIPLENGTHINT+2.1+k)+127), brightness*(127*cos(i*6.3/STRIPLENGTHINT+4.2+k)+127));
      }
      strip.show();

      if (k<=6.3)
      {
        k=k+0.1;
      }
      else
      {
        k=0;
      }
    }

    // white mode
    while ((analogRead(0)>=90)&&(analogRead(0)<150))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i<=STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness*255, brightness*255, brightness*255);
      }
      strip.show();
    }

    // warm white mode
    while ((analogRead(0)>=150)&&(analogRead(0)<210))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i<=STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness*41, brightness*255, brightness*147);
      }
      strip.show();
    }

    // triangle strobe mode
    while ((analogRead(0)>=210)&&(analogRead(0)<341))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i,brightness*(255*k), brightness*(255*k), brightness*(255*k));
      }
      strip.show();

      if (k<=1)
      {
        k=k+0.05;
      }
      else
      {
        k=0;
      }
    }

    // normal strobe mode
    while ((analogRead(0)>=341)&&(analogRead(0)<457))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i<=STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness*255, brightness*255, brightness*255);
      }
      strip.show();
      delay(10);

      for (int i=0; i<=STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness*0, brightness*0, brightness*0);
      }
      strip.show();
      delay(10);
    }

    // constant colour mode
    while ((analogRead(0)>=454)&&(analogRead(0)<568))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i<=STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i, brightness*255, brightness*200, brightness*100);
      }
      strip.show();
      delay(50);
    }

    // overflowing mode
    while ((analogRead(0)>=568)&&(analogRead(0)<682))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i <= STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i,brightness*(127*cos(i*6.3/STRIPLENGTHINT+k)+127)*3, brightness*(127*cos(i*6.3/STRIPLENGTHINT+2.1+k)+127)*3, brightness*(127*cos(i*6.3/STRIPLENGTHINT+4.2+k)+127)*3);
      }
      strip.show();

      if (k<=6.3)
      {
        k=k+0.1;
      }
      else
      {
        k=0;
      }
    }

    // some sort of interference pattern thing
    while ((analogRead(0)>=682)&&(analogRead(0)<796))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i <= STRIPLENGTHINT; i++)
      {
        //delta[1]=(cos(i*6.3f/49.0f*2.0f+k)/5.0f+0.66f+cos(i*6.3f/49.0f*10.0f+2*k)/12.5f+cos(i*6.3f/49.0f*18.2f+2*k)/17.5f);
        delta1=(cos(i*6.3f/STRIPLENGTH*2.0f+k)/5.0f+0.66f);
        delta2=(0.66f+cos(i*6.3f/STRIPLENGTH*10.0f+2*k)/12.5f);
        delta3=(0.66f+cos(i*6.3f/STRIPLENGTH*18.2f+2*k)/17.5f);
        strip.setPixelColor(i,delta1*brightness*41, brightness*255*delta2,brightness*147*delta3);
      }
      strip.show();

      if (k<=6.3f)
      {
        k=k+0.05f+random(0,100)/2000.0f;
      }
      else
      {
        k=0.0f;
      }
    }

    // swimming pool mode
    while ((analogRead(0)>=796)&&(analogRead(0)<909))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i <= STRIPLENGTHINT; i++)
      {
        delta1=(cos(i*6.3f/STRIPLENGTH*2.0f+k)/5.0f+0.8f);
        strip.setPixelColor(i,brightness*delta1*41, brightness*delta1*255,brightness*delta1*147);
      }
      strip.show();

      if (k<=6.3f)
      {
        k=k+0.15f;
      }
      else
      {
        k=0.0f;
      }

      Serial.print(k);
    }

    // blob physics mode
    while ((analogRead(0)>=909))
    {
      brightness=1-analogRead(3)/1023.0f;
      for (int i=0; i<=STRIPLENGTHINT; i++)
      {
        strip.setPixelColor(i,7*brightness,45*brightness,27*brightness);
      }
      for (int i=0; i<4; i++)
      {
        intx=(int)blob[i].x;
        remx=(blob[i].x-intx-0.5)*2;

        smooth[1]=cos((remx-1)*0.7854f);
        smooth[2]=1;
        smooth[3]=cos((remx+1)*0.7854f);

        strip.setPixelColor(intx-1,brightness*blob[i].color[0]*smooth[3],brightness*blob[i].color[1]*smooth[3],brightness*blob[i].color[2]*smooth[3]);
        strip.setPixelColor(intx, brightness*blob[i].color[0]*smooth[2], brightness*blob[i].color[1]*smooth[2], brightness*blob[i].color[2]*smooth[2]);
        strip.setPixelColor(intx+1,brightness*blob[i].color[0]*smooth[1],brightness*blob[i].color[1]*smooth[1],brightness*blob[i].color[2]*smooth[1]);
      }
      strip.show();

      for (int a=0; a<4; a++)
      {
        for (int b=a; b<4; b++)
        {
          whatever::blobColl(blob[a],blob[b]);
        }
      }

      for (int f=0; f<4; f++)
      {
        blob[f]=whatever::wallColl(blob[f]) ;
      }

      for (int p=0; p<4; p++)
      {
        blob[p]=whatever::update(blob[p]);
      }
    }
  }
}
