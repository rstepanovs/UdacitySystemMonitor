#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
   Processor();
  float Utilization();

 private:
    float prevWorkTime_;
    float prevIdleTime_;
};

#endif