#ifndef BLOBWORLD_HPP
#define BLOBWORLD_HPP

#include <Adafruit_NeoPixel.h>

class BlobWorld 
{
  private:
    typedef struct blob_type
    {
      float x;
      float v;
      int color[3];
      unsigned long timeColl;
    }
    blob_type;

    blob_type blob[4];
    float smooth[4];
    int stripLength;

    blob_type updateBlob(blob_type blob);
    blob_type wallColl(blob_type blob);
    int findSign(int inputNum);
    int depOrArrv(blob_type &blob1, blob_type &blob2);
    void blobColl(blob_type &blob1, blob_type &blob2);

  public:
    BlobWorld(int stripLength);
    
    void update(Adafruit_NeoPixel &strip, float brightness);
};

#endif
