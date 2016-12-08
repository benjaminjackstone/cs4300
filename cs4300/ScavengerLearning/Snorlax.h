#ifndef _SNORLAX_H_
#define _SNORLAX_H_
#include <map>
#include <ai_scavenger.h>
#include <ai_search.h>
#include "cell.h"
#include  "model.h"
#include "problem.h"

namespace bjs
{
  namespace Scavenger
  {
    class Snorlax : public ai::Agent::AgentProgram
    {
    public:
        Snorlax();
      Snorlax(ai::Agent::Options *opts);
      ~Snorlax();
      virtual ai::Agent::Action * Program(const ai::Agent::Percept * percept);
      std::pair<int, int> Coordinates(int x, int y);
      std::map<std::string, std::string> ParseObject(std::string object);
      void PrintQuit(std::string message);
      void GetObjectCounts(std::map<std::string, std::string> parsed_object, std::string val);
      std::string GetObjectValue(const ai::Agent::Percept * percept);
      bool LookNorth(const ai::Agent::Percept * percept);
      bool LookSouth(const ai::Agent::Percept * percept);
      bool LookEast(const ai::Agent::Percept * percept);
      bool LookWest(const ai::Agent::Percept * percept);
      double GetTotals(std::string attr, int origin);
      std::pair<std::string, double> DepositorNot();
      std::vector<double> ProbOriginGivenColorShapeSize(std::string size, std::string color, std::string shape);
      void VisitCell(Cell * c, const ai::Agent::Percept *percept);
      Cell * getCell(std::pair<int, int> pair);
      void parsePercepts(const ai::Agent::Percept * percept, bool recharge);
      double getCharge(){return mCharge_num;};
      double getHitPoints(){return mHp_num;};
      int getX(){return mX;};
      int getY(){return mY;};
      double getZ(){return mZ;};
      int getNX(){return mNX;};
      int getNY(){return mNY;};
      std::map <std::pair<int, int>, Cell *> getMCells(){return mCells;};
      void setCharge(double c){mCharge_num = c;};
      void setHP(double hp){mHp_num = hp;};
      void setX(int x){mX = x;};
      void setY(int y){mX = y;};
      void setZ(double z){mX = z;};
      void setNX(int nx){mX = nx;};
      void setNY(int ny){mX = ny;};
      std::string getNorthInterface(){return mNorthInterface;};
      std::string getSouthInterface(){return mSouthInterface;};
      std::string getEastInterface(){return mEastInterface;};
      std::string getWestInterface(){return mWestInterface;};
      void setNorthInterface(std::string n){mNorthInterface = n;};
      void setSouthInterface(std::string s){mSouthInterface = s;};
      void setEastInterface(std::string e){mEastInterface = e;};
      void setWestInterface(std::string w){mWestInterface = w;};
      Model * getModel(){return mModel;};
      void setModel(Model * m){mModel = m;};
      void setMCells(std::map <std::pair<int, int>, Cell *> cells){mCells = cells;};
    protected:
          Model * mModel;
          std::map <std::pair<int, int>, Cell *> mCells;
        double mCharge_num = 0, mHp_num = 0, mGoalz = 0, mZ = 0;
        int mX = 0, mY = 0, mNX = 0, mNY = 0, mGoalx = 0, mGoaly = 0;   
        int mBase_num = 0,mCell_num = 0;
        std::pair<double,double> mCoordinates;
        std::pair<int, int> mNew_coordinates;
        std::string mNorthInterface;std::string mSouthInterface;std::string mEastInterface;std::string mWestInterface;
        State * mState;
        bool mExhaustionMode = 0;

    private:
    };
    class Action : public ai::Search::Action
    {
    public:
      enum
        {
          /* Movement actions */
          GO_NORTH,
          GO_SOUTH,
          GO_EAST,
          GO_WEST,
          /* Look at the horizon in one direction */
          LOOK,

          /* others */
          RECHARGE,
          PICKUP,  /* Attempt to put an object in the bin */
          DROP,    /* Attempt to remove an object from the bin */
          DEPOSIT, /* Attempt to put an object from the bin to the base */
          EXAMINE, /* Examine the details of object */
          QUIT,    /* Stop playing */
        };
        
      Action();
      Action(const Action &rhs);
      virtual ~Action();
      Action &operator=(const Action &rhs);
      bool operator==(const Action &rhs) const;
      virtual void Display() const;
      bool SetType(int type_in);
      int GetType() const;
        
    protected:
      int type; /* A_* */

    private:
    };
  }
}

#endif /* _SNORLAX_H_ */