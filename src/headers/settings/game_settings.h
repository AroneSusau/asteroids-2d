#ifndef H_GAME_SETTINGS
#define H_GAME_SETTINGS

class arena_t {
  public:
    
    float width = 1500;
    float height = 1500;
    float margin = 0.1;

    void resize(float o_width, float o_height);

};

class game_window_t {
  public:
    
    float width = 0;
    float height = 0;

    void resize(float width, float height);
    
};

#endif