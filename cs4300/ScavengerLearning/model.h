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
      class Object;
class Model
      {
      public:
          Model();
          Model(int gx,int gy,double gz,int base, double charge, double hp, int ax, int ay, double az, bool visited);
          int mgoalx = -1, mgoaly = -1;
          double mgoalz = -1;
        int mbase_num = 0;
        double mcharge_num=0, mhp_num = 0;
        int magentx = 0, magenty = 0;
        double magentz = 0;
        bool visited;
        protected:
//            std::map<std::string, Object *> mObjects;
        private:
      };
   class Object{
       public:
       protected:
//           std::map<std::string, std::string> mObjects;
       private:
   };
  }
}
#endif /* MODEL_H */

