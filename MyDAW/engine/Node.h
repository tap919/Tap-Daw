#pragma once
struct AudioBlock{ float** in; float** out; int frames; double sr; };
struct Node{ virtual ~Node(){}; virtual void prepare(double,int)=0; virtual void process(const AudioBlock&)=0; virtual int latencySamples() const=0; };
