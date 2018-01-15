class PointNet
{
  public:
    PointNet(float valX, float valY, float valZ);
    PointNet();
    void setX(float value);
    void setY(float value);
    void setZ(float value);

    float getX();
    float getY();
    float getZ();

  private:
    float X;
    float Y;
    float Z;
};