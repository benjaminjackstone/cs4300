#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <queue>
#include "state.h"
#include "Snorlax.h"

bjs::Scavenger::Problem * mProblem;
bjs::Scavenger::Problem * mProblemRecharge;
std::queue<std::string> mPath;
double lastBacktoBaseCost = 0;
int gObjectsIndex = 0;
int dropoffIndex = 0;
double deduct = 0;
double MARGIN = 1.5 + 2.0 + 1.5;
double UCSpathCost = 0.0;
bool pickupcheck;
bool newObject;
bool needDropOff;
bool pickupLastTime;
bool examine;
bool need_charge;
//
bool end;
bool exploreMode = true;
int gCount = 0;
int gIndex = 0;
bool gdir;
bool gSetStartGoal = true;
bool gohome;
bool start = true;
bjs::Scavenger::State * startstate = new bjs::Scavenger::State();
double gcharge = 0, ghp = 0;
double basex=0,basey=0,basez=0;
std::vector<std::string> mObjectIds;
std::vector<std::string> mDropoffObjectIds;
std::map<std::string, std::string> mObjects;
 
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
        delete mModel;
    }
    //Check the North Interface every time the agent enters the cell
    bool Snorlax::LookNorth(const ai::Agent::Percept * percept) {
        //Flag if the previous action was a directinal look, gDir.  gIndex enforces which direction is was.
        if (gdir){
            gdir = false; // reset gDir to false since no action will be taken and I want to advance to next direction
            gIndex = 1; //advance to next Interface
            ai::Agent::PerceptAtom a = percept->GetAtom("LOOK"); // get the percept back from the LOOK action
            mNorthInterface = a.GetValue().c_str();  // set the agents CURRENT interface to this value.
            mNX = mX + 0; // simulate the agent peekiing into the next cell
            mNY = mY + 1000; // simulate the agent peekiing into the next cell
            mNew_coordinates = Coordinates(mNX, mNY);
            std::map<std::pair<double, double>, Cell*>::const_iterator it;
            it = this->mCells.find(mNew_coordinates);// see if this cell was previously seen 
            if(it == mCells.end())
            {
                if(mNorthInterface == "cliff"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "wall", "", "", 0,1,0,0,0);
                  mCells[mNew_coordinates] = c;
                }
                else if(mNorthInterface == "wall"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "cliff", "", "", 0,1,0,0,0);
                  mCells[mNew_coordinates] = c;
                }
                else { Cell *c =  new Cell(mCell_num, mNX, mNY, mZ, "", mNorthInterface, "", "", 0,1,0,0,0);
                    mCells[mNew_coordinates] = c;
                }
                //make a brand new cell.  the only thing I know so far is that the agent's current percept is this next cell's opposite interface
//                                  std::cout << "peek north interfaces unexplored cell created south interface set: " << mNorthInterface << " " << " " << " " << " " << " " << " " << std::endl;                                  
            }
            else
            {
                //if the cell was already in agent memory, update the cell's interface and interface seen boolean
//                  std::cout << "peek north's south interface updated at x: " << mNX << " mY: " << mNY << " " << it->second->msnorth << " " << it->second->mssouth << " " << it->second->mseast << " " << it->second->mswest << std::endl;
//                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, mCells[mNew_coordinates]->msnorth, mNorthInterface, mCells[mNew_coordinates]->mseast, mCells[mNew_coordinates]->mswest,mCells[mNew_coordinates]->north, true, mCells[mNew_coordinates]->east,mCells[mNew_coordinates]->west, mCells[mNew_coordinates]->visited);
                  mCells[mNew_coordinates]->mssouth = mNorthInterface;
                  mCells[mNew_coordinates]->south = true;
              }
      }
        //reaches this case if this is the first time peeking at this interface.  Ask if this interface has already been seen.  If not, issue a look command
      else if(mCells[mCoordinates]->north == false){
              mCells[mCoordinates]->north = true;
              gdir = true;
              deduct += .25;
              mCharge_num -= 0.25;
              return true;
      }
        //Reaches this case if this LOOK has previously been issued so don't need to waste another command.  Just need to make sure to
        //advance to the next interface and reset the current agent interface to this cell (Otherwise it would still be last cell's interface value)
      else {
          gIndex++;
          mNorthInterface = mCells[mCoordinates]->msnorth;
      }
        //False means that interface had previously been explored
        return false;
    }
    bool Snorlax::LookSouth(const ai::Agent::Percept * percept) {
        if (gdir){
            gdir = false;
            gIndex = 2;
            ai::Agent::PerceptAtom a = percept->GetAtom("LOOK");
            mSouthInterface = a.GetValue().c_str();
            mNX = mX + 0;
            mNY = mY - 1000;
            mNew_coordinates = Coordinates(mNX, mNY);
            std::map<std::pair<double, double>, Cell*>::const_iterator it;
            it = this->mCells.find(mNew_coordinates);                          
            if(it == mCells.end())
            {
                if(mSouthInterface == "cliff"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "wall", "", "", "", 1,0,0,0,0);
                  mCells[mNew_coordinates] = c;
                }
                else if(mSouthInterface == "wall"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "cliff", "", "", "",1,0,0,0,0);
                  mCells[mNew_coordinates] = c;
                }
                else { Cell *c =  new Cell(mCell_num, mNX, mNY, mZ, mSouthInterface, "", "", "",1,0,0,0,0);
                mCells[mNew_coordinates] = c;
//                                  std::cout << "peek south interfaces unexplored new cell created and north interface set: " << " " << " " << mSouthInterface << " " << " " << " " << " " << std::endl;                                                                    
                }
            }
            else
            {
//                                  std::cout << "peek south's north interface updated at x: " << mNX << " mY: " << mNY << " " << it->second->msnorth << " " << it->second->mssouth << " " << it->second->mseast << " " << it->second->mswest << std::endl;
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, mSouthInterface, mCells[mNew_coordinates]->mssouth, mCells[mNew_coordinates]->mseast, mCells[mNew_coordinates]->mswest, true, mCells[mNew_coordinates]->south, mCells[mNew_coordinates]->east,mCells[mNew_coordinates]->west, mCells[mNew_coordinates]->visited);
                  mCells[mNew_coordinates] = c;
              }
      }
      else if (mCells[mCoordinates]->south == false){
          mCells[mCoordinates]->south = true;
          gdir = true;
          deduct += .25;
          mCharge_num -= 0.25;
          return true;
      }
      else {
          gIndex++;
          mSouthInterface = mCells[mCoordinates]->mssouth;
      }
    return false;
    }
    bool Snorlax::LookEast(const ai::Agent::Percept * percept) {
        if (gdir){
            gdir = false;
            gIndex = 3;
            ai::Agent::PerceptAtom a = percept->GetAtom("LOOK");
            mEastInterface = a.GetValue().c_str();
            mNX = mX + 1000;
            mNY = mY;
            mNew_coordinates = Coordinates(mNX,mNY);
            std::map<std::pair<double, double>, Cell*>::const_iterator it;
            it = this->mCells.find(mNew_coordinates);                          
            if(it == mCells.end())
            {
                if(mEastInterface == "cliff"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "", "","wall", 0,0,0,1,0);
                  mCells[mNew_coordinates] = c;
                }
                else if(mEastInterface == "wall"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "", "", "cliff",0,0,0,1,0);
                  mCells[mNew_coordinates] = c;
                }
                else { Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "", "", mEastInterface,0,0,0,1,0);
                  mCells[mNew_coordinates] = c;
//                                  std::cout << "west interfaces unexplored new cell created east interface set: " << " " << " " << " " << " " << " " << " " << mWestInterface << std::endl;                                                                    
                }//                                  std::cout << "east interfaces unexplored new cell created west interface set: " << " " << " " << " " << " " << mEastInterface << " " << " " << std::endl;                                                                    
            }
            else
            {
//                                  std::cout << "peek east's west interface updated at x: " << mNX << " mY: " << mNY << " " << it->second->msnorth << " " << it->second->mssouth << " " << it->second->mseast << " " << it->second->mswest << std::endl;
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, mCells[mNew_coordinates]->msnorth, mCells[mNew_coordinates]->mssouth, mCells[mNew_coordinates]->mseast, mEastInterface, mCells[mNew_coordinates]->north,mCells[mNew_coordinates]->south,mCells[mNew_coordinates]->east, true, mCells[mNew_coordinates]->visited);
                  mCells[mNew_coordinates] = c;
              }
      }
      else if (mCells[mCoordinates]->east == false){                     
          mCells[mCoordinates]->east = true;
          gdir = true;
          deduct += .25;
          mCharge_num -= 0.25;
          return true;
      }
      else {
          gIndex++;
          mEastInterface = mCells[mCoordinates]->mseast;
      }
    return false;
    }
    bool Snorlax::LookWest(const ai::Agent::Percept * percept) {
        if (gdir){
            gdir = false;
            gIndex = 4;
            ai::Agent::PerceptAtom a = percept->GetAtom("LOOK");
            mWestInterface = a.GetValue().c_str();
            mNX = mX - 1000;
            mNY = mY;
            mNew_coordinates = Coordinates(mNX,mNY);
            std::map<std::pair<double, double>, Cell*>::const_iterator it;
            it = this->mCells.find(mNew_coordinates);                          
            if(it == mCells.end())
            {
                if(mWestInterface == "cliff"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "", "wall", "",0,0,1,0,0);
                  mCells[mNew_coordinates] = c;
                }
                else if(mWestInterface == "wall"){
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "", "cliff", "",0,0,1,0,0);
                  mCells[mNew_coordinates] = c;
                }
                else { Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, "", "", mWestInterface, "",0,0,1,0,0);
                  mCells[mNew_coordinates] = c;
//                                  std::cout << "west interfaces unexplored new cell created east interface set: " << " " << " " << " " << " " << " " << " " << mWestInterface << std::endl;                                                                    
                }
            }
            else
            {
//                                  std::cout << "peek west's east interface updated at x: " << mNX << " mY: " << mNY << " " << it->second->msnorth << " " << it->second->mssouth << " " << it->second->mseast << " " << it->second->mswest << std::endl;
                  Cell * c =  new Cell(mCell_num, mNX, mNY, mZ, mCells[mNew_coordinates]->msnorth,mCells[mNew_coordinates]->mssouth, mWestInterface, mCells[mNew_coordinates]->mswest,mCells[mNew_coordinates]->north,mCells[mNew_coordinates]->south, true, mCells[mNew_coordinates]->west,mCells[mNew_coordinates]->visited);
                  mCells[mNew_coordinates] = c;
              }
      }
      else if (mCells[mCoordinates]->west == false){
          mCells[mCoordinates]->west = true;
          gdir = true;
          deduct += .25;
          mCharge_num -= 0.25;
          return true;
      }
      else {
          gIndex++;
          mWestInterface = mCells[mCoordinates]->mswest;
      }
    return false;
    }
    void Snorlax::VisitCell(Cell * c, const ai::Agent::Percept *percept) {
        std::cout << gCount <<" cell number: " << c->mcell_num << std::endl;
        gIndex = 0;
        examine = false;
        gObjectsIndex = 0;
        c =  new Cell(mCell_num, mX, mY, mZ, mNorthInterface, mSouthInterface, mEastInterface, mWestInterface, 1,1,1,1, true);
        if(!mCells[mCoordinates]->visited){
            mCell_num = gCount;
            gCount++;
            mCells[mCoordinates]->visited = true;
            mCells[mCoordinates] = c;
        }
//        std::cout << gCount <<" final cell interfaces at mCell: " << c->mcell_num <<  " mX: " << mX << " mY: " << mY << " " << "mZ: " << mZ << " " << mNorthInterface << " " << mSouthInterface << " " << mEastInterface << " " << mWestInterface << std::endl;
        
        mState->SetCell(*mCells[mCoordinates]);
        mState->SetX(mX);
        mState->SetY(mY);
        mState->SetZ(mZ);
        mState->SetCharge(mCharge_num);
        mState->SetCellNum(mCell_num);
        mState->SetHP(mHp_num);
        mModel = new Model(mGoalx, mGoaly, mGoalz, mBase_num, mCharge_num, mHp_num, mX, mY, mZ, true);
        mProblem = new Problem(mState, mModel, true, false);        
//                    State * goal = new State();
        mProblem->SetGoal(startstate);
        mProblem->mCells = mCells;
        mProblemRecharge = new Problem(mState, mModel, false, true);
        mProblemRecharge->SetGoal(startstate);
        mProblemRecharge->mCells = mCells;
    }
    
    std::pair<double, double> Snorlax::Coordinates(double x, double y){
        std::pair<double, double> coordinates;
        coordinates.first = x;
        coordinates.second = y;
        return coordinates;
    }
    Cell * Snorlax::getCell(std::pair<double,double> pair){
        Cell * c =  new Cell();
        if(start){
            start = false;
            startstate->SetX(0);
            startstate->SetY(0);
            startstate->SetZ(0);
            std::cout << "NEW CELL AND START STATE" << std::endl;
            c =  new Cell(mCell_num, 0, 0, 0, "","","","",0,0,0,0,0); 
            mCells[pair] = c;
        }
        return mCells[pair];
    }
    void Snorlax::parsePercepts(const ai::Agent::Percept * percept, bool oldcell){
        std::stringstream ss;
        ss.str(percept->GetAtom("CHARGE").GetValue()); 
        ss.clear();
        ss >> mCharge_num;
        ss.str(percept->GetAtom("X_LOC").GetValue());
        ss.clear();
        ss >> mX;
        ss.str(percept->GetAtom("Y_LOC").GetValue()); 
        ss.clear();
        ss >> mY;
        ss.str(percept->GetAtom("Z_LOC").GetValue()); 
        ss.clear();
        ss >> mZ;
        startstate->SetX(basex);startstate->SetY(basey);startstate->SetZ(basez);
        ss.str(percept->GetAtom("HP").GetValue()); 
        ss.clear();
        ss >> mHp_num;
        if(!oldcell){
            for(unsigned int i = 0; i < percept->NumAtom(); i++)
            {
                std::string object_id;
                ai::Agent::PerceptAtom a = percept->GetAtom(i);
                if(std::strncmp(a.GetName().c_str(), "OBJECT_", 7) == 0)
                {
                    for(int j = 7; a.GetName().c_str()[j] != '\0'; j++){
                        object_id += a.GetName().c_str()[j];
                    }
                }
                if(object_id != "") {
                    mObjectIds.push_back(object_id);
//                    std::cout << object_id << std::endl;
                }
            }
            mObjectIds.push_back("end");
        }
    }
    ai::Agent::Action * Snorlax::Program(const ai::Agent::Percept * percept)
    {
        ai::Scavenger::Action *action = new ai::Scavenger::Action;
        if(need_charge || (end && mExhaustionMode)){ 
            mObjectIds.clear();
            parsePercepts(percept, true);
                mCoordinates = Coordinates(mX,mY);
                while(need_charge && mCharge_num < 100.00){
                    if(needDropOff && dropoffIndex < (int)mDropoffObjectIds.size() && mCharge_num > 0.5){
                        action->SetObjectId(mDropoffObjectIds[dropoffIndex]);
                        action->SetCode(ai::Scavenger::Action::DEPOSIT);
//                        std::cout << "ITEMS DEPOSITED" << std::endl;
                        dropoffIndex++;
                        mCharge_num-= 0.25;
//                        std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                        return action;
                    }
//                    std::cout << "RECHARGE ACTION ACTUAL CHARGE IS: "<< mCharge_num << std::endl;
                    action->SetCode(ai::Scavenger::Action::RECHARGE);
//                    std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                    return action;
                }
                if((end && need_charge) || (end && mExhaustionMode)){
                    action->SetCode(ai::Scavenger::Action::QUIT);
                    return action;
                }
                mDropoffObjectIds.clear();
                needDropOff = false;
                dropoffIndex = 0;
                need_charge = false;
       }
//                std::cout << "IS IT GETTING HERE " << mExhaustionMode << " " << mCharge_num << " " << (mCharge_num < 2.0) << std::endl;
        if(mExhaustionMode && mCharge_num < 2.0){
            while(!mPath.empty()){
                mPath.pop();
            }
            action->SetCode(ai::Scavenger::Action::QUIT);
            return action;
        }
        if (mPath.empty()){
        if(!gdir){
        if(!examine) {
            mObjectIds.clear();
            parsePercepts(percept, false);
            mState = new State();
            mModel = new Model();
            mProblem = new Problem(mState, mModel, true, false); 
            mProblemRecharge = new Problem(mState, mModel, false, true);
            mCoordinates = Coordinates(mX,mY);
            examine = true;
            pickupcheck = false;
        }
        Cell *c = getCell(mCoordinates);
        while(mObjectIds[gObjectsIndex] != "end"){
            std::map<std::string, std::string>::const_iterator it;
            if(!c->visited && !pickupcheck){
                    it = mObjects.find(mObjectIds[gObjectsIndex]);
                    if(it == mObjects.end()){
                        action->SetObjectId(mObjectIds[gObjectsIndex]);
                        action->SetCode(ai::Scavenger::Action::EXAMINE);
                        mObjects[mObjectIds[gObjectsIndex]] = "";
                        pickupcheck = true;
                        deduct += .5;
//                        std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                        mCharge_num -= 0.25;
                        return action;
                    }
            }
            if(!c->visited && pickupcheck){
//                        std::cout << "IS IT GETTING HERE " << mObjectIds[gObjectsIndex] << std::endl;
                        ai::Agent::PerceptAtom a = percept->GetAtom("EXAMINE");
                        std::string object = a.GetValue().c_str();
                        mObjects[mObjectIds[gObjectsIndex]] = object;
                        if ((object == mObjectIds[gObjectsIndex]+" orchid quadruped tiny tiny " || object == mObjectIds[gObjectsIndex]+" dayglo-orange cylinder medium medium ")){
//                            std::cout << mObjects[mObjectIds[gObjectsIndex]] << "=========== PICKED UP Object"<<std::endl;
                            action->SetObjectId(mObjectIds[gObjectsIndex]);
                            action->SetCode(ai::Scavenger::Action::PICKUP);
                            mDropoffObjectIds.push_back(mObjectIds[gObjectsIndex]);
                            needDropOff = true;
                            pickupcheck = false;
                            gObjectsIndex++;
//                            std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                            mCharge_num -= 0.25;
                            return action;
                    }
             }
          pickupcheck = false;
          gObjectsIndex++;
          }
       }
        Cell * c =  getCell(mCoordinates);
        if(gIndex == 0) {
              bool look = LookNorth(percept);
              if(look) {
                  action->SetDirection(ai::Scavenger::Location::Direction::NORTH);
                  action->SetCode(ai::Scavenger::Action::LOOK);
//                  std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                  return action;
              }
          }
          if(gIndex == 1) {
              bool look = LookSouth(percept);
              if(look) {
                  action->SetDirection(ai::Scavenger::Location::Direction::SOUTH);
                  action->SetCode(ai::Scavenger::Action::LOOK);
//                  std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                  return action;
              }
          }
          if(gIndex == 2) {
              bool look = LookEast(percept);
              if(look) {
                  action->SetDirection(ai::Scavenger::Location::Direction::EAST);
                  action->SetCode(ai::Scavenger::Action::LOOK);
//                  std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                  return action;
              }
          }
          if(gIndex == 3) {
              bool look = LookWest(percept);
              if(look) {
                  action->SetDirection(ai::Scavenger::Location::Direction::WEST);
                  action->SetCode(ai::Scavenger::Action::LOOK);
//                  std::cout << "CHARGE = " << mCharge_num << " \nACTION: " << action->GetCode() << std::endl;
                  return action;
              }
          }
          if(gIndex == 4){
            VisitCell(c, percept);
          }
        }
        if(exploreMode){
            exploreMode = false;
//            ai::Search::Frontier *fringe  = new ai::Search::DFFrontier;
////            ai::Search::Frontier *fringe  = new ai::Search::BFFrontier;
            ai::Search::Frontier *fringe  = new ai::Search::UCFrontier;
            ai::Search::Graph *search = new ai::Search::Graph(mProblem, fringe);
//            ai::Search::Tree *search = new ai::Search::Tree(mProblem, fringe);
            if (mPath.empty()){
                if(search->Search()){
                    std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
                    std::list<ai::Search::Node *>::const_iterator it;
                    UCSpathCost = solution->back()->GetPathCost();
                    if(MARGIN + UCSpathCost + lastBacktoBaseCost - (mZ/1000.0) > 100.0 || (lastBacktoBaseCost < 0.001 && gCount > 2)){
                            std::cout << "PATH TO NEXT CELL WON'T ALLOW FOR A SUCCESSFUL RETURN.....TURNING ON EXHAUSTION MODE...HERE WE GO........................." << std::endl;
//                            std::cout << "PATH TO NEXT CELL WON'T ALLOW FOR A SUCCESSFUL RETURN" << std::endl;
                            mExhaustionMode = 1;
                    }
                    for(it = solution->begin(); it != solution->end(); it++)
                    {
                        if((*it)->GetAction())
                        {
                            (*it)->GetAction()->Display();
                        }
                    }
                }
                else {
                    std::cout << "NO PATH FOUND TO A NEW CELL TRYING ROCKS...." << std::endl;
                    ai::Search::Frontier *fringe  = new ai::Search::UCFrontier;
                    mProblem->SetRocks(1);
                    ai::Search::Graph *search = new ai::Search::Graph(mProblem, fringe);
                    if(search->Search()){
                        std::list<ai::Search::Node *> *solution = search->GetSolution().GetList();
                        std::list<ai::Search::Node *>::const_iterator it;
                        UCSpathCost = solution->back()->GetPathCost();
                        if(MARGIN + UCSpathCost + lastBacktoBaseCost > 100.0 || (lastBacktoBaseCost < 0.001 && gCount > 2)){
//                            std::cout << "PATH TO NEXT CELL WON'T ALLOW FOR A SUCCESSFUL RETURN.....TURNING ON EXHAUSTION MODE" << std::endl;
                          //  std::cout << "PATH TO NEXT CELL WON'T ALLOW FOR A SUCCESSFUL RETURN" << std::endl;
                            mExhaustionMode = 1;
                        }
                        for(it = solution->begin(); it != solution->end(); it++)
                        {
                            if((*it)->GetAction())
                            {
                                (*it)->GetAction()->Display();
                            }
                        }
                        }
                        else {
                            std::cout << "NO PATH FOUND...." << std::endl;
                            end = true;
                        }
                }
                lastBacktoBaseCost = 0.0;
                if(!mExhaustionMode){
                    ai::Search::Frontier *recharge_fringe  = new ai::Search::AStarFrontier;
                    ai::Search::Graph *recharge_search = new ai::Search::Graph(mProblemRecharge, recharge_fringe);
                    if(recharge_search->Search()){
                             std::list<ai::Search::Node *> *solution = recharge_search->GetSolution().GetList();
                            lastBacktoBaseCost = solution->back()->GetPathCost() + UCSpathCost;
                            double pathome = solution->back()->GetPathCost();
                            if(pathome < 0.001) pathome = UCSpathCost;
//                             std::cout << "UCS PATH COST ============ " << UCSpathCost << std::endl;
//                             std::cout << "HOME PATH COST ============ " << pathome << std::endl;
//                             std::cout << "HP ============ " << mHp_num << std::endl;
//                             std::cout << "RECHARGE ============ " << mCharge_num-deduct << std::endl;
//                             std::cout << "Charge: " << mCharge_num << " Est. CHARGE to get home if I go to a new cell first: " << MARGIN + pathome + UCSpathCost - (mZ/1000.0) << std::endl;
                             if(MARGIN + pathome + UCSpathCost - (mZ/1000.0) > mCharge_num - .5|| end){
                                 gohome = true;
                                    while(!mPath.empty()){
                                        mPath.pop();
                                     }
                                 
                                     std::list<ai::Search::Node *> *solution = recharge_search->GetSolution().GetList();
                                     std::list<ai::Search::Node *>::const_iterator it;
                                     for(it = solution->begin(); it != solution->end(); it++)
                                    {
                                        if((*it)->GetAction())
                                        {
                                            (*it)->GetAction()->Display();
                                        }
                                     }
                             }

                            }
                }
                else {
                    if(!mExhaustionMode){
                        std::cout << "SHOULD ALWAYS FIND A PATH BACK HOME...TRYING ROCKS" << std::endl;
                        mProblemRecharge->SetRocks(1);
                        ai::Search::Frontier *recharge_fringe  = new ai::Search::AStarFrontier;
                        ai::Search::Graph *recharge_search = new ai::Search::Graph(mProblemRecharge, recharge_fringe);
                        if(recharge_search->Search()){
                             std::list<ai::Search::Node *> *solution = recharge_search->GetSolution().GetList();
                            lastBacktoBaseCost = solution->back()->GetPathCost() + UCSpathCost;
                            double pathome = solution->back()->GetPathCost();
                            if(pathome < 0.001) pathome = UCSpathCost;
//                             std::cout << "UCS PATH COST ============ " << UCSpathCost << std::endl;
//                             std::cout << "HOME PATH COST ============ " << pathome << std::endl;
//                             std::cout << "HP ============ " << mHp_num << std::endl;
//                             std::cout << "RECHARGE ============ " << mCharge_num-deduct << std::endl;
//                             std::cout << "Charge: " << mCharge_num << " Est. CHARGE to get home if I go to a new cell first: " << MARGIN + pathome + UCSpathCost - (mZ/1000.0) << std::endl;
                             if(MARGIN + pathome + UCSpathCost - (mZ/1000.0) > mCharge_num - .5|| end){
                                 gohome = true;
                                     while(!mPath.empty()){
                                         mPath.pop();
                                     }
                                     std::list<ai::Search::Node *> *solution = recharge_search->GetSolution().GetList();
                                     std::list<ai::Search::Node *>::const_iterator it;
                                    for(it = solution->begin(); it != solution->end(); it++)
                                    {
                                        if((*it)->GetAction())
                                        {
                                            (*it)->GetAction()->Display();
                                        }
                                    }
                             }

                            }
                    else {
                        std::cout << "STILL DIDNt FIND A PATH BACK HOME...something went wrong" << std::endl;
                    }
                    }
                
                }
            }
                    deduct = 0;
         }
        if(!mPath.empty() /*&& basePath*/) 
        {
                    std::string act = mPath.front();
                    mPath.pop();
                    if(mPath.empty()) {
                        exploreMode = true;
                        if(gohome){
                            gohome = false;
                            need_charge = true;
                        }
                    }
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
                    else if(act == "QUIT")
                    {
                        action->SetCode(ai::Scavenger::Action::QUIT);
                    }
                    else {std::cout << "SHOULDN'T GET HERE AT ALL????" << std::endl;}
//                std::cout << "CHARGE = " << mCharge_num << " \nACTION MOVES: " << action->GetCode() << std::endl;
            return action;
        }
        else {
            std::cout << "Agent is safely quitting..." << std::endl;
            action->SetCode((int)ai::Scavenger::Action::QUIT);
        }
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
      if(type_in >= GO_NORTH && type_in <= QUIT)
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
//