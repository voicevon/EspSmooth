# pragma once

template<typename T>
class Base{
   public:
      Base(T someValue){}
      virtual T someFunc() =0;
};

// template<typename T>
// Base<T>::Base(T someValue){}

typedef enum on_off{ON,OFF} on_off_t; 

class Sub : public Base<on_off_t>{
   public:
      Sub():Base(ON){}
      virtual on_off_t someFunc();
};

// Sub::Sub(): Base(ON) {}


