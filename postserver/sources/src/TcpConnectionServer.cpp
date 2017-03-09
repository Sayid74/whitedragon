#include "TcpConnectionServer.h"
#include "Parameter.h"
#include <memory>

using namespace SayidYoung;
using namespace SayidYoung::WhiteDragon;

class TcpRequestImp : public TcpRequest
{
public:
	TcpRequestImp:: ~TcpRequestImp()
	{
	}
	TcpRequestImp:: TcpRequestImp(Parameters *p):_parameters(shared_ptr<Parameters>(p))
	{
	}
private:
	shared_ptr<Parameters> _parameters;
};

