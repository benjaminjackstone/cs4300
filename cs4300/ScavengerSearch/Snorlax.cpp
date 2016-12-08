#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <queue>
#include "state.h"
#include "model.h"
#include "problem.h"
#include "Snorlax.h"

bool base = true;
bool start = true;
bool gGoal = true;
bool basePath = true;
double gcharge = 0, ghp = 0;
double basex=0,basey=0,basez=0;
int newgoalx=0,newgoaly=0;
int MAXx,MAXy,MINx,MINy;
int gcount = 0;
std::queue<std::string> mPath;
 
namespace bjs
{
  namespace Scavenger
  {
    Snorlax::Snorlax(){}
    Snorlax::Snorlax(ai::Agent::Options *opts)
    {
      SetName("Snorlax");
      std::cout << "The value of the -U option is: " << opts->GetArgInt("user1") << std::endl;
    }

    Snorlax::~Snorlax()
    {
    }
    ai::Agent::Action * Snorlax::Program(const ai::Agent::Percept * percept)
    {
        
        ai::Scavenger::Action *action = new ai::Scavenger::Action;
        if(base && gGoal)
        {
            double cell_x = 0;double cell_y = 0;double cell_z = 0;double charge_num = 0, hp_num = 0;double goalx = 0, goaly = 0, goalz = 0;
            std::string snorth;std::string ssouth;std::string seast;std::string swest;
            State * s = new State();mModel = new Model();Problem * p = new Problem(s, mModel);
            unsigned int i;int base_num = 0,cell_num = 0;double x = 0, y = 0, z = 0;
            
            std::stringstream ss;
            ss.str(percept->GetAtom("BASE").GetValue()); 
            ss.clear();
            ss >> base_num;

            ss.str(percept->GetAtom("CHARGE").GetValue()); 
            ss.clear();
            ss >> charge_num;
            // if I want to find a new goal, i need to comment this out and assigb goalx and y elsewhere
            ss.str(percept->GetAtom("GOAL_LOC").GetValue());
            ss.clear();
            ss >> goalx; ss.ignore();
            ss >> goaly; ss.ignore();
            ss >> goalz; ss.ignore();


            ss.str(percept->GetAtom("HP").GetValue()); 
            ss.clear();
            ss >> hp_num;
            ss.str(percept->GetAtom("X_LOC").GetValue());
            ss.clear();
            ss >> x;
            ss.str(percept->GetAtom("Y_LOC").GetValue()); 
            ss.clear();
            ss >> y;
            ss.str(percept->GetAtom("Z_LOC").GetValue()); 
            ss.clear();
            ss >> z;
            
            if(start)
            {
                start = false;
                basex = x;
                basey = y;
                basez = z;
            
//            std::string directions = {"north", "south", "east", "west"};
              for(i = 0; i < percept->NumAtom(); i++)
                {
                    ai::Agent::PerceptAtom a = percept->GetAtom(i);
                    if(std::strncmp(a.GetName().c_str(), "CELL_", 5) == 0)
                    {
                        std::sscanf(a.GetName().c_str(), "CELL_%d", &cell_num);
                        std::string value = percept->GetAtom(i).GetValue();
                        ss.str(value); 
                        ss.clear();

                        ss >> cell_x; ss.ignore();
                        ss >> cell_y; ss.ignore();
                        ss >> cell_z; ss.ignore();
                        // code to read other parts of position
                        std::getline(ss, snorth, ',');
                        std::getline(ss, ssouth, ',');
                        std::getline(ss, seast, ',');
                        std::getline(ss, swest, ',');
                        // code to read other interface values
                    }
//                    std::cout << goalx << " " << goaly << std::endl;
                    
                    if(cell_x > MAXx) MAXx = cell_x; //leftmost goal
                    if(cell_y > MAXy) MAXy = cell_y; // bottom most goal
                    if(cell_x < MINx) MINx = cell_x; //leftmost goal
                    if(cell_y < MINy) MINy = cell_y; // bottom most goal
                    newgoalx = -5000;
                    newgoaly = 0;        
//                    if(cell_x > goalx) goalx = cell_x; // rightmost goal
//                    if(cell_y > goaly) goaly = cell_y; // topmost goal
                    
                    std::pair<double, double> coordinates;
                    coordinates.first = cell_x;
                    coordinates.second = cell_y;
                    Cell * c =  new Cell(cell_num, cell_x, cell_y, cell_z, snorth, ssouth, seast, swest);
                    mCells[coordinates] = c;
                }
            }
//            std::map<std::pair<double, double>, Cell*>::const_iterator it;
//          for (it = mCells.begin(); it != mCells.end(); it++)
//          {
//              std::cout << "contents: " << it->first.first << " " << it->first.second << " " << it->second->mcell_x << " " << it->second->mcell_y <<std::endl;
//          }
//                mModel = new Model(goalx, goaly, goalz, base_num, charge_num, hp_num, x, y, z);
                mModel = new Model(newgoalx, newgoaly, 0, base_num, charge_num, hp_num, x, y, z);
                s->SetX(x);
                s->SetY(y);
                s->SetZ(z);
                s->SetCharge(charge_num);
                s->SetCellNum(cell_num);
                s->SetHP(hp_num);
                //std::cout << "How many times is Program called? " << mCells.size() << " then the north? " << mCells.begin()->second->msnorth << std::endl;
                std::pair<double, double> pair;
                pair.first = x;
                pair.second = y;
                Cell * startcell = mCells[pair];
                s->SetCell(*startcell);
                //
                p = new Problem(s, mModel);
                State * goal = new State();
                 goal->SetX(newgoalx);
                 goal->SetY(newgoaly);
//                 goal->SetX(goalx);
//                 goal->SetY(goaly);
                p->SetGoal(goal);
                p->mCells = mCells;
            ai::Search::Frontier *fringe  = new ai::Search::AStarFrontier;
            ai::Search::Graph *search = new ai::Search::Graph(p, fringe);
            if (mPath.empty())
            {   
                if(search->Search())
                {
                    std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
                    std::list<ai::Search::Node *>::const_iterator it;
                    for(it = solution->begin(); it != solution->end(); it++)
                    {
                        if((*it)->GetAction())
                        {
                            (*it)->GetAction()->Display();
                        }
//                        if((*it)->GetState())
//                        {
//                            (*it)->GetState()->Display();
//                        }
                    }
//                    std::cout << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
//                    std::cout << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
//                    std::cout << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
                    base = false;
                }
                else{
                    while(!search->Search()){
                        if(gcount > 50){
                            newgoalx = goalx;
                            newgoaly = goaly;
                        }
                        std::cout << "DID NOT FIND A PATH....TRYING A NEW COORDINATE" << std::endl;
                        gcount++;
                        newgoalx -= 1000;
                        newgoaly -= 1000;
//                        if(newgoalx < 0) newgoalx += 1000;
//                        if(newgoaly < 0) newgoaly += 1000;
                        mModel = new Model(newgoalx, newgoaly, 0, base_num, charge_num, hp_num, x, y, z);
                        s->SetX(x);
                        s->SetY(y);
                        s->SetZ(z);
                        s->SetCharge(charge_num);
                        s->SetCellNum(cell_num);
                        s->SetHP(hp_num);
                        //std::cout << "How many times is Program called? " << mCells.size() << " then the north? " << mCells.begin()->second->msnorth << std::endl;
                        std::pair<double, double> pair;
                        pair.first = x;
                        pair.second = y;
                        Cell * startcell = mCells[pair];
                        s->SetCell(*startcell);
                        //
                        p = new Problem(s, mModel);
                        State * goal = new State();
                         goal->SetX(newgoalx);
                         goal->SetY(newgoaly);
        //                 goal->SetX(goalx);
        //                 goal->SetY(goaly);
                        p->SetGoal(goal);
                        p->mCells = mCells;
                        ai::Search::Frontier *fringe  = new ai::Search::AStarFrontier;
                        search = new ai::Search::Graph(p, fringe);
                        if(search->Search())
                        {
                            std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
                            std::list<ai::Search::Node *>::const_iterator it;
                            for(it = solution->begin(); it != solution->end(); it++)
                            {
                                if((*it)->GetAction())
                                {
                                    (*it)->GetAction()->Display();
                                }
//                                if((*it)->GetState())
//                                {
//                                    (*it)->GetState()->Display();
//                                }
                            }
//                            std::cout << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
//                            std::cout << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
//                            std::cout << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
                            base = false;
                        }
                    }
//                    newgoalx = goalx;
//                    newgoaly = goaly;
//                    return action;
                }
            }
        }
        if(!mPath.empty() && basePath) 
        {
            std::string act = mPath.front();
            mPath.pop();
            if(act == "GO_NORTH")
            {
                action->SetCode(ai::Scavenger::Action::GO_NORTH);
            }
            else if(act == "GO_SOUTH")
            {
                action->SetCode(ai::Scavenger::Action::GO_SOUTH);
            }
            else if(act == "GO_WEST")
            {
                action->SetCode(ai::Scavenger::Action::GO_WEST);
            }
            else if(act == "GO_EAST")
            {
                action->SetCode(ai::Scavenger::Action::GO_EAST);
            }
            else if(act == "LOOK")
            {
                action->SetCode(ai::Scavenger::Action::LOOK);
            }
            else if(act == "QUIT")
            {
                action->SetCode(ai::Scavenger::Action::QUIT);
            }
//            action->SetCode(ai::Scavenger::Action::QUIT);
            return action;
        }
        basePath = false;
        //gGoal = true;
        if (!base && gGoal)
        {
            State * s = new State();mModel = new Model();Problem * p = new Problem(s, mModel);
            int base_num = 0;double charge_num = 0, hp_num = 0;double x = 0, y = 0, z = 0;
            
            std::stringstream ss;
            ss.str(percept->GetAtom("BASE").GetValue()); 
            ss.clear();
            ss >> base_num;

            ss.str(percept->GetAtom("CHARGE").GetValue()); 
            ss.clear();
            ss >> charge_num;
            
            ss.str(percept->GetAtom("HP").GetValue()); 
            ss.clear();
            ss >> hp_num;
            
            ss.str(percept->GetAtom("X_LOC").GetValue());
            ss.clear();
            ss >> x;
            ss.str(percept->GetAtom("Y_LOC").GetValue()); 
            ss.clear();
            ss >> y;
            ss.str(percept->GetAtom("Z_LOC").GetValue()); 
            ss.clear();
            ss >> z;
            
                mModel = new Model(basex, basey, basez, base_num, charge_num, hp_num, x, y, z);
                s->SetX(x);
                s->SetY(y);
                s->SetZ(z);
                s->SetCharge(charge_num);
                s->SetHP(hp_num);
                std::pair<double, double> pair;
                pair.first = x;
                pair.second = y;
                Cell * endcell = mCells[pair];
                s->SetCell(*endcell);
                p = new Problem(s, mModel);
                State * goal = new State();
                 goal->SetX(basex);
                 goal->SetY(basey);
                p->SetGoal(goal);
                p->mCells = mCells;
                
//            ai::Search::Frontier *fringe  = new ai::Search::BFFrontier;
//            ai::Search::Frontier *fringe  = new ai::Search::DFFrontier;
            ai::Search::Frontier *fringe  = new ai::Search::AStarFrontier;
            ai::Search::Graph *search = new ai::Search::Graph(p, fringe);
//            ai::Search::Tree *search = new ai::Search::Tree(p, fringe);
            if (mPath.empty())
            {   
                if(search->Search())
                {
                    std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
                    std::list<ai::Search::Node *>::const_iterator it;
                    for(it = solution->begin(); it != solution->end(); it++)
                    {
                        if((*it)->GetAction())
                        {
                            (*it)->GetAction()->Display();
                        }
//                        if((*it)->GetState())
//                        {
//                            (*it)->GetState()->Display();
//                        }
                    }
//                    std::cout << "Path Cost: " << solution->back()->GetPathCost() << std::endl;
//                    std::cout << "Nodes generated: " << search->GetNumberNodesGenerated() << std::endl;
//                    std::cout << "Nodes stored:    " << search->GetMaxNodesStored() << std::endl;
                    gGoal = false;
                }
            }
        }
        //std::cout << mPath.size() << " = queue size" << std::endl;
        if(!mPath.empty())
        {
            std::string act = mPath.front();
            mPath.pop();
            if(act == "GO_NORTH")
            {
                action->SetCode(ai::Scavenger::Action::GO_NORTH);
            }
            else if(act == "GO_SOUTH")
            {
                action->SetCode(ai::Scavenger::Action::GO_SOUTH);
            }
            else if(act == "GO_WEST")
            {
                action->SetCode(ai::Scavenger::Action::GO_WEST);
            }
            else if(act == "GO_EAST")
            {
                action->SetCode(ai::Scavenger::Action::GO_EAST);
            }
            else if(act == "LOOK")
            {
                action->SetCode(ai::Scavenger::Action::LOOK);
            }
            else if(act == "QUIT")
            {
                action->SetCode(ai::Scavenger::Action::QUIT);
            }
            return action;
        }
        action->SetCode(ai::Scavenger::Action::QUIT);
        return action;
    }
    
    
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    
    Action::Action()
    {
      /* empty */
    }
      
    Action::Action(const Action &rhs)
    {
      *this = rhs;
    }
      
    Action::~Action()
    {
      /* empty */
    }
      
    Action &Action::operator=(const Action &rhs)
    {
      this->type   = rhs.type;
      return *this;
    }
      
    bool Action::operator==(const Action &rhs) const
    {
      return (this->type == rhs.type);
    }
    void Action::Display() const
    {
        std::string act;
      switch(this->type)
        {
        case GO_NORTH:
//          std::cout << "N" << std::endl;
          mPath.push("GO_NORTH");
          break;
        case GO_SOUTH:
//          std::cout << "S" << std::endl;
          mPath.push("GO_SOUTH");
          break;
        case GO_EAST:
//          std::cout << "E" << std::endl;
          mPath.push("GO_EAST");
          break;
        case GO_WEST:
//          std::cout << "W" << std::endl;
          mPath.push("GO_WEST");
          break;
        case QUIT:
          std::cout << "Quit: " << std::endl;
          mPath.push("QUIT");
          break;
        default:
          std::cout << "Unknown action" << std::endl;
          break;
        }
    }

    bool Action::SetType(int type_in)
    {
      if(type_in >= GO_NORTH && type_in < NONE)
        {
          this->type = type_in;
          return true;
        }
      return false;
    }
      
    int Action::GetType() const
    {
      return this->type;
    }
  }
}