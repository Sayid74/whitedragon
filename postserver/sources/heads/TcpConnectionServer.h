#pragma once

#include "LatenceProcessor.h"

using namespace std;
namespace SayidYoung
{
	namespace WhiteDragon
	{
		class TcpRequest : public Request
		{
		public:
			size_t parameterCount()const override;
			shared_ptr<string> parameterNameAt(size_t index)const override;
			shared_ptr<Content> getValueByName(string name)const override;
		};

		class TcpResponse : public Response
		{
		public:
			bool outText(const TextMedia& media, const string& message)override;
			bool outError(const int state, const string &message) override;
			bool isFinished();
		};

		class TcpConnectionServer
		{
		public:
			static TcpConnectionServer* connect();
		public:
			TcpConnectionServer();
			~TcpConnectionServer();
			TcpRequest& getRequest();
			TcpResponse& getResponse();
		private:
			TcpConnectionServer();
		};
	}
}
