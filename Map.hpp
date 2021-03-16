#ifndef Map_hpp
#define Map_hpp

   const int width = 10;
   const int height = 20;

class map{
    private:
        char level[height][width];
    
    public:
        map ();
        void initializeArray();
};
#endif
