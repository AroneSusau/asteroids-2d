#ifndef H_WALL
#define H_WALL

class Wall {
  public:
    float x1, x2, y1, y2 = 0;
    float colour [3] = { 1.0, 1.0, 1.0 };
    float warning_distance = 0.12;

    void setPoints(float x1, float y1, float x2, float y2) {
      this->x1 = x1;
      this->y1 = y1;
      this->x2 = x2;
      this->y2 = y2;
    };

    void setRed() {
      this->colour[0] = 1.0;
      this->colour[1] = 0.0;
      this->colour[2] = 0.0;
    }

    void  setWhite() {
      this->colour[0] = 1.0;
      this->colour[1] = 1.0;
      this->colour[2] = 1.0;
    }
      
};

#endif