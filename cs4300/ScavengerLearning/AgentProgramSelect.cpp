
#include <ai_agent.h>

#include "Snorlax.h"
#include "Manual.h"

namespace ai
{
  namespace Agent
  {
    AgentProgram *AgentProgramSelect(Options *opts)
    {
      AgentProgram *agent_program = 0;
      switch(opts->GetArgChr("agent"))
        {
        case 's':
          agent_program = new bjs::Scavenger::Snorlax(opts);
          break;
        case 'm':
          agent_program = new bjs::Scavenger::Manual();
          break;

        default:
          std::cout << "Unknown agent program " << opts->GetArgChr("agent") << std::endl
                    << "s : Snorlax" << std::endl
                    << "m : Manual" << std::endl
      
		    << std::endl;
          exit(1);
          break;
        }
      return agent_program;
    }
  }
}
