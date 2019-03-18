#include "HAPISession.h"

namespace Echo
{
	void startHAPISessionInProcess(HAPI_Session& Session)
	{
		// Create Session
		HAPI_CreateInProcessSession(&Session);
	}

	void startHAPISessionTcpSocket(HAPI_Session& Session)
	{
		HAPI_ThriftServerOptions ServerOptions;
		ServerOptions.autoClose = true;
		ServerOptions.timeoutMs = 3000;

		// Start Server (This can be done on another machine)
		HAPI_StartThriftSocketServer(&ServerOptions, 8089, nullptr);

		// Create Session (Client connect to server)
		HAPI_CreateThriftSocketSession(&Session, "localhost", 8089);
	}

	void startHAPISessionNamedPipe(HAPI_Session& Session)
	{
		HAPI_ThriftServerOptions ServerOptions;
		ServerOptions.autoClose = true;
		ServerOptions.timeoutMs = 3000;

		// Start Server
		HAPI_StartThriftNamedPipeServer(&ServerOptions, "hapi", nullptr);

		// Create Session
		HAPI_CreateThriftNamedPipeSession(&Session, "hapi");
	}
}