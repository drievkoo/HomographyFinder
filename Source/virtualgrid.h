#ifndef VIRTUALGRID
#define VIRTUALGRID

class VirtualGrid{
    int unitInPixels;
public:
    VirtualGrid(){
        unitInPixels=10;
    }

    ~VirtualGrid(){

    }

    int getUnitInPixels(){
        return unitInPixels;
    }

private:
};

#endif // VIRTUALGRID

