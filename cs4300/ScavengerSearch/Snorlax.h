#ifndef _SNORLAX_H_
#define _SNORLAX_H_
#include <map>
#include <ai_scavenger.h>
#include <ai_search.h>
#include <queue>
#include "cell.h"
#include  "model.h"

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
      Model * mModel;
      std::map <std::pair<double, double>, Cell *> mCells;
      
    protected:
    private:
    };
    class Action : public ai::Search::Action
    {
    public:
      enum
        {
          GO_NORTH,
          GO_SOUTH,
          GO_EAST,
          GO_WEST,
          LOOK,
          QUIT,
          NONE
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