/**
 * Version: 2023-10-8
 * 
*/


class Ball
{
    int x;
    int y;
    int gravity;

    //Constructor
    public: Ball(int x, int y, int gravity)
    {
        this->x = x;
        this->y = y;
        this->gravity = gravity;
    }


    //Setters
    void setX(int newX)
    {
        x = newX;
    }

    void setY(int newY)
    {
        y = newY;
    }
    
    void setGravity(int newG)
    {
        gravity = newG;
    }

    //getters
    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

    int getGravity()
    {
        return gravity;
    }

};

