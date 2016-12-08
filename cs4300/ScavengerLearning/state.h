/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   state.h
 * Author: bstone
 *
 * Created on October 20, 2016, 9:45 AM
 */

#ifndef STATE_H
#define STATE_H

#include <ai_search.h>
#include "cell.h"

namespace bjs
{
  namespace Scavenger
  {
      
        class State : public ai::Search::State
              {
              public:
                State();
              State(const State &rhs);
              virtual ~State();
              State &operator=(const State &rhs);
              bool operator==(const State &rhs) const;
              bool operator<(const State &rhs) const;
              virtual void Display() const;

              virtual bool IsEqual(const ai::Search::State * const state_in) const;

              virtual bool IsLessThan(const ai::Search::State * const state_in) const;

              int GetX() const;
              int GetY() const;
              double GetZ() const {return mZ;};
              double GetHP() const {return mHP;};
              double GetCharge() const;
              double GetCellNum() const {return mCellNum;};
              void SetCell(Cell &cell);
              Cell * GetCell() const;
              bool SetX(const int &x_in);
              bool SetY(const int &y_in);
              bool SetZ(const double &z_in) {mZ = z_in; return true;};
              bool SetCellNum(const int &cell_in) {mCellNum = cell_in; return true;};
              bool SetHP(const double &hp_in) {mHP = hp_in; return true;};
              bool SetCharge(const double &c_in);
              Cell * mCell;

            protected:
                int mX;
                int mY;
                double mZ;
                int mCellNum;
                double mCharge, mHP;
              /* Choose data values appropriate for your problem.
               */

            private:
        };
  }
}
#endif /* STATE_H */

