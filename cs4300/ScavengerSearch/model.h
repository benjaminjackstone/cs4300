/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   model.h
 * Author: bstone
 *
 * Created on October 20, 2016, 9:48 AM
 */

#ifndef MODEL_H
#define MODEL_H

namespace bjs
{
  namespace Scavenger
  {
      
class Model
      {
      public:
          Model();
          Model(double gx,double gy,double gz,int base, double charge, double hp, double ax, double ay, double az);
          double mgoalx = -1, mgoaly = -1, mgoalz = -1;
        int mbase_num = 0;
        double mcharge_num=0, mhp_num = 0;
        double magentx = 0, magenty = 0, magentz = 0;
        protected:
        private:
      };
  }
}
#endif /* MODEL_H */

