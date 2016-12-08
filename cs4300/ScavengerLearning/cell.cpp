/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "cell.h"

namespace bjs
{
  namespace Scavenger
  {
      
        Cell::Cell()
        {
      
        }
       Cell::Cell(int cnum, int cx, int cy, double cz, std::string n, std::string s, std::string e, std::string w, bool vn, bool vs, bool ve, bool vw, bool v)
        : mcell_num(cnum), mcell_x(cx), mcell_y(cy), mcell_z(cz), msnorth(n), mssouth(s), mseast(e), mswest(w), north(vn),south(vs),east(ve),west(vw), visited(v)
      {
          //pass
      }
  }
}