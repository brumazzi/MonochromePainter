#ifndef __DOOR_HPP__
#define __DOOR_HPP__

#include <raylib.h>
#include <vector>
#include <string>
#include <object.hpp>

class Door: public Object{
    public:
    Door();

    virtual void draw();
    // virtual void destroy();
    void open();
    void close();
    bool isOpen();

    bool _isOpen;
};

#endif