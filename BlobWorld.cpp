#include "Arduino.h"
#include "BlobWorld.hpp"

BlobWorld::BlobWorld(int stripLength) :
  blob{
    {
      3.0f, 1.0f, {
        0, 255, 147
      }
      , millis()
    }
    ,
    {
      36.0f, -1.3f, {
        255, 155, 47
      }
      , millis()
    }
    ,
    {
      25.0f, 3.0f, {
        41, 105, 150
      }
      , millis()
    }
    ,
    {
      12.0f, -1.1f, {
        100, 255, 47
      }
      , millis()
    }
  },
  smooth{0,0,0,0},
  stripLength(stripLength)
{
  
}

BlobWorld::blob_type BlobWorld::updateBlob(BlobWorld::blob_type blob)
{
  blob.x = blob.x + blob.v / 50.0f;

  return blob;
}

int BlobWorld::findSign(int inputNum)
{
  int sign;
  if (inputNum > 0)
  {
    sign = 1;
  }
  else
  {
    sign = -1;
  }
  return (sign);
}

BlobWorld::blob_type BlobWorld::wallColl(blob_type blob)
{
  if ((blob.x >= stripLength) && (blob.v > 0.0f))
  {
    blob.v = -abs(blob.v) * 1.3f;
  }
  else if ((blob.x <= 1.0f) && (blob.v < 0.0f))
  {
    blob.v = abs(blob.v) * 1.3f;
  }

  if (blob.v > 6.0f)
  {
    blob.v = 6.0f;
  }
  else if (blob.v < -6.0f)
  {
    blob.v = -6.0f;
  }

  return blob;
}

// depart or arrive?
int BlobWorld::depOrArrv(blob_type &blob1, blob_type &blob2)
{
  int dep;
  if (blob1.x > blob2.x)
  {
    if (blob1.v > blob2.v)
    {
      dep = 1;
    }
    else
    {
      dep = 0;
    }
  }
  else
  {
    if (blob1.v > blob2.v)
    {
      dep = 0;
    }
    else
    {
      dep = 1;
    }
  }

  return dep;
}

// blob collisions
void BlobWorld::blobColl(BlobWorld::blob_type &blob1, BlobWorld::blob_type &blob2)
{
  int v1;
  float e;
  float v1Dash;
  float v2Dash;
  if ((abs(blob1.x - blob2.x) < 5) && abs(blob1.x - blob2.x) > 0.02)
  {
    if (((millis() - blob1.timeColl) > 30) && ((millis() - blob2.timeColl) > 30))
    {
      if (depOrArrv(blob1, blob2) == 0)
      {
        if (abs(blob1.x - blob2.x) <= 3)
        {
          v1 = blob1.v;
          e = random(110, 125) / 100.0f;

          v1Dash = (e * (blob1.v + blob2.v) + sqrt(abs(pow(e, 2) * pow((blob1.v + blob2.v), 2) - 2 * e * (e * pow(blob1.v + blob2.v, 2) - (pow(blob1.v, 2) + pow(blob2.v, 2)))))) / (2 * e);
          v2Dash = blob1.v + blob2.v - v1Dash;

          if ((blob1.x > blob2.x) && (v1Dash < v2Dash))
          {
            v1Dash = (e * (blob1.v + blob2.v) - sqrt(abs(pow(e, 2) * pow((blob1.v + blob2.v), 2) - 2 * e * (e * pow(blob1.v + blob2.v, 2) - (pow(blob1.v, 2) + pow(blob2.v, 2)))))) / (2 * e);
            v2Dash = blob1.v + blob2.v - v1Dash;
          }
          else if ((blob1.x < blob2.x) && (v1Dash > v2Dash))
          {
            v1Dash = (e * (blob1.v + blob2.v) - sqrt(abs(pow(e, 2) * pow((blob1.v + blob2.v), 2) - 2 * e * (e * pow(blob1.v + blob2.v, 2) - (pow(blob1.v, 2) + pow(blob2.v, 2)))))) / (2 * e);
            v2Dash = blob1.v + blob2.v - v1Dash;
          }

          blob1.timeColl = millis();
          blob2.timeColl = millis();
          blob1.v = v1Dash;
          blob2.v = v2Dash;

          if (blob1.v > 6.0f)
          {
            blob1.v = 6.0f;
          }
          else if (blob1.v < -6.0f)
          {
            blob1.v = -6.0f;
          }

          if (blob2.v > 6.0f)
          {
            blob2.v = 6.0f;
          }
          else if (blob2.v < -6.0f)
          {
            blob2.v = -6.0f;
          }
        }
      }
    }
  }
}

void BlobWorld::update(Adafruit_NeoPixel &strip, float brightness) 
{
  for (int i = 0; i <= stripLength; i++)
  {
    strip.setPixelColor(i, 7 * brightness, 45 * brightness, 27 * brightness);
  }
  
  for (int i = 0; i < 4; i++)
  {
    int blobX = (int)blob[i].x;
    float remx = (blob[i].x - blobX - 0.5) * 2;

    smooth[1] = cos((remx - 1) * 0.7854f);
    smooth[2] = 1;
    smooth[3] = cos((remx + 1) * 0.7854f);

    strip.setPixelColor(blobX - 1, brightness * blob[i].color[0]*smooth[3], brightness * blob[i].color[1]*smooth[3], brightness * blob[i].color[2]*smooth[3]);
    strip.setPixelColor(blobX, brightness * blob[i].color[0]*smooth[2], brightness * blob[i].color[1]*smooth[2], brightness * blob[i].color[2]*smooth[2]);
    strip.setPixelColor(blobX + 1, brightness * blob[i].color[0]*smooth[1], brightness * blob[i].color[1]*smooth[1], brightness * blob[i].color[2]*smooth[1]);
  }
  
  strip.show();

  for (int a = 0; a < 4; a++)
  {
    for (int b = a; b < 4; b++)
    {
      blobColl(blob[a], blob[b]);
    }
  }

  for (int f = 0; f < 4; f++)
  {
    blob[f] = wallColl(blob[f]);
  }

  for (int p = 0; p < 4; p++)
  {
    blob[p] = updateBlob(blob[p]);
  }
}
